#include "Utils.hpp"

/// @brief 
/// returns a random number between min and max inclusive [min, max]
float Utils::GetRandomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(min, std::nextafter(max, std::numeric_limits<float>::max()));

    return dist(gen);
}

/// @brief 
/// returns a random number between min and max inclusive [min, max]
int Utils::GetRandomInt(int min, int max){    
    if (max != INT_MAX)
        max++;
    return min + rand() % (max - min);
}