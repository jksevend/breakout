#pragma once
#include <string>
#include <vector>

#include "AssetManager.h"
#include "GameEntity.h"

namespace Toyengine
{
    class GameLevel
    {
        std::vector<GameEntity> m_Bricks;

        std::shared_ptr<AssetManager> m_AssetManager;

        auto initialize(std::vector<std::vector<unsigned int>> tileData,
                        unsigned int levelWidth, unsigned int levelHeight) -> void;

    public:
        explicit GameLevel(std::shared_ptr<AssetManager> assetManager);
        ~GameLevel() = default;

        auto draw(const std::unique_ptr<Renderer>& renderer) -> void;
        [[nodiscard]] auto isCompleted() const -> bool;
        [[nodiscard]] auto getBricks() const -> std::vector<GameEntity>&;

        auto load(std::string file, unsigned int levelWidth, unsigned int levelHeight) -> void;
    };
}
