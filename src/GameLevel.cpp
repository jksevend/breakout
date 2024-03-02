#include "GameLevel.h"

#include <fstream>
#include <sstream>
#include <utility>

auto Toyengine::GameLevel::initialize(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth,
                                      unsigned int levelHeight) -> void
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / height;
    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameEntity entity(pos, size,
                                  this->m_AssetManager->getTexture2D("block_solid"),
                                  glm::vec3(0.8f, 0.8f, 0.7f)
                );
                entity.setSolid(true);
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

                glm::vec2 pos(x * unit_width, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->m_Bricks.emplace_back(
                    pos, size, this->m_AssetManager->getTexture2D("block"), color
                );
            }
        }
    }
}

Toyengine::GameLevel::GameLevel(std::shared_ptr<AssetManager> assetManager) : m_AssetManager(std::move(assetManager))
{
}

auto Toyengine::GameLevel::draw(const std::unique_ptr<Renderer>& renderer) -> void
{
    for (auto& brick : this->m_Bricks)
    {
        if (!brick.isDestroyed())
            brick.draw(renderer);
    }
}

auto Toyengine::GameLevel::isCompleted() const -> bool
{
    for (auto& brick : this->m_Bricks)
    {
        if (!brick.isSolid() && !brick.isDestroyed())
            return false;
    }
    return true;
}

auto Toyengine::GameLevel::load(std::string file, unsigned int levelWidth, unsigned int levelHeight) -> void
{
    this->m_Bricks.clear();

    unsigned int tileCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (!tileData.empty())
            this->initialize(tileData, levelWidth, levelHeight);
    }
}
