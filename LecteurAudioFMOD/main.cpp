#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <dirent.h>
#include <signal.h>
#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include "wincompat.h"
#include "playlistHelper.h"
#include "audioPlayer.h"
#include "audioMultiPlayer.h"

using namespace std;

/** Single Song **/

struct SongStruct {
    audioPlayer* ap;
    string songPath;
    string songName;
    bool withBack;
};

void *playSongThread(void* arg) {
    SongStruct *songStruct = (SongStruct*) arg;
    audioPlayer* ap = songStruct->ap;
    ap->playSound(songStruct->songPath, songStruct->songName, songStruct->withBack);
    return NULL;
}

/** Multi Song **/

struct MultiSongStruct {
    audioMultiPlayer* ap;
    string songPath;
    string songName;
    bool firstStart;
};

void *playMultiSongThread(void* arg) {
    MultiSongStruct *multiSongStruct = (MultiSongStruct*) arg;
    audioMultiPlayer* ap = multiSongStruct->ap;
    ap->playSound(multiSongStruct->songPath, multiSongStruct->songName, multiSongStruct->firstStart);
    return NULL;
}


/** Ctrl+C Signal **/

audioMultiPlayer* ap1;
audioMultiPlayer* ap2;
bool firstCase = true;
bool flagFadIn = false;
int position = 0;

void handler(int signum) {
    if (signum == SIGINT) {
        exit(EXIT_SUCCESS);
    } else if (signum == SIGALRM) {
        if (position != 0) {
            switch (position) {
                case 1:
                {
                    if (!ap2->getPaused()) {
                        position = 0;
                    }
                    break;
                }
                case 2:
                {
                    if (!ap1->getPaused()) {
                        position = 0;
                    }
                    break;
                }
            }
        } else {
            if (firstCase) {
                if (!flagFadIn && !ap1->getPaused()) {
                    ap1->fadOut();
                    flagFadIn = true;
                } else if (flagFadIn && ap2->getPaused()) {
                    ap2->playPause();
                    ap2->fadIn();
                    flagFadIn = false;
                    firstCase = false;
                    position = 1;
                }
            } else {
                if (!flagFadIn && !ap2->getPaused()) {
                    ap2->fadOut();
                    flagFadIn = true;
                } else if (flagFadIn && ap1->getPaused()) {
                    ap1->playPause();
                    ap1->fadIn();
                    flagFadIn = false;
                    firstCase = true;
                    position = 2;
                }
            }
        }
    }
}

int playWithThread(string songPath, string fileName, audioPlayer* ap) {
    SongStruct* songStruct = new SongStruct();
    songStruct->songPath = songPath;
    songStruct->songName = fileName;
    songStruct->withBack = false;
    songStruct->ap = ap;

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, playSongThread, songStruct);

    int key;
    do {
        if (kbhit()) {
            key = getch();
            switch (key) {
                case ' ':
                {
                    ap->playPause();
                    break;
                }
            }
        }
    } while (key != 27);
    return 0;
}

int managePlaylist(string songPath) {
    playlistHelper* ph = new playlistHelper();
    if (utilities::fileExist(songPath)) {
        ph->loadPlaylist(songPath);
    } else {
        string path, playlistName, trackName;
        printf("===================================================================\n");
        printf("Welcome to Playlist creator\n");
        printf("===================================================================\n");
        printf("Please enter the path of the playlist (music files need to be on same folder):\n");
        cin >> path;
        printf("Please enter the name of the playlist:\n");
        cin >> playlistName;

        ph->createPlaylist(playlistName, path);
        printf("What is the first track?\n");
        cin >> trackName;
        ph->addTrack(trackName, path, playlistName);

        bool playlist = true;
        while (playlist) {
            printf("What is the next track?\n");
            cin >> trackName;
            ph->addTrack(trackName, path, playlistName);
            printf("Another track (Y/n)?\n");
            cin >> trackName;
            if (trackName == "n") {
                playlist = false;
            }
        }
        printf("\nEnjoy the new playlist!\n");
        ph->loadPlaylist(path + playlistName);
        delete ph; 
        ph = NULL;
    }
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        audioPlayer* ap = new audioPlayer();
        string songPath = argv[1];
        vector<string> fileSplit = utilities::split(songPath, '/');
        string fileName = fileSplit.at(fileSplit.size() - 1);
        fileSplit = utilities::split(fileName, '.');
        if (fileSplit.size() > 1) {
            if (utilities::fileIsAudio(fileName)) {
                playWithThread(songPath, fileName, ap);
            } else if (utilities::fileIsPlaylist(fileName)) {
                managePlaylist(songPath);
            } else {
                utilities::errorLogger("Désolé mais le type de fichier n'est pas pris en charge...\n");
            }
        } else {
            vector<string> filesName = utilities::listFilesInFolder(songPath);
            if (filesName.size() > 0) {
                printf("===================================================================\n");
                printf("Choissisez votre musique.\n");
                printf("===================================================================\n");
                ap->choixMusique(songPath, filesName);
            } else {
                utilities::errorLogger("Aucun fichier audio n'est présent dans ce dossier\n");
            }
        }
        delete ap;
        ap = NULL;
    } else if (argc == 3) {
        ap1 = new audioMultiPlayer();
        ap2 = new audioMultiPlayer();
        string songPath1 = argv[1];
        string songPath2 = argv[2];
        vector<string> fileSplit1 = utilities::split(songPath1, '/');
        vector<string> fileSplit2 = utilities::split(songPath2, '/');
        string fileName1 = fileSplit1.at(fileSplit1.size() - 1);
        string fileName2 = fileSplit2.at(fileSplit2.size() - 1);
        fileSplit1 = utilities::split(fileName1, '.');
        fileSplit2 = utilities::split(fileName2, '.');
        if (fileSplit1.size() > 1 && fileSplit2.size() > 1 && utilities::fileIsAudio(fileName1) && utilities::fileIsAudio(fileName2)) {
            MultiSongStruct* multiSongStruct1 = new MultiSongStruct();
            multiSongStruct1->songPath = songPath1;
            multiSongStruct1->songName = fileName1;
            multiSongStruct1->ap = ap1;
            multiSongStruct1->firstStart = true;

            MultiSongStruct* multiSongStruct2 = new MultiSongStruct();
            multiSongStruct2->songPath = songPath2;
            multiSongStruct2->songName = fileName2;
            multiSongStruct2->ap = ap2;
            multiSongStruct2->firstStart = false;

            pthread_t thread_id1;
            pthread_create(&thread_id1, NULL, playMultiSongThread, multiSongStruct1);

            pthread_t thread_id2;
            pthread_create(&thread_id2, NULL, playMultiSongThread, multiSongStruct2);

            signal(SIGALRM, handler);

            int key;
            do {
                if (kbhit()) {
                    key = getch();
                    switch (key) {
                        case ' ':
                        {
                            alarm(1);
                            break;
                        }
                    }
                }
            } while (key != 27);
        }
    }
    return 0;
}
