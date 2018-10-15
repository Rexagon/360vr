#pragma once

namespace ej
{
	class ManagerLocator;

	class BaseManager
	{
	public:
		explicit BaseManager(const ManagerLocator& locator) : m_locator(locator) {}
		virtual ~BaseManager() = default;

	protected:
		const ManagerLocator& m_locator;
	};
}