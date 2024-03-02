#pragma once

#include <memory>

#include "AssetManager.h"
#include "BallObject.h"
#include "GameLevel.h"
#include "Renderer.h"
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
    auto keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) -> void;

    class Application
    {
        glm::vec2 kPlayerSize = glm::vec2(100.0f, 20.0f);
        glm::vec2 kPlayerVelocity = glm::vec2(500.0f, 0.0f);
        glm::vec2 kBallVelocity = glm::vec2(100.0f, -350.0f);

        float kBallRadius = 12.5f;

        GLFWwindow* mp_Window;
        Toyengine::GameEntity* mp_Player;
        Toyengine::BallObject* mp_Ball;

        std::shared_ptr<Toyengine::AssetManager> m_AssetManager;
        std::unique_ptr<Toyengine::Renderer> m_Renderer;

        ApplicationState m_State;
        std::vector<Toyengine::GameLevel> m_Level;
        unsigned int m_CurrentLevel;

        auto handleEvents(float deltaTime) -> void;
        auto update(float deltaTime) -> void;
        auto render() -> void;

        auto handeCollisions() -> void;
        auto checkCollision(Toyengine::GameEntity& one, Toyengine::GameEntity& two) -> bool;

    public:
        inline static bool sKeys[1024];

        Application();
        ~Application();

        void run();
    };
}
