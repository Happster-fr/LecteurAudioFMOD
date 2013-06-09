/* 
 * File:   audioPlayer.cpp
 * Author: user
 * 
 * Created on 8 juin 2013, 19:38
 */

#include "audioPlayer.h"

audioPlayer::audioPlayer() {
    channel = 0;
}

audioPlayer::audioPlayer(const audioPlayer& orig) {
    channel = 0;
}

audioPlayer::~audioPlayer() {}

int audioPlayer::playSound(string songPath, string songName, bool withBack) {
    int key;
    bool endOfTrack = false;
    /*
        Create a System object and initialize.
     */
    result = FMOD::System_Create(&system);
    utilities::ERRCHECK(result);

    result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    utilities::ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    utilities::ERRCHECK(result);

    /* INIT SONG */
    result = system->createSound(songPath.c_str(), FMOD_SOFTWARE, 0, &sound1);
    utilities::ERRCHECK(result);

    /* PLAY SONG */
    result = system->playSound(FMOD_CHANNEL_FREE, sound1, 0, &channel);
    utilities::ERRCHECK(result);

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
                case ' ': {
                    playPause();
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
                    utilities::ERRCHECK(result);
                }

                result = channel->getPaused(&paused);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    utilities::ERRCHECK(result);
                }

                result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    utilities::ERRCHECK(result);
                }

                channel->getCurrentSound(&currentsound);
                if (currentsound) {
                    result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
                    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                        utilities::ERRCHECK(result);
                    }
                }
            }

            printf("%02d:%02d:%02d/%02d:%02d:%02d - (%s)\r", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Pause " : playing ? "Play" : "Stop");
            fflush(stdout);
            
        }
    } while (key != 27);

    printf("\n");
    release();
    return 0;
}

int audioPlayer::choixMusique(string folderPath, vector<string> filesName) {
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

void audioPlayer::release() {
    result = sound1->release();
    utilities::ERRCHECK(result);
    result = system->close();
    utilities::ERRCHECK(result);
    result = system->release();
    utilities::ERRCHECK(result);
}

void audioPlayer::playPause() {
    bool paused;
    channel->getPaused(&paused);
    channel->setPaused(!paused);
}
