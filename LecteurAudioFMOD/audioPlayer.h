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
    /**
     * Play an audio file
     * @param songPath : to retrieve audio file
     * @param songName : displayed in the console this audio name
     * @param withBack : if true print 'return' to the console, else print 'quit'
     * @return int : return 0 if alreasing is ok
     */
    int playSound(string songPath, string songName, bool withBack = false);
    /**
     * Print all positons files in folder and start song with its position
     * @param folderPath : to retrieve folder file
     * @param filesName : list of all files in the folder
     * @return int : return 0 if alreasing is ok
     */
    int choixMusique(string folderPath, vector<string> filesName);
    /**
     * Play/Pause song
     */
    void playPause();
    /**
     * Release sound and system
     */
    void release();
private:
    FMOD::System *system;
    FMOD::Sound *sound1;
    FMOD::Channel *channel;
    FMOD_RESULT result;

};

#endif	/* AUDIOPLAYER_H */

