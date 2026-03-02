#include "scenes/simple_scene.h"

void UpdateText();

SimpleScene* scene;

int main()
{
    scene = new SimpleScene("simple_scene");
    scene->screen_height = 720;
    scene->screen_width = 1280;
    
    scene->Init();

    scene->GetButton("button1")->SetCallback(&UpdateText);

    scene->Update();
    scene->Terminate();
	return 0;
}

void UpdateText()
{
    std::string pick = scene->GetDropdown("dropdown1")->GetSelected();
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    if (pick == "red")
        color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    if (pick == "green")
        color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    if (pick == "blue")
        color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

    scene->GetText("text1")->SetColor(color);
    scene->GetText("text1")->UpdateText(scene->GetInputText("input1")->GetText());
}
