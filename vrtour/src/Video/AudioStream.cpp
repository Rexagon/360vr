#include "Video/AudioStream.h"

extern "C" {
#include <libavutil/opt.h>
}

AudioStream::AudioStream(VideoState& state, AVStream * stream) :
	m_state(state), m_stream(stream)
{
}

AudioStream::~AudioStream()
{
	clear();
}

void AudioStream::init()
{
	if (m_isInitialized) {
		return;
	}

	if (m_stream == nullptr || m_stream->codecpar->codec_type != AVMEDIA_TYPE_AUDIO) {
		throw std::runtime_error("Stream doesn't contain audio\n");
	}

	printf("Audio codec id: %d\n", m_stream->codecpar->codec_id);

	m_decoder = avcodec_find_decoder(m_stream->codecpar->codec_id);
	if (!m_decoder) {
		clear();
		throw std::runtime_error("Can't find video decoder!\n");
	}

	m_decoderContext = avcodec_alloc_context3(m_decoder);

	if (avcodec_parameters_to_context(m_decoderContext, m_stream->codecpar) < 0) {
		clear();
		throw std::runtime_error("Failed to provide parameters\n");
	}

	if (avcodec_open2(m_decoderContext, m_decoder, nullptr) < 0) {
		clear();
		throw std::runtime_error("Unable to open decoder\n");
	}

	// Init sound

	const auto channelCount = 1;
	const auto channelLayout = AV_CH_LAYOUT_MONO;

	m_swrContext = swr_alloc();
	av_opt_set_int(m_swrContext, "in_channel_count", m_decoderContext->channels, 0);
	av_opt_set_int(m_swrContext, "out_channel_count", channelCount, 0);
	av_opt_set_int(m_swrContext, "in_channel_layout", m_decoderContext->channel_layout, 0);
	av_opt_set_int(m_swrContext, "out_channel_layout", channelLayout, 0);
	av_opt_set_int(m_swrContext, "in_sample_rate", m_decoderContext->sample_rate, 0);
	av_opt_set_int(m_swrContext, "out_sample_rate", m_decoderContext->sample_rate, 0);
	av_opt_set_sample_fmt(m_swrContext, "in_sample_fmt", m_decoderContext->sample_fmt, 0);
	av_opt_set_sample_fmt(m_swrContext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	swr_init(m_swrContext);

	printf("Channel count: %d\n", channelCount);

	const auto provider = [this](int16_t const** samples, size_t& sampleCount) {
		m_hasData = false;

		decode();

		if (!m_hasData || m_buffer.empty()) {
			return false;
		}

		*samples = reinterpret_cast<int16_t*>(m_buffer.data());
		sampleCount = m_sampleCount;

		return true;
	};

	m_audioPlayer = std::make_unique<AudioPlayer>(channelCount, m_decoderContext->sample_rate, provider);

	m_audioPlayer->setVolume(100.0f);

	m_isInitialized = true;
}

void AudioStream::clear()
{
	if (!m_isInitialized) {
		return;
	}

	m_isInitialized = false;

	std::unique_lock<std::mutex> lockDecoder(m_decoderMutex);
	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);

	m_audioPlayer.reset();

	while (!m_frameQueue.empty()) {
		av_frame_free(&m_frameQueue.back());
		m_frameQueue.pop_back();
	}

	m_decoder = nullptr;
	if (m_decoderContext != nullptr) {
		avcodec_free_context(&m_decoderContext);
	}

	m_buffer.clear();

	if (m_swrContext != nullptr) {
		swr_free(&m_swrContext);
	}

	m_stream = nullptr;
	m_hasData = false;
	m_decodingId = 0;
	m_hasStarted = false;
}

void AudioStream::receive(AVPacket* packet)
{
	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);

	if (!m_isInitialized) {
		return;
	}

	if (avcodec_send_packet(m_decoderContext, packet) < 0) {
		throw std::runtime_error("Unable to send audio packet for decoding");
	}

	AVFrame* frame = av_frame_alloc();

	if (avcodec_receive_frame(m_decoderContext, frame) < 0) {
		av_frame_free(&frame);
	}
	else {
		//printf("Received audio frame: %lld\n", frame->pts);

		{
			std::lock_guard<std::mutex> queueLock(m_queueMutex);
			m_frameQueue.push_back(frame);
		}

		if (m_audioPlayer->getStatus() != sf::SoundStream::Playing) {
			m_audioPlayer->play();
		}
	}
}

void AudioStream::decode()
{
	std::unique_lock<std::mutex> decoderLock(m_decoderMutex);

	if (!m_isInitialized || m_frameQueue.empty()) {
		return;
	}

	AVFrame* frame = m_frameQueue.front();

	if (frame == nullptr) {
		return;
	}

	if (!m_hasStarted) {
		m_state.updateAudioTimings(m_stream->start_time * av_q2d(m_stream->time_base), 0.0);
		m_hasStarted = true;
	}

	const auto nextDts = m_state.getNextAudioDts();
	const auto delta = nextDts - m_state.getCurrentTime();

	//printf("TME: %f\tDTS: %f\tDT: %f\n", m_state.getCurrentTime(), nextDts, delta);

	if (m_state.getCurrentTime() < nextDts) {
		return;
	}

	m_state.setVideoOffset(delta);

	const auto dataSize = av_samples_get_buffer_size(nullptr,
		m_audioPlayer->getChannelCount(), frame->nb_samples,
		AV_SAMPLE_FMT_S16, 1);

	{
		std::unique_lock<std::mutex> queueLock(m_queueMutex);
		m_frameQueue.pop_front();
	}

	{
		std::unique_lock<std::mutex> bufferLock(m_bufferMutex);

		m_buffer.resize(dataSize);

		auto dataPtr = m_buffer.data();
		const auto result = swr_convert(m_swrContext, &dataPtr, dataSize,
			const_cast<const uint8_t**>(frame->data), frame->nb_samples);

		if (result < 0) {
			m_sampleCount = 0;
		}
		else {
			m_sampleCount = result;
		}

		const auto duration = static_cast<double>(m_sampleCount) / m_audioPlayer->getSampleRate();
		m_state.updateAudioTimings(nextDts, duration);

		m_hasData = true;
		m_decodingId++;
	}

	av_frame_free(&frame);
}

void AudioStream::flush()
{
	if (m_isInitialized) {
		avcodec_send_packet(m_decoderContext, nullptr);
	}
}

int AudioStream::getIndex() const
{
	if (!m_isInitialized) {
		return -1;
	}

	return m_stream->index;
}

size_t AudioStream::shouldReceive() const
{
	return m_frameQueue.size() < 256;
}

void AudioStream::setVolume(float volume)
{
	if (m_audioPlayer != nullptr) {
		m_audioPlayer->setVolume(volume);
	}
}

uint64_t AudioStream::getCurrentDecodingId() const
{
	return m_decodingId;
}

bool AudioStream::isInitialized() const
{
	return m_isInitialized;
}
