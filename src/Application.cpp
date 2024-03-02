#include "Application.h"

#include <stdexcept>
#include "spdlog/spdlog.h"

auto Breakout::sizeCallback(GLFWwindow* window, int width, int height) -> void
{
    glViewport(0, 0, width, height);
}

auto Breakout::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) -> void
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Application::sKeys[key] = true;
        else if (action == GLFW_RELEASE)
            Application::sKeys[key] = false;
    }
}

auto Breakout::Application::handleEvents(float deltaTime) -> void
{
    if (this->m_State == ACTIVE)
    {
        const auto velocity = this->kPlayerVelocity * deltaTime;
        // move playerboard
        if (sKeys[GLFW_KEY_A])
        {
            if (this->mp_Player->getPosition().x >= 0.0f)
            {
                this->mp_Player->translate(-velocity);
                if (this->mp_Ball->isStuck())
                {
                    this->mp_Ball->translate(-velocity);
                }
            }
        }
        if (sKeys[GLFW_KEY_D])
        {
            if (this->mp_Player->getPosition().x <= 1000 - this->mp_Player->getSize().x)
            {
                this->mp_Player->translate(velocity);
                if (this->mp_Ball->isStuck())
                {
                    this->mp_Ball->translate(velocity);
                }
            }
        }

        if (sKeys[GLFW_KEY_SPACE])
        {
            this->mp_Ball->setStuck(false);
        }
    }
}

auto Breakout::Application::update(float deltaTime) -> void
{
    this->mp_Ball->move(deltaTime, 1000);

    this->handeCollisions();

    if (this->mp_Ball->getPosition().y >= 800) // did ball reach bottom edge?
    {
        this->resetLevel();
        this->resetPlayer();
    }
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
        this->mp_Player->draw(this->m_Renderer);
        this->mp_Ball->draw(this->m_Renderer);
    }
}

auto Breakout::Application::handeCollisions() -> void
{
    for (const auto box : this->m_Level.at(this->m_CurrentLevel).getBricks())
    {
        if (!box->isDestroyed())
        {
            if (Collision collision = this->checkCollision(*this->mp_Ball, *box); std::get<0>(collision))
            // if collision is true
            {
                // destroy block if not solid
                if (!box->isSolid())
                {
                    box->setDestroyed(true);
                }

                // collision resolution
                const Direction dir = std::get<1>(collision);
                const glm::vec2 difference = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // horizontal collision
                {
                    this->mp_Ball->setVelocity(glm::vec2(-this->mp_Ball->getVelocity().x,
                                                         this->mp_Ball->getVelocity().y));
                    // relocate
                    float penetration = this->mp_Ball->getRadius() - std::abs(difference.x);
                    if (dir == LEFT)
                        this->mp_Ball->translate(glm::vec2(penetration, 0));
                    else
                        this->mp_Ball->translate(glm::vec2(-penetration, 0));
                }
                else // vertical collision
                {
                    this->mp_Ball->setVelocity(glm::vec2(this->mp_Ball->getVelocity().x,
                                                         -this->mp_Ball->getVelocity().y));
                    // relocate
                    const float penetration = this->mp_Ball->getRadius() - std::abs(difference.y);
                    if (dir == UP)
                        this->mp_Ball->translate(glm::vec2(0, -penetration));
                    else
                        this->mp_Ball->translate(glm::vec2(0, penetration));
                }
            }
        }
    }

    if (const Collision result = this->checkCollision(*this->mp_Ball, *this->mp_Player); !this->mp_Ball->isStuck() &&
        std::get<0>(result))
    {
        // check where it hit the board, and change velocity based on where it hit the board
        const float centerBoard = this->mp_Player->getPosition().x + this->mp_Player->getSize().x / 2.0f;
        const float distance = (this->mp_Ball->getPosition().x + this->mp_Ball->getRadius()) - centerBoard;
        const float percentage = distance / (this->mp_Player->getSize().x / 2.0f);
        // then move accordingly
        constexpr float strength = 2.0f;
        const glm::vec2 oldVelocity = this->mp_Ball->getVelocity();
        this->mp_Ball->setVelocity(glm::vec2(kBallVelocity.x * percentage * strength, -this->mp_Ball->getVelocity().y));
        this->mp_Ball->setVelocity(normalize(this->mp_Ball->getVelocity()) * length(oldVelocity));
        this->mp_Ball->setVelocity(glm::vec2(0, -1.0f * abs(this->mp_Ball->getVelocity().y)));
    }
}

