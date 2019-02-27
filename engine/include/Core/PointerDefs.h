#pragma once

#include <memory>

namespace ej
{
	/**
	 * \brief Smart pointer definitions
	 * \tparam T 
	 */
	template<typename T>
	class PointerDefs
	{
	public:
		using ptr = std::shared_ptr<T>;
		using uptr = std::unique_ptr<T>;
	};
}

/**
 * \class ej::PointerDefs
 * \ingroup Core
 */