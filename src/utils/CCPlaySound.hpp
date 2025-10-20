#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CCPlaySound : public CCActionInstant
{
public:
    static CCPlaySound* create(const char* path, float speed = 1.0f, float pitch = 1.0f, float volume = 1.0f);

    virtual void update(float time) override;

protected:
    CCPlaySound();
    virtual ~CCPlaySound();

    bool initWithSound(const char* path, float speed, float pitch, float volume);

private:
    std::string _path;
    float _speed;
    float _pitch;
    float _volume;
};
