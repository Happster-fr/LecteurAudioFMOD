/* 
 * File:   utilities.cpp
 * Author: user
 * 
 * Created on 8 juin 2013, 15:42
 */

#include "utilities.h"

utilities::utilities() {
}

utilities::utilities(const utilities& orig) {
}

utilities::~utilities() {
}

void utilities::ERRCHECK(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

int utilities::check_version(unsigned int version) {
    if (version < FMOD_VERSION) {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        exit(-1);
    }
    return 0;
}

void utilities::errorLogger(string err) {
    printf("Error!  %s.\n", err.c_str());
}

bool utilities::fileExist(string filename) {
    FILE* fp = NULL;
    fp = fopen(filename.c_str(), "rb");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}

vector<string> utilities::split2(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string utilities::onlyPath(const string &s) {
    vector<string> elems;
    split2(s, '/', elems);
    string concat = "";
    for (int i = 0; i < elems.size() - 1; i++) {
        concat += elems[i] + "/";
    }
    return concat;
}

vector<string> utilities::esplit(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> utilities::split(const string &s, char delim) {
    vector<string> elems;
    esplit(s, delim, elems);
    return elems;
}

bool utilities::fileIsAudio(string fileName) {
    vector<string> fileSplit = split(fileName, '.');
    if (fileSplit.size() > 1) {
        if (fileSplit.at(1) == "mp3" || fileSplit.at(1) == "ogg" || fileSplit.at(1) == "wav") {
            return true;
        }
    }
    return false;
}

vector<string> utilities::listFilesInFolder(string folder) {
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
