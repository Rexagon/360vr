#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace app
{
	template<typename T>
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

			std::vector<State*> connections;
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

		void addConnection(const std::string& begin, const std::string& end);

		const State* getCurrentState() const;

		void setTransitionSpeed(float speed);

		const Transition& getCurrentTransition() const;

	private:
		State* m_currentState = nullptr;

		float m_transitionSpeed = 1.0f;
		Transition m_currentTransition;

		std::unordered_map<std::string, std::unique_ptr<State>> m_states;
	};
}

template <typename T>
void app::StateGraph<T>::update(const float dt)
{
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
	}
}

template <typename T>
void app::StateGraph<T>::startTransition(const std::string& name)
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

template <typename T> 
template <typename ... Args>
const typename app::StateGraph<T>::State* app::StateGraph<T>::addState(const std::string& name, Args&& ... args)
{
	auto state = std::make_unique<State>(name, std::forward<Args>(args)...);
	const auto result = state.get();
	m_states.try_emplace(name, std::move(state));
	return result;
}

template <typename T>
void app::StateGraph<T>::setCurrentState(const std::string& name)
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

template <typename T>
void app::StateGraph<T>::addConnection(const std::string& begin, const std::string& end)
{
	const auto itBegin = m_states.find(begin);
	if (itBegin == m_states.end()) {
		return;
	}

	const auto itEnd = m_states.find(end);
	if (itEnd == m_states.end()) {
		return;
	}

	itBegin->second->connections.emplace_back(itEnd->second.get());
}

template <typename T>
const typename app::StateGraph<T>::State* app::StateGraph<T>::getCurrentState() const
{
	return m_currentState;
}

template <typename T>
void app::StateGraph<T>::setTransitionSpeed(const float speed)
{
	if (speed <= 0.0f) {
		return;
	}
	m_transitionSpeed = speed;
}

template <typename T>
const typename app::StateGraph<T>::Transition& app::StateGraph<T>::getCurrentTransition() const
{
	return m_currentTransition;
}
