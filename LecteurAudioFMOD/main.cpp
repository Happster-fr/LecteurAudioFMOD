#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <dirent.h>
#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include "wincompat.h"
#include "playlistHelper.h"
#include "audioPlayer.h"


using namespace std;

int main(int argc, char *argv[]) {

    if (argc > 1) {
        audioPlayer* ap;
        ap = new audioPlayer();
        
        string songPath = argv[1];
        vector<string> fileSplit = utilities::split(songPath, '/');
        string fileName = fileSplit.at(fileSplit.size() - 1);
        fileSplit = utilities::split(fileName, '.');
        if (fileSplit.size() > 1) {
            if (utilities::fileIsAudio(fileName)) {
                return ap->playSound(songPath, fileName);
            } else {
                printf("Désolé mais le fichier n'est pas un audio!!\n");
            }
        } else {
            vector<string> filesName = utilities::listFilesInFolder(songPath);
            if (filesName.size() > 0) {
                printf("===================================================================\n");
                printf("Choissisez votre musique.\n");
                printf("===================================================================\n");
                ap->choixMusique(songPath, filesName);
            } else {
                printf("Aucun fichier audio n'est présent dans ce dossier\n");
            }
        }
        delete ap;
        ap = NULL;
    }

    return 0;
}
