#pragma once

#include "PointerDefs.h"

namespace ej
{
	class Core;

	/**
	 * \brief Base manager
	 * 
	 * 
	 */
	class BaseManager
	{
	public:
		/**
		 * \brief Default constructor
		 * \param core Owner of this manager
		 */
		explicit BaseManager(const Core& core) : m_core(core) {}
		
		/**
		 * \brief Default destructor
		 */
		virtual ~BaseManager() = default;

	protected:
		const Core& m_core;
	};
}

/**
 * \class ej::BaseManager
 * \ingroup Core
 */