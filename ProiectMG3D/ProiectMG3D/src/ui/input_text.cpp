#include "ui/input_text.h"

InputText::InputText(std::string id, ImVec2 pos, float size, ImVec4 bgColor, ImVec4 textColor, const char* defaultText) : UIElement(id, pos)
{
	this->size = size;
	this->bgColor = bgColor;
	this->textColor = textColor;

	strcpy_s(inputBuffer, defaultText);
}

void InputText::Render()
{
    ImGui::SetNextWindowPos(uiPos);
    ImGui::SetNextWindowBgAlpha(0.0f); // transparent window background

    // Begin a borderless window
    ImGui::Begin(("##InputWindow" + GetID()).c_str(), nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings);

    // Set width of the input box
    ImGui::SetNextItemWidth(size);

    // Push style to remove frame and set background color
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor); // custom background color
    ImGui::PushStyleColor(ImGuiCol_Text, textColor); // text color

    // InputText without title/label (use ## to make an internal ID)
    ImGui::InputText(("##input" + GetID()).c_str(), inputBuffer, IM_ARRAYSIZE(inputBuffer));

    isAccessed = ImGui::IsItemActive();

    // Restore styles
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();

    ImGui::End();
}

std::string InputText::GetText()
{
    return std::string(inputBuffer);
}