#ifndef AUDIOPLAYER_H
#define	AUDIOPLAYER_H

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

class audioPlayer {
public:
    audioPlayer();
    audioPlayer(const audioPlayer& orig);
    virtual ~audioPlayer();
    int playSound(string songPath, string songName, bool withBack = false);
    int choixMusique(string folderPath, vector<string> filesName);
private:
    FMOD::System *system;
    FMOD::Sound *sound1;
    FMOD::Channel *channel = 0;
    FMOD_RESULT result;
};

#endif	/* AUDIOPLAYER_H */

