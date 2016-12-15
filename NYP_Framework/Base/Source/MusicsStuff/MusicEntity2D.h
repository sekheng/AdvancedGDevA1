/******************************************************************************/
/*!
\file           MusicEntity2D.h
\author      Lee Sek Heng
\par            email: 150629Z@mymail.nyp.edu.sg
\brief
MusicEntity2D defines 2D sounds from IrrKlang and making Irrklang sounds more compatible with this project
*/
/******************************************************************************/
#ifndef MUSIC_ENTITY_2D_H
#define MUSIC_ENTITY_2D_H

#include "irrklang.h"
#include <queue>
#include "Vector3.h"
#include <vector>

using namespace irrklang;

class MusicEntity2D {
public:
    MusicEntity2D();
    virtual ~MusicEntity2D();

    virtual void Init(const std::string &theName, const std::string &fileName, const float &theVol, const int &playHowManyTime = 1, const bool &unlimitedTimes = false, const bool &loop = false);
    virtual void Play();
    virtual void Update(double dt);
    virtual void SetVolume(const float &theNum);
    virtual void SetPosition(const Vector3 &pos);
    virtual void SetUnlimitedPlayTimes(const bool &playTimes);
    virtual void SetConstantLooping(const bool &loop);
    virtual void SetNumTimeToPlay(const int &numTime);
    virtual void setISoundSouce(const std::string &theFile);

    virtual void Stop(double dt);

    float getVolume();
    unsigned getMaxTimesToPlay();
    bool getUnlimitedTimes();
    bool getLooping();
    std::string getFileName();

    std::string getName();
    void setName(const std::string &zeName);

protected:
    ISoundSource *SoundSource;
    float volume_;
    //std::queue<ISound*> HistoryOfPlayTimes;
    std::vector<ISound*> HistoryOfPlayTimes;
    unsigned maxTimeToPlay;
    bool unlimitedTimes, loopIt, justStopPlaying;
    std::string fileName;
    double m_ElapsedTime;
    std::string entityName_;
};

#endif