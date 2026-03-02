#include "ui/slider.h"

Slider::Slider(std::string id, ImVec2 pos, ImVec2 size, float value, ImVec4 bgColor, ImVec4 fillColor) : UIElement(id, pos)
{
	this->size = size;
	this->value = value;
	this->bgColor = bgColor;
	this->fillColor = fillColor;
}

void Slider::Render()
{
	if (!enabled)
		return;

	ImDrawList* draw = ImGui::GetForegroundDrawList();

	// Background
	draw->AddRectFilled(uiPos, ImVec2(uiPos.x + size.x, uiPos.y + size.y), ImGui::ColorConvertFloat4ToU32(bgColor), 4.0f);

	// Fill
	draw->AddRectFilled(uiPos, ImVec2(uiPos.x + size.x * value, uiPos.y + size.y), ImGui::ColorConvertFloat4ToU32(fillColor), 4.0f);
}

void Slider::UpdateValue(float newValue)
{
	value = newValue;

	if (value > 1.0f)
		value = 1.0f;
	if (value < 0.0f)
		value = 0.0f;
}

float Slider::GetValue()
{
	return value;
}