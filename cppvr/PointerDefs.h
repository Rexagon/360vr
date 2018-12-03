#pragma once

#include <memory>

namespace ej
{
	template<typename T>
	class PointerDefs
	{
	public:
		using ptr = std::shared_ptr<T>;
		using uptr = std::unique_ptr<T>;
	};
}