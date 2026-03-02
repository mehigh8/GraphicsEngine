#pragma once

#include "ui/ui_element.h"

class Slider : public UIElement {
public:
	float value; // 0f-1f
	ImVec4 bgColor;
	ImVec4 fillColor;
	ImVec2 size;

	Slider(std::string id, ImVec2 pos, ImVec2 size, float value, ImVec4 bgColor, ImVec4 fillColor);

	void Render() override;

	void UpdateValue(float newValue);
	float GetValue();
};