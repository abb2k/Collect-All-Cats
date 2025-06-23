#include "utils.hpp"

float Utils::GetRandomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(min, std::nextafter(max, std::numeric_limits<float>::max()));

    return dist(gen);
}