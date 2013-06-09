#ifndef AUDIOMULTIPLAYER_H
#define	AUDIOMULTIPLAYER_H

#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include "wincompat.h"
#include "utilities.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <dirent.h>

using namespace std;

class audioMultiPlayer {
public:
    audioMultiPlayer();
    audioMultiPlayer(const audioMultiPlayer& orig);
    virtual ~audioMultiPlayer();
    int playSound(string songPath, string songName, bool firstStart, pthread_mutex_t* mut);
    bool getPaused();
    void playPause();
    void release();
    void fadIn();
    void fadOut();
private:
    FMOD::System *system;
    FMOD::Sound *sound1;
    FMOD::Channel *channel;
    FMOD_RESULT result;
    bool flagFadIn;
    bool flagFadOut;

};

#endif	/* AUDIOMULTIPLAYER_H */


