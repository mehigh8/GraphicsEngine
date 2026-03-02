#pragma once

#include "ui/ui_element.h"
#include <vector>

class Dropdown : public UIElement {
public:
	int currentItem = 0;
	std::vector<std::string> items;

	float size;
	ImVec4 bgColor = ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
	ImVec4 hoverColor = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
	ImVec4 activeColor = ImVec4(0.80f, 0.80f, 0.80f, 1.0f);
	ImVec4 popupColor = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
	ImVec4 textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	Dropdown(std::string id, ImVec2 pos, float size);

	void Render();

	void ChangeColors(ImVec4 bg, ImVec4 hover, ImVec4 active, ImVec4 popup, ImVec4 text);

	void ClearItems();
	void AddItem(std::string item);
	void AddItems(std::vector<std::string> items);

	std::string GetSelected();
};
