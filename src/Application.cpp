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

    if (this->m_State == ACTIVE)
    {
        this->m_Renderer->drawSprite(this->m_AssetManager->getTexture2D("background"), glm::vec2(0.0f, 0.0f),
                                     glm::vec2(1000, 800));
        this->m_Level.at(this->m_CurrentLevel).draw(this->m_Renderer);
    }
}

Breakout::Application::Application() : m_State(ACTIVE)
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

    this->m_AssetManager = std::make_shared<Toyengine::AssetManager>();

    auto shader = this->m_AssetManager->loadShader("textureShader", "shaders/texture2d.vert", "shaders/texture2d.frag");
    const glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1000),
                                            static_cast<float>(800), 0.0f, -1.0f, 1.0f);
    shader.activate();
    shader.setInteger("image", 0);
    shader.setMatrix4("projection", projection);

    this->m_Renderer = std::make_unique<Toyengine::Renderer>(shader);

    this->m_AssetManager->loadTexture2D("background", "sprites/background.jpg", false);
    this->m_AssetManager->loadTexture2D("block", "sprites/block.png", false);
    this->m_AssetManager->loadTexture2D("block_solid", "sprites/block_solid.png", false);

    Toyengine::GameLevel level1(this->m_AssetManager);
    level1.load("levels/one.lvl", 1000, 800 / 2);

    Toyengine::GameLevel level2(this->m_AssetManager);
    level2.load("levels/two.lvl", 1000, 800 / 2);

    Toyengine::GameLevel level3(this->m_AssetManager);
    level3.load("levels/three.lvl", 1000, 800 / 2);

    Toyengine::GameLevel level4(this->m_AssetManager);
    level4.load("levels/four.lvl", 1000, 800 / 2);

    this->m_Level.emplace_back(level1);
    this->m_Level.emplace_back(level2);
    this->m_Level.emplace_back(level3);
    this->m_Level.emplace_back(level4);
    this->m_CurrentLevel = 0;
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
