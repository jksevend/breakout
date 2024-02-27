#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Breakout
{
    enum ApplicationState
    {
        ACTIVE,
        MENU,
        WIN,
    };

    auto sizeCallback(GLFWwindow* window, int width, int height) -> void;

    class Application
    {
        GLFWwindow* mp_Window;

        ApplicationState m_State;

        auto handleEvents() -> void;
        auto update(float deltaTime) -> void;
        auto render() -> void;

    public:
        Application();
        ~Application();

        void run();
    };
}
