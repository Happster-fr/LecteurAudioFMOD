#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <dirent.h>
#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include "wincompat.h"

using namespace std;

/** FMOD **/

void ERRCHECK(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

int playSound(string songPath, string songName, bool withBack = false) {
    FMOD::System *system;
    FMOD::Sound *sound1;
    FMOD::Channel *channel = 0;
    FMOD_RESULT result;
    int key;

    /*
        Create a System object and initialize.
     */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

    /* INIT SONG */
    result = system->createSound(songPath.c_str(), FMOD_SOFTWARE, 0, &sound1);
    ERRCHECK(result);

    /* PLAY SONG */
    result = system->playSound(FMOD_CHANNEL_FREE, sound1, 0, &channel);
    ERRCHECK(result);

    printf("===================================================================\n");
    printf("Lecteur Audio - %s\n", songName.c_str());
    printf("===================================================================\n");
    printf("\n");
    printf("'Space' -> Play/Pause\n");
    if (withBack) {
        printf("'Esc'   -> Retour\n");
    } else {
        printf("'Esc'   -> Quitter\n");
    }
    printf("\n");

    do {
        if (kbhit()) {
            key = getch();
            switch (key) {
                case ' ':
                {
                    /* PLAY/PAUSE SONG */
                    bool paused;
                    channel->getPaused(&paused);
                    channel->setPaused(!paused);
                    break;
                }
            }
        }
        system->update();

        {
            unsigned int ms = 0;
            unsigned int lenms = 0;
            bool playing = 0;
            bool paused = 0;

            if (channel) {
                FMOD::Sound *currentsound = 0;

                result = channel->isPlaying(&playing);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    ERRCHECK(result);
                }

                result = channel->getPaused(&paused);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    ERRCHECK(result);
                }

                result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    ERRCHECK(result);
                }

                channel->getCurrentSound(&currentsound);
                if (currentsound) {
                    result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
                    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                        ERRCHECK(result);
                    }
                }
            }

            printf("%02d:%02d:%02d/%02d:%02d:%02d - (%s)\r", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Pause " : playing ? "Play" : "Stop");
            fflush(stdout);
        }
    } while (key != 27);
    
    printf("\n");

    /*
        Shut down
     */
    result = sound1->release();
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

    return 0;
}

/** UTILS **/

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

bool fileIsAudio(string fileName) {
    vector<string> fileSplit = split(fileName, '.');
    if (fileSplit.size() > 1) {
        if (fileSplit.at(1) == "mp3" || fileSplit.at(1) == "ogg" || fileSplit.at(1) == "wav") {
            return true;
        }
    }
    return false;
}

vector<string> listFilesInFolder(string folder) {
    DIR *dir;
    struct dirent *ent;
    vector<string> filesName;
    if ((dir = opendir(folder.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string fileName = ent->d_name;
            if (fileIsAudio(fileName)) {
                filesName.push_back(fileName);
            }
        }
        closedir(dir);
    }
    return filesName;
}

int choixMusique(string folderPath, vector<string> filesName) {
    printf("\n");
    printf("'Autre' -> Quitter\n");
    for (int i = 0; i < filesName.size(); i++) {
        printf("'%d' -> %s\n", i + 1, filesName.at(i).c_str());
    }
    printf("\n");

    int indexSong;
    printf("Position : ");
    cin >> indexSong;

    printf("\n");
    if (indexSong > 0 && indexSong <= filesName.size()) {
        indexSong--;
        string songPath = folderPath + "/" + filesName.at(indexSong);
        playSound(songPath, filesName.at(indexSong), true);
        printf("===================================================================\n");
        printf("Choissisez votre musique.\n");
        printf("===================================================================\n");
        choixMusique(folderPath, filesName);
    } else if (indexSong > 0) {
        printf("Choix incorrecte, veuillez recommencer!!");
        printf("\n");
        choixMusique(folderPath, filesName);
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        string songPath = argv[1];

        vector<string> fileSplit = split(songPath, '/');
        string fileName = fileSplit.at(fileSplit.size() - 1);
        fileSplit = split(fileName, '.');
        if (fileSplit.size() > 1) {
            if (fileIsAudio(fileName)) {
                return playSound(songPath, fileName);
            } else {
                printf("Désolé mais le fichier n'est pas un audio!!\n");
            }
        } else {
            vector<string> filesName = listFilesInFolder(songPath);
            if (filesName.size() > 0) {
                printf("===================================================================\n");
                printf("Choissisez votre musique.\n");
                printf("===================================================================\n");
                choixMusique(songPath, filesName);
            } else {
                printf("Aucun fichier audio n'est présent dans ce dossier\n");
            }
        }
    }
    return 0;
}
