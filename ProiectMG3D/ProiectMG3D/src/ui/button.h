#pragma once

#include "ui/ui_element.h"

class Button : public UIElement {
public:
    ImVec4 normalColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);       // normal (light gray)
    ImVec4 hoveredColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);      // hovered (almost white)
    ImVec4 activeColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);       // pressed (darker gray)
    ImVec4 textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // black text

    ImVec2 size;
    std::string text;
    void (*callback)();

    Button(std::string id, ImVec2 pos, ImVec2 size, std::string text, void (*callback)());

    void Render();

    void ChangeColors(ImVec4 normalColor, ImVec4 hoveredColor, ImVec4 activeColor, ImVec4 textColor);
    void SetCallback(void (*callback)());
};
