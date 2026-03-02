#include "ui/dropdown.h"

Dropdown::Dropdown(std::string id, ImVec2 pos, float size) : UIElement(id, pos)
{
	this->size = size;
}

void Dropdown::Render()
{
    ImGui::SetNextWindowPos(uiPos, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f); // transparent window

    ImGui::Begin(("##window" + GetID()).c_str(), nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings);

    // Size of the dropdown
    ImGui::SetNextItemWidth(size);

    // Styling (light gray)
    ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, hoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, activeColor);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, popupColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);

    // Remove borders
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);

    // Dropdown
    if (ImGui::BeginCombo(("##combo" + GetID()).c_str(), items[currentItem].c_str()))
    {
        for (int i = 0; i < items.size(); i++)
        {
            bool selected = (currentItem == i);
            if (ImGui::Selectable(items[i].c_str(), selected))
                currentItem = i;

            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Restore style
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);

    ImGui::End();
}

void Dropdown::ChangeColors(ImVec4 bg, ImVec4 hover, ImVec4 active, ImVec4 popup, ImVec4 text)
{
    bgColor = bg;
    hoverColor = hover;
    activeColor = active;
    popupColor = popup;
    textColor = text;
}

void Dropdown::ClearItems()
{
    items.clear();
}

void Dropdown::AddItem(std::string item)
{
    items.push_back(item);
}

void Dropdown::AddItems(std::vector<std::string> items)
{
    for (int i = 0; i < items.size(); i++)
        this->items.push_back(items[i]);
}

std::string Dropdown::GetSelected()
{
    return items[currentItem];
}
