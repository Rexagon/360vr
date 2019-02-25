#include "Video\AudioStream.h"

extern "C" {
#include <libavutil/opt.h>
}

AudioStream::AudioStream(AVStream * stream) :
	m_stream(stream)
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

	m_swrContext = swr_alloc();
	av_opt_set_int(m_swrContext, "in_channel_count", m_decoderContext->channels, 0);
	av_opt_set_int(m_swrContext, "out_channel_count", m_decoderContext->channels, 0);
	av_opt_set_int(m_swrContext, "in_channel_layout", m_decoderContext->channel_layout, 0);
	av_opt_set_int(m_swrContext, "out_channel_layout", m_decoderContext->channel_layout, 0);
	av_opt_set_int(m_swrContext, "in_sample_rate", m_decoderContext->sample_rate, 0);
	av_opt_set_int(m_swrContext, "out_sample_rate", m_decoderContext->sample_rate, 0);
	av_opt_set_sample_fmt(m_swrContext, "in_sample_fmt", m_decoderContext->sample_fmt, 0);
	av_opt_set_sample_fmt(m_swrContext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	swr_init(m_swrContext);

	const auto provider = [this](int16_t const** samples, size_t& sampleCount) {
		//TODO: call decoder and fill samples
		return false;
	};

	m_audioPlayer = std::make_unique<AudioPlayer>(
		m_decoderContext->channels, m_decoderContext->sample_rate, provider);

	m_audioPlayer->play();
}

void AudioStream::clear()
{
	if (!m_isInitialized) {
		return;
	}

	m_isInitialized = false;

	std::unique_lock<std::mutex> lockDecoder(m_decoderMutex);

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
	m_decodingId = 0;
	m_lastAudioDts = 0.0;
	m_lastAudioDelay = 0.0;
	m_hasStarted = false;
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
		m_lastAudioDts = m_stream->start_time * av_q2d(m_stream->time_base);
		m_hasStarted = true;
	}

	const auto nextDts = m_lastAudioDts + m_lastAudioDelay;
	
}

void AudioStream::setVolume(float volume)
{
	if (m_audioPlayer != nullptr) {
		m_audioPlayer->setVolume(volume);
	}
}

uint64_t AudioStream::getCurrentDecodingId() const
{
}

bool AudioStream::isInitialized()
{
}
