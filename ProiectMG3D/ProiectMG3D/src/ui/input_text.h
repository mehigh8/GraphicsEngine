#pragma once

#include "ui/ui_element.h"
#include "cstring"

class InputText : public UIElement {
public:
	float size;
	ImVec4 bgColor;
	ImVec4 textColor;

	char inputBuffer[256];

	InputText(std::string id, ImVec2 pos, float size, ImVec4 bgColor, ImVec4 textColor, const char* defaultText);

	void Render();

	std::string GetText();
};
