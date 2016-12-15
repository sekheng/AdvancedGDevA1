/******************************************************************************/
/*!
\file	MusicSystem.h
\author Lee Sek Heng
\par email: 150629Z@mymail.nyp.edu.sg
\brief
A Music System that is derived from IrrKlang, and make IrrKlang more compatible with this project
*/
/******************************************************************************/
#ifndef MUSIC_SYSTEM_H
#define MUSIC_SYSTEM_H

#include "MusicEntity2D.h"
#include <map>

typedef std::map<std::string, MusicEntity2D*> strMEmap;

class MusicSystem
{
public:
    static MusicSystem &accessing()
    {
        static MusicSystem cantTouchThis;
        return cantTouchThis;
    }
    virtual void Init();
    //This is gonna be a pain
    virtual void Update(double dt);
  
    ISoundEngine *musicEngine;

    virtual bool playMusic(const std::string &songName);

    virtual bool playBackgroundMusic(const std::string &songName);

    virtual void clearEverything();
    virtual void setPlayerPos(Vector3 &pos);
    virtual void setTimeToUpdate(const double &dt);

    MusicEntity2D *accessTheMusic(const std::string &songName);
    MusicEntity2D *theOnlyBackgroundMusic;

private:
    std::map<std::string, MusicEntity2D*> all_the_Music;
    bool beginLoadingMusic(const std::string &fileName);
    size_t loopingAndFindKey(std::vector<std::string> &theKeys, const std::string &whatyouwant);
    Vector3 *playerPos;
    double TimeUpdate, m_accumulatedTime;

    MusicSystem();
    ~MusicSystem();
};

#endif