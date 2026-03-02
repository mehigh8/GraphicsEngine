#include "ui/text.h"

Text::Text(std::string id, ImVec2 pos, std::string text, ImVec4 color, float fontSize) : UIElement(id, pos)
{
	this->text = std::move(text);
	this->color = color;
    this->fontSize = fontSize;
}

void Text::Render()
{
    if (!enabled)
        return;

    ImDrawList* draw = ImGui::GetForegroundDrawList();

    ImFont* font = ImGui::GetFont();

    draw->AddText(
        font,
        fontSize,
        uiPos,                    // Screen position (pixels)
        ImGui::ColorConvertFloat4ToU32(color),      // Color (RGBA)
        text.c_str()
    );
}

void Text::UpdateText(std::string text)
{
    this->text = std::move(text);
}

void Text::SetColor(ImVec4 color)
{
    this->color = color;
}