#include "MusicEntity2D.h"
#include "MyMath.h"
#include "MusicSystem.h"

MusicEntity2D::MusicEntity2D()
{
    SoundSource = nullptr;
    volume_ = 1.f;
    maxTimeToPlay = 1;
    unlimitedTimes = loopIt = false;
    m_ElapsedTime = 0;
    justStopPlaying = true;
}

MusicEntity2D::~MusicEntity2D()
{
    while (HistoryOfPlayTimes.size() > 0)
    {
        ISound *theFront = HistoryOfPlayTimes.front();
        if (theFront)
        {
            theFront->drop();
            theFront = 0;
        }
        HistoryOfPlayTimes.erase(HistoryOfPlayTimes.begin());
    }
}

void MusicEntity2D::SetVolume(const float &theNum)
{
    volume_ = theNum;
    volume_ = Math::Clamp(volume_, 0.01f, 1.f);
    SoundSource->setDefaultVolume(volume_);
}

void MusicEntity2D::Init(const std::string &theName, const std::string &fileName, const float &theVol, const int &playHowManyTime, const bool &unlimitedTimes, const bool &loop)
{
    setName(theName);
    setISoundSouce(fileName);
    SetVolume(theVol);
    SetNumTimeToPlay(playHowManyTime);
    SetUnlimitedPlayTimes(unlimitedTimes);
    SetConstantLooping(loop);
}

void MusicEntity2D::Play()
{
    if (HistoryOfPlayTimes.size() < maxTimeToPlay || unlimitedTimes == true)
    {
        ISound *thEffect = MusicSystem::accessing().musicEngine->play2D(SoundSource, loopIt, false, true);
        HistoryOfPlayTimes.push_back(thEffect);
        justStopPlaying = false;
    }
}

void MusicEntity2D::Update(double dt)
{
    m_ElapsedTime = dt;
    if (HistoryOfPlayTimes.size() > 0 && HistoryOfPlayTimes.front()->isFinished())
    {
        ISound *theFront = HistoryOfPlayTimes.front();
        HistoryOfPlayTimes.erase(HistoryOfPlayTimes.begin());
        if (theFront)
        {
            theFront->drop();
            theFront = 0;
        }
    }
    else if (HistoryOfPlayTimes.size() > 0 && justStopPlaying)
    {
        Stop(dt);
    }
}

void MusicEntity2D::SetConstantLooping(const bool &loop)
{
    loopIt = loop;
}

void MusicEntity2D::SetUnlimitedPlayTimes(const bool &playTimes)
{
    unlimitedTimes = playTimes;
}

void MusicEntity2D::SetNumTimeToPlay(const int &numTime)
{
    if (numTime > 0)
        maxTimeToPlay = numTime;
}

void MusicEntity2D::SetPosition(const Vector3 &pos)
{

}

void MusicEntity2D::Stop(double dt)
{
    for (std::vector<ISound*>::iterator it = HistoryOfPlayTimes.begin(), end = HistoryOfPlayTimes.end(); it != end;)
    {
        ISound * theEffect = (*it);
        if (theEffect)
        {
            float decrement = 0;
            if (dt > Math::EPSILON)
                decrement = (float)dt;
            else
                decrement = (float)m_ElapsedTime;
            if (theEffect->getVolume() - decrement > Math::EPSILON)
            {
                theEffect->setVolume(theEffect->getVolume() - decrement);
                ++it;
            }
            else
            {
                theEffect->stop();
                theEffect->drop();
                theEffect = 0;
                bool justBreakIt = false;
                std::vector<ISound*>::iterator it2;
                if (it + 1 == end)
                {
                    justBreakIt = true;
                }
                else
                    it2 = it + 1;
                HistoryOfPlayTimes.erase(it);
                if (justBreakIt)
                    break;
                it = it2;
            }
        }
    }
}

float MusicEntity2D::getVolume()
{
    return volume_;
}

unsigned MusicEntity2D::getMaxTimesToPlay()
{
    return maxTimeToPlay;
}

bool MusicEntity2D::getLooping()
{
    return loopIt;
}

bool MusicEntity2D::getUnlimitedTimes()
{
    return unlimitedTimes;
}

void MusicEntity2D::setISoundSouce(const std::string &theFile)
{
    fileName = theFile;
    SoundSource =
        MusicSystem::accessing().musicEngine->addSoundSourceFromFile(fileName.c_str());
}

void MusicEntity2D::setName(const std::string &zeName)
{
    entityName_ = zeName;
}

std::string MusicEntity2D::getName()
{
    return entityName_;
}