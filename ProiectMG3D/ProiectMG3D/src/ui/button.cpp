#include "ui/button.h"

Button::Button(std::string id, ImVec2 pos, ImVec2 size, std::string text, void (*callback)()) : UIElement(id, pos)
{
	this->size = size;
	this->text = std::move(text);
	this->callback = callback;
}

void Button::Render()
{
    ImGui::SetNextWindowPos(uiPos);
    ImGui::SetNextWindowBgAlpha(0.0f); // transparent window

    // Begin borderless window
    ImGui::Begin(("##HUDButton" + GetID()).c_str(), nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings);

    ImGui::PushStyleColor(ImGuiCol_Button, normalColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);

    // Button with visible text
    if (ImGui::Button((text + "##my_button" + GetID()).c_str(), size))
    {
        callback();
    }

    ImGui::PopStyleColor(4);
    ImGui::End();
}

void Button::ChangeColors(ImVec4 normalColor, ImVec4 hoveredColor, ImVec4 activeColor, ImVec4 textColor)
{
    this->normalColor = normalColor;
    this->hoveredColor = hoveredColor;
    this->activeColor = activeColor;
    this->textColor = textColor;
}

void Button::SetCallback(void (*callback)())
{
    this->callback = callback;
}