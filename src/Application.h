#pragma once

#include <memory>

#include "AssetManager.h"
#include "BallObject.h"
#include "GameLevel.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


#ifdef NDEBUG
    #define TOYENGINE_DEBUG false
#else
#define TOYENGINE_DEBUG true
#endif

namespace Breakout
{
    enum Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    typedef std::tuple<bool, Direction, glm::vec2> Collision;

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

        float m_ShakeTime = 0.0f;

        GLFWwindow* mp_Window;
        Toyengine::GameEntity* mp_Player;
        Toyengine::BallObject* mp_Ball;

        std::shared_ptr<Toyengine::AssetManager> m_AssetManager;
        std::unique_ptr<Toyengine::Renderer> m_Renderer;
        std::unique_ptr<Toyengine::ParticleGenerator> m_ParticleGenerator;
        std::unique_ptr<Toyengine::PostProcessor> m_PostProcessor;

        ApplicationState m_State;
        std::vector<Toyengine::GameLevel> m_Level;
        unsigned int m_CurrentLevel;

        auto handleEvents(float deltaTime) -> void;
        auto update(float deltaTime) -> void;
        auto render() -> void;

        auto handleCollisions() -> void;
        auto checkCollision(Toyengine::BallObject& ball, Toyengine::GameEntity& box) -> Collision;
        auto vectorDirection(glm::vec2 target) -> Direction;

        auto resetLevel() -> void;
        auto resetPlayer() -> void;

    public:
        inline static bool sKeys[1024];

        Application();
        ~Application();

        void run();
    };
}
