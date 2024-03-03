#include "GameLevel.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>

auto Toyengine::GameLevel::initialize(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth,
                                      unsigned int levelHeight) -> void
{
    // calculate dimensions
    const unsigned int height = tileData.size();
    const unsigned int width = tileData[0].size();
    const float unit_width = static_cast<float>(levelWidth) / static_cast<float>(width);
    const float unit_height = static_cast<float>(levelHeight) / static_cast<float>(height);
    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                const glm::vec2 pos(unit_width * x, unit_height * y);
                const glm::vec2 size(unit_width, unit_height);
                auto entity = new GameEntity(pos, size,
                                             this->m_AssetManager->getTexture2D("block_solid"),
                                             glm::vec3(0.8f, 0.8f, 0.7f)
                );
                entity->setSolid(true);
                this->m_Bricks.emplace_back(entity);
            }
            else if (tileData[y][x] > 1)
            {
                auto color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(static_cast<float>(x) * unit_width, static_cast<float>(y) * unit_height);
                glm::vec2 size(unit_width, unit_height);
                auto entity = new GameEntity(pos, size, this->m_AssetManager->getTexture2D("block"), color);
                this->m_Bricks.emplace_back(entity);
            }
        }
    }
}

Toyengine::GameLevel::GameLevel(std::shared_ptr<AssetManager> assetManager) : m_AssetManager(std::move(assetManager))
{
}

auto Toyengine::GameLevel::draw(const std::unique_ptr<Renderer>& renderer) -> void
{
    for (const auto brick : this->m_Bricks)
    {
        if (!brick->isDestroyed())
            brick->draw(renderer);
    }
}

auto Toyengine::GameLevel::isCompleted() const -> bool
{
    return std::ranges::all_of(this->m_Bricks, [](const auto brick)
    {
        return !brick->isSolid() && !brick->isDestroyed();
    });
}

auto Toyengine::GameLevel::getBricks() -> std::vector<GameEntity*>
{
    return this->m_Bricks;
}

auto Toyengine::GameLevel::load(const std::string& file, unsigned int levelWidth, unsigned int levelHeight) -> void
{
    this->m_Bricks.clear();

    if (std::ifstream fstream(file); fstream)
    {
        std::vector<std::vector<unsigned int>> tileData;
        std::string line;
        unsigned int tileCode;
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode)
            {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        if (!tileData.empty())
        {
            this->initialize(tileData, levelWidth, levelHeight);
        }
    }
}
