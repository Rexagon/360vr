#pragma once

#include <Resources/Material.h>

class WidgetMaterial : public ej::Material, public ej::PointerDefs<WidgetMaterial>
{
public:
	WidgetMaterial(const ej::Core& core);

	void bind() override;

private:

};