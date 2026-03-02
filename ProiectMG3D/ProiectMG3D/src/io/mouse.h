#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
    mouse class to handle mouse callbacks
*/

class Mouse {
private:
    static double x;
    static double y;

    static double lastX;
    static double lastY;

    static double dx;
    static double dy;

    static double scrollDx;
    static double scrollDy;

    // if this is the first change in the mouse position
    static bool firstMouse;

    static bool buttons[];
    static bool buttonsChanged[];

public:
    static void CursorPosCallback(GLFWwindow* window, double _x, double _y);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseWheelCallback(GLFWwindow* window, double dx, double dy);

    static double GetMouseX();
    static double GetMouseY();

    static double GetDX();
    static double GetDY();

    static double GetScrollDX();
    static double GetScrollDY();

    static bool Button(int button);
    static bool ButtonChanged(int button);
    static bool ButtonWentUp(int button);
    static bool ButtonWentDown(int button);
};
