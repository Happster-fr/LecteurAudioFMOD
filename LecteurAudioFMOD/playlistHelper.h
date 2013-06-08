#ifndef PLAYLISTHELPER_H
#define	PLAYLISTHELPER_H

#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include "wincompat.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <dirent.h>
#include <fstream>

#include "utilities.h"

using namespace std;

class playlistHelper {
public:
    playlistHelper();
    playlistHelper(const playlistHelper& orig);
    virtual ~playlistHelper();
    int loadPlaylist(string path);
    int addTrack(string trackName, string playlistPath, string playlistName);
    int createPlaylist(string playlistName, string playlistPath);
private:
    void instructions();
    int preparePlaylist();
    int launchPlaylist();
    int initPlaylist();
        
    string pathName;
    FMOD::System *system;
    FMOD::Sound *playlist;
    FMOD::Sound *sound;
    FMOD::Channel *channel;
    FMOD_TAG tag;
    FMOD_RESULT result;
    FMOD_SOUND_TYPE soundtype;
    bool isplaylist;
    char *title;
    int trackNumber;
    int key;
    unsigned int version;
    char file[128];
    string playlistPath;
};

#endif	/* PLAYLISTHELPER_H */

