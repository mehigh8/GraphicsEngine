#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Keyboard {
private:
    static bool keys[];
    static bool keysChanged[];

public:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static bool Key(int key);
    static bool KeyChanged(int key);
    static bool KeyWentUp(int key);
    static bool KeyWentDown(int key);
};
