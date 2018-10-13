#pragma once

#include <typeindex>
#include <cassert>
#include <memory>

namespace core
{
class Core;

class RequireManager
{
protected:
	template<typename T>
	std::shared_ptr<T> get() const;

private:
	friend class Core;

	Core* m_core;
};

class BaseManager : public RequireManager
{
public:
	virtual ~BaseManager() = default;
};

}