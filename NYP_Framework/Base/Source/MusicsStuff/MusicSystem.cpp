#include "MusicSystem.h"
#include <fstream>
#include <sstream>
#include <set>

MusicSystem::MusicSystem()
{

}

MusicSystem::~MusicSystem()
{
    musicEngine->drop();
}

void convertStringToUpperCaps(std::string &zeStr)
{
    for (std::string::iterator it = zeStr.begin(), end = zeStr.end(); it != end; ++it)
    {
        (*it) = toupper(*it);
    }
}

void MusicSystem::Init()
{
    musicEngine = createIrrKlangDevice();
    theOnlyBackgroundMusic = nullptr;
    beginLoadingMusic("DrivenFiles//MusicDriven.csv");
    musicEngine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 0));
    musicEngine->update();
    m_accumulatedTime = TimeUpdate = 0;
}

//This is gonna be a pain
void MusicSystem::Update(double dt)
{
    //How to update listener position
    m_accumulatedTime += dt;
    if (m_accumulatedTime > TimeUpdate)
    {
        musicEngine->update();
    }
    for (std::map<std::string, MusicEntity2D*>::iterator it = all_the_Music.begin(), end = all_the_Music.end(); it != end; ++it)
    {
        it->second->Update(dt);
    }
}


bool MusicSystem::beginLoadingMusic(const std::string &fileName)
{
    std::ifstream file(fileName.c_str());
    if (file.is_open())
    {
        MusicEntity2D *theMusic = nullptr;
        std::string data = "";
        std::vector<std::string> keys;
        std::vector<std::string> values;
        std::set<std::string> keepingTrackOfFileNames;
        while (getline(file, data))
        {
            if (data == "")
                continue;
            std::string token = "";
            std::istringstream iss(data);
            if (keys.empty())
            {
                while (getline(iss, token, ','))
                {
                    convertStringToUpperCaps(token);
                    keys.push_back(token);
                }
            }
            else {
                while (getline(iss, token, ','))
                    values.push_back(token);

                size_t pos = loopingAndFindKey(keys, "SOUNDTYPE");
                if (values[pos] != "")
                    theMusic = new MusicEntity2D();
                if (theMusic) {
                    pos = loopingAndFindKey(keys, "SOUNDFILENAME");
                    theMusic->setISoundSouce(values[pos]);

                    pos = loopingAndFindKey(keys, "VOLUME");
                    theMusic->SetVolume(stof(values[pos]));

                    pos = loopingAndFindKey(keys, "TIMESTOPLAY");
                    theMusic->SetNumTimeToPlay(stoi(values[pos]));

                    pos = loopingAndFindKey(keys, "UNLIMITEDTIMES");
                    if (values[pos] != "")
                        theMusic->SetUnlimitedPlayTimes(true);

                    pos = loopingAndFindKey(keys, "LOOP");
                    if (values[pos] != "")
                        theMusic->SetConstantLooping(true);

                    pos = loopingAndFindKey(keys, "SOUNDID");
                    theMusic->setName(values[pos]);

                    all_the_Music.insert(std::pair<std::string, MusicEntity2D*>(theMusic->getName(), theMusic));
                }
                theMusic = nullptr;
                values.clear();
            }
        }

        file.close();
        return true;
    }
    return false;
}

bool MusicSystem::playBackgroundMusic(const std::string &songName)
{
    strMEmap::iterator it = all_the_Music.find(songName);
    if (it != all_the_Music.end())
    {
        if (theOnlyBackgroundMusic && theOnlyBackgroundMusic->getName() != it->second->getName())
        {
            theOnlyBackgroundMusic->Stop(1);
            theOnlyBackgroundMusic = nullptr;
        }
        if (theOnlyBackgroundMusic == nullptr)
        {
            theOnlyBackgroundMusic = it->second;
            theOnlyBackgroundMusic->Play();
        }
        return true;
    }
    return false;
}

bool MusicSystem::playMusic(const std::string &songName)
{
    strMEmap::iterator it = all_the_Music.find(songName);
    if (it != all_the_Music.end())
    {
        it->second->Play();
        return true;
    }
    return false;
}

void MusicSystem::clearEverything()
{
    for (auto it : all_the_Music)
    {
        delete it.second;
    }
    all_the_Music.clear();
    theOnlyBackgroundMusic = nullptr;
    musicEngine->drop();
}

size_t MusicSystem::loopingAndFindKey(std::vector<std::string> &theKeys, const std::string &whatyouwant)
{
    size_t thePosOfVec = 0;
    for (std::vector<std::string>::iterator it = theKeys.begin(), end = theKeys.end(); it != end; ++it)
    {
        if ((*it).find(whatyouwant) != std::string::npos)
        {
            break;
        }
        else
            ++thePosOfVec;
    }
    return thePosOfVec;
}

void MusicSystem::setPlayerPos(Vector3 &pos)
{
    playerPos = &pos;
}

void MusicSystem::setTimeToUpdate(const double &dt)
{
    TimeUpdate = dt;
}

MusicEntity2D *MusicSystem::accessTheMusic(const std::string &songName)
{
    strMEmap::iterator it = all_the_Music.find(songName);
    if (it != all_the_Music.end())
    {
        return it->second;
    }
    return nullptr;
}