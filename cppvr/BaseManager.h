#pragma once

#include "PointerDefs.h"

namespace ej
{
	class Core;

	class BaseManager
	{
	public:
		explicit BaseManager(const Core& core) : m_core(core) {}
		virtual ~BaseManager() = default;

	protected:
		const Core& m_core;
	};
}