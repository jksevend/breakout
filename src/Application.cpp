#include "Application.h"

#include <stdexcept>
#include "spdlog/spdlog.h"

auto Breakout::sizeCallback(GLFWwindow* window, int width, int height) -> void
{
    glViewport(0, 0, width, height);
}

auto Breakout::Application::handleEvents() -> void
{
    if (glfwGetKey(this->mp_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->mp_Window, true);
    }
}

auto Breakout::Application::update(float deltaTime) -> void
{
}

auto Breakout::Application::render() -> void
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

Breakout::Application::Application()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->mp_Window = glfwCreateWindow(1000, 800, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(this->mp_Window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetFramebufferSizeCallback(this->mp_Window, sizeCallback);

    glViewport(0, 0, 1000, 800);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spdlog::info("Application initialized");
}

Breakout::Application::~Application()
{
    glfwTerminate();
    spdlog::info("Application destroyed");
}

void Breakout::Application::run()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(this->mp_Window))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        this->handleEvents();
        this->update(deltaTime);
        this->render();

        glfwSwapBuffers(this->mp_Window);
    }
}
