#include "Utility.h"

#include <format>
#include <fstream>

auto Toyengine::Utility::loadFile(std::string path) -> std::string
{
    std::ifstream file(path, std::fstream::in);
    if (!file.is_open())
    {
        const auto message = std::format("Cannot open file {}", path);
        throw std::runtime_error(message);
    }
    auto sourceCode = std::string(std::istreambuf_iterator(file),
                                  std::istreambuf_iterator<char>());
    return sourceCode;
}