auto Breakout::Application::checkCollision(Toyengine::GameEntity& one, Toyengine::GameEntity& two) -> bool
{
    const bool collisionX = one.getPosition().x + one.getSize().x >= two.getPosition().x &&
        two.getPosition().x + two.getSize().x >= one.getPosition().x;
    const bool collisionY = one.getPosition().y + one.getSize().y >= two.getPosition().y &&
        two.getPosition().y + two.getSize().y >= one.getPosition().y;
    return collisionX && collisionY;
}

auto Breakout::Application::checkCollision(Toyengine::BallObject& ball, Toyengine::GameEntity& box) -> Collision
{
    // get center point circle first
    glm::vec2 center(ball.getPosition() + ball.getRadius());
    // calculate AABB info (center, half-extents)
    glm::vec2 aabbHalfs(box.getSize().x / 2.0f, box.getSize().y / 2.0f);
    glm::vec2 aabbCenter(box.getPosition().x + aabbHalfs.x, box.getPosition().y + aabbHalfs.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabbCenter;
    glm::vec2 clamped = glm::clamp(difference, -aabbHalfs, aabbHalfs);
    // now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabbCenter + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (length(difference) < ball.getRadius())
    {
        // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, this->vectorDirection(difference), difference);
    }
    return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

auto Breakout::Application::vectorDirection(glm::vec2 target) -> Direction
{
    constexpr glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f), // up
        glm::vec2(1.0f, 0.0f), // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f) // left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        if (const float dot_product = dot(normalize(target), compass[i]); dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return static_cast<Direction>(best_match);
}

auto Breakout::Application::resetLevel() -> void
{
    if (this->m_CurrentLevel == 0)
    {
        this->m_Level[0].load("levels/one.lvl", 1000, 800 / 2);
    }

    else if (this->m_CurrentLevel == 1)
    {
        this->m_Level[1].load("levels/two.lvl", 1000, 800 / 2);
    }
    else if (this->m_CurrentLevel == 2)
    {
        this->m_Level[2].load("levels/three.lvl", 1000, 800 / 2);
    }
    else if (this->m_CurrentLevel == 3)
    {
        this->m_Level[3].load("levels/four.lvl", 1000, 800 / 2);
    }
}

auto Breakout::Application::resetPlayer() -> void
{
    this->mp_Player->setSize(this->kPlayerSize);
    this->mp_Player->setPosition(glm::vec2(1000 / 2.0f - this->kPlayerSize.x / 2.0f, 800 - this->kPlayerSize.y));
    this->mp_Ball->reset(
        this->mp_Player->getPosition() + glm::vec2(this->kPlayerSize.x / 2.0f - this->kBallRadius, -(this->kBallRadius * 2.0f)),
        this->kBallVelocity);
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
    glfwSetKeyCallback(this->mp_Window, keyCallback);

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
    this->m_AssetManager->loadTexture2D("paddle", "sprites/paddle.png", false);
    this->m_AssetManager->loadTexture2D("face", "sprites/awesomeface.png", true);

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

    auto playerPos = glm::vec2(1000 / 2.0f - this->kPlayerSize.x / 2.0f, 800 - this->kPlayerSize.y
    );

    this->mp_Player = new Toyengine::GameEntity(playerPos, kPlayerSize, this->m_AssetManager->getTexture2D("paddle"));

    auto ballPos = playerPos + glm::vec2(kPlayerSize.x / 2.0f - kBallRadius,
                                         -kBallRadius * 2.0f);
    this->mp_Ball = new Toyengine::BallObject(ballPos, this->kBallRadius, this->kBallVelocity,
                                              this->m_AssetManager->getTexture2D("face"));
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

        this->handleEvents(deltaTime);
        this->update(deltaTime);
        this->render();

        glfwSwapBuffers(this->mp_Window);
    }
}
