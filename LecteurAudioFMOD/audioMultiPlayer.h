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
    /**
     * Play an audio file
     * @param songPath : to retrieve audio file
     * @param songName : displayed in the console this audio name
     * @param firstStart : si true play the audio else pause and put the volume down
     * @param mut : can unlock mutex to unlock the main thread
     * @return int : return 0 if alreasing is ok
     */
    int playSound(string songPath, string songName, bool firstStart, pthread_mutex_t* mut);
    /**
     * Return true if the song is paused, else false
     * @return bool
     */
    bool getPaused();
    /**
     * Play/Pause song
     */
    void playPause();
    /**
     * Release sound and system
     */
    void release();
    /**
     * Change flagFadIn = true for trigger FadIn effect in playsong method
     */
    void fadIn();
    /**
     * Change flagFadOut = true for trigger FadOut effect in playsong method
     */
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


