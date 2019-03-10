#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace app
{
	template<typename T, typename C>
	class StateGraph final
	{
	public:
		struct State final
		{
			template<typename... Args>
			explicit State(const std::string& name, Args&&... args) : 
				name(name), data(std::forward<Args>(args)...)
			{}

			std::string name;
			T data;

			std::vector<std::pair<State*, C>> connections;
		};

		struct Transition final
		{
			State* begin = nullptr;
			State* end = nullptr;
			float value = 0.0f;
			bool isPaused = false;
		};

		void update(float dt);

		void startTransition(const std::string& name);

		template<typename... Args>
		const State* addState(const std::string& name, Args&&... args);

		void setCurrentState(const std::string& name);

		template<typename... Args>
		void addConnection(const std::string& begin, const std::string& end, Args&&... args);

		const State* getCurrentState() const;

		void setTransitionSpeed(float speed);

		bool wasTransitionFinished() const;

		const Transition& getCurrentTransition() const;

	private:
		State* m_currentState = nullptr;

		float m_transitionSpeed = 1.0f;
		Transition m_currentTransition;

		bool m_transitionFinished = false;

		std::unordered_map<std::string, std::unique_ptr<State>> m_states;
	};
}

template <typename T, typename C>
void app::StateGraph<T, C>::update(const float dt)
{
	m_transitionFinished = false;

	if (m_currentTransition.begin == nullptr || 
		m_currentTransition.isPaused) 
	{
		return;
	}

	m_currentTransition.value += dt * m_transitionSpeed;
	if (m_currentTransition.value >= 1.0f) {
		m_currentState = m_currentTransition.end;
		m_currentTransition.begin = m_currentState;
		m_currentTransition.end = nullptr;

		m_currentTransition.value = 0.0f;
		m_currentTransition.isPaused = true;

		m_transitionFinished = true;
	}
}

template <typename T, typename C>
void app::StateGraph<T, C>::startTransition(const std::string& name)
{
	const auto it = m_states.find(name);
	if (it == m_states.end()) {
		return;
	}

	m_currentTransition.begin = m_currentState;
	m_currentTransition.end = it->second.get();

	m_currentTransition.value = 0.0f;
	m_currentTransition.isPaused = false;
}

template <typename T, typename C>
template <typename ... Args>
const typename app::StateGraph<T, C>::State* app::StateGraph<T, C>::addState(const std::string& name, Args&& ... args)
{
	auto state = std::make_unique<State>(name, std::forward<Args>(args)...);
	const auto result = state.get();
	m_states.try_emplace(name, std::move(state));
	return result;
}

template <typename T, typename C>
void app::StateGraph<T, C>::setCurrentState(const std::string& name)
{
	const auto it = m_states.find(name);
	if (it == m_states.end()) {
		return;
	}

	m_currentState = it->second.get();

	m_currentTransition.begin = m_currentState;
	m_currentTransition.end = nullptr;
	m_currentTransition.value = 0.0f;
	m_currentTransition.isPaused = true;
}

template <typename T, typename C>
template <typename ... Args>
void app::StateGraph<T, C>::addConnection(const std::string& begin, const std::string& end, Args&&... args)
{
	const auto itBegin = m_states.find(begin);
	if (itBegin == m_states.end()) {
		return;
	}

	const auto itEnd = m_states.find(end);
	if (itEnd == m_states.end()) {
		return;
	}

	itBegin->second->connections.emplace_back(itEnd->second.get(), C{ std::forward<Args>(args)... });
}

template <typename T, typename C>
const typename app::StateGraph<T, C>::State* app::StateGraph<T, C>::getCurrentState() const
{
	return m_currentState;
}

template <typename T, typename C>
void app::StateGraph<T, C>::setTransitionSpeed(const float speed)
{
	if (speed <= 0.0f) {
		return;
	}
	m_transitionSpeed = speed;
}

template <typename T, typename C>
bool app::StateGraph<T, C>::wasTransitionFinished() const
{
	return m_transitionFinished;
}

template <typename T, typename C>
const typename app::StateGraph<T, C>::Transition& app::StateGraph<T, C>::getCurrentTransition() const
{
	return m_currentTransition;
}
