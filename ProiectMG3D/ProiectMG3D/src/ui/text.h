#pragma once

#include "ui/ui_element.h"

class Text : public UIElement {
public:
	std::string text;
	ImVec4 color;
	float fontSize;

	Text(std::string id, ImVec2 pos, std::string text, ImVec4 color, float fontSize);

	void Render() override;

	void UpdateText(std::string text);

	void SetColor(ImVec4 color);
};