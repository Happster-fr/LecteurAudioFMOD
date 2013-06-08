#include "playlistHelper.h"

playlistHelper::playlistHelper() {
}

playlistHelper::playlistHelper(const playlistHelper& orig) {
}

playlistHelper::~playlistHelper() {
}

void playlistHelper::instructions() {
    printf("===================================================================\n");
    printf("Playlist Mode. \n");
    printf("===================================================================\n");
    printf("\n");
    printf("Press 'n'     to play next sound in playlist\n");
    printf("Press 'space' to pause/unpause current sound\n");
    printf("Press 'Esc'   to quit\n");
    printf("\n");
}

int playlistHelper::preparePlaylist() {
    /*
        Create a System object and initialize
     */

    result = FMOD::System_Create(&system);
    utilities::ERRCHECK(result);

    result = system->getVersion(&version);
    utilities::ERRCHECK(result);

    utilities::check_version(version);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    utilities::ERRCHECK(result);

    result = system->createSound(playlistPath.c_str(), FMOD_DEFAULT, 0, &playlist);
    utilities::ERRCHECK(result);

    result = playlist->getFormat(&soundtype, 0, 0, 0);
    utilities::ERRCHECK(result);

    isplaylist = (soundtype == FMOD_SOUND_TYPE_PLAYLIST);

    instructions();
}

int playlistHelper::initPlaylist() {
    if (isplaylist) {
        printf("PLAYLIST loaded.\n");
        /*
        Get the first song in the playlist, create the sound and then play it.
         */
        result = playlist->getTag("FILE", trackNumber, &tag);
        utilities::ERRCHECK(result);

        pathName = utilities::onlyPath(playlistPath);
        sprintf(file, (pathName + "%s").c_str(), (char *) tag.data);
        result = system->createSound(file, FMOD_DEFAULT, 0, &sound);
        utilities::ERRCHECK(result);

        result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
        utilities::ERRCHECK(result);

        playlist->getTag("TITLE", trackNumber, &tag);
        title = (char *) tag.data;

        trackNumber++;
    } else {
        //simple sound...
    }

    printf("\n");
}

int playlistHelper::launchPlaylist() {
    /*
    Main loop.
     */
    do {
        bool isplaying = false;

        if (channel && isplaylist) {
            /*
                When sound has finished playing, play the next sound in the playlist
             */

            channel->isPlaying(&isplaying);
            if (!isplaying) {
                if (sound) {
                    sound->release();
                    sound = NULL;
                }

                result = playlist->getTag("FILE", trackNumber, &tag);
                if (result != FMOD_OK) {
                    trackNumber = 0;
                } else {
                    printf("playing next song in playlist...\n");

                    sprintf(file, (pathName + "%s").c_str(), (char *) tag.data);

                    result = system->createSound(file, FMOD_DEFAULT, 0, &sound);
                    utilities::ERRCHECK(result);

                    result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
                    utilities::ERRCHECK(result);

                    playlist->getTag("TITLE", trackNumber, &tag);
                    title = (char *) tag.data;

                    trackNumber++;
                }
            }
        }


        if (kbhit()) {
            key = getch();

            switch (key) {
                case 'n':
                {
                    /*
                        Play the next song in the playlist
                     */
                    if (channel && isplaylist) {
                        channel->stop();
                    }

                    break;
                }
                case ' ':
                {
                    if (channel) {
                        bool paused;

                        channel->getPaused(&paused);
                        channel->setPaused(!paused);
                    }
                }
            }
        }

        system->update();

        {
            unsigned int ms = 0;
            unsigned int lenms = 0;
            bool paused = 0;

            if (channel) {
                if (sound) {
                    result = sound->getLength(&lenms, FMOD_TIMEUNIT_MS);
                    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                        utilities::ERRCHECK(result);
                    }
                }

                result = channel->getPaused(&paused);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    utilities::ERRCHECK(result);
                }

                result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
                    utilities::ERRCHECK(result);
                }
            }

            printf("Time %02d:%02d:%02d/%02d:%02d:%02d : %s : %s\r", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Paused " : "Playing ", title);
            fflush(stdout);
        }


    } while (key != 27);
}

int playlistHelper::loadPlaylist(string path) {
    if (path.length() > 0) {
        if (utilities::fileExist(path)) {
            playlistPath = path;
            preparePlaylist();
            initPlaylist();
            launchPlaylist();
        } else {
            utilities::errorLogger("the m3u file doesn't exist");
        }
    } else {
        utilities::errorLogger("the path is empty");
    }
}

int playlistHelper::addTrack(string trackName, string playlistPath, string playlistName) {
    int resultint = 0;
    if (utilities::fileExist(playlistPath + trackName)) {
        ofstream myfile;
        myfile.open((playlistPath + playlistName).c_str(), std::ios_base::app);
        if (myfile.is_open()) {
            myfile << "#EXTINF:1," << trackName << "\n";
            myfile << trackName << "\n";
        } else {
            utilities::errorLogger("Unable to open file");
            resultint = -1;
        }
    } else {
        utilities::errorLogger("The " + trackName + " isn't in the playlist path : " + playlistPath);
        resultint = -1;
    }
    return resultint;
}

int playlistHelper::createPlaylist(string playlistName, string playlistPath) {
    int resultint = 0;
    if (!utilities::fileExist(playlistPath + playlistName)) {
        ofstream myfile;
        myfile.open((playlistPath + playlistName).c_str());
        myfile << "#EXTM3U\n";
    } else {
        utilities::errorLogger("Playlist exists");
        resultint = -1;
    }
    return resultint;
}






