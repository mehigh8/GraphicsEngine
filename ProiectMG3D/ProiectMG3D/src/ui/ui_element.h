#pragma once

#include "imgui.h"
#include <string>

class UIElement {
private:
	std::string id;
public:
	ImVec2 uiPos;
	bool enabled;
	bool isAccessed;

	UIElement(std::string id, ImVec2 pos)
	{
		this->id = std::move(id);
		uiPos = pos;
		enabled = true;
		isAccessed = false;
	}

	virtual void Render() = 0;

	std::string GetID()
	{
		return id;
	}
};
