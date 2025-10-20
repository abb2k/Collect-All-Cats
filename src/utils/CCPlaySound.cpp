#include "CCPlaySound.hpp"

CCPlaySound::CCPlaySound()
: _speed(1.0f)
, _pitch(1.0f)
, _volume(1.0f)
{
}

CCPlaySound::~CCPlaySound()
{
}

CCPlaySound* CCPlaySound::create(const char* path, float speed, float pitch, float volume)
{
    CCPlaySound* pRet = new CCPlaySound();
    if (pRet && pRet->initWithSound(path, speed, pitch, volume))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool CCPlaySound::initWithSound(const char* path, float speed, float pitch, float volume)
{
    if (!path) return false;

    _path = path;
    _speed = speed;
    _pitch = pitch;
    _volume = volume;

    return true;
}

void CCPlaySound::update(float time)
{
    FMODAudioEngine::sharedEngine()->playEffect(_path.c_str(), _speed, _pitch, _volume);
}
