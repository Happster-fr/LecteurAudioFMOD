/* 
 * File:   utilities.h
 * Author: user
 *
 * Created on 8 juin 2013, 15:42
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include "wincompat.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <dirent.h>

using namespace std;

class utilities {
public:
    utilities();
    utilities(const utilities& orig);
    virtual ~utilities();
    /**
     * Print error in console
     * @param err
     */
    static void errorLogger(string err);
    /**
     * Check if FMOD_RESULT is ok, else Exit
     * @param result
     */
    static void ERRCHECK(FMOD_RESULT result);
    /**
     * Return true if the version is smaller FMOD_VERSION, else false
     * @param version
     * @return int
     */
    static int check_version(unsigned int version);
    /**
     * Return true if file exist
     * @param filename
     * @return bool
     */
    static bool fileExist(string filename);
    /**
     * Cutting a string with separator and result in string vector var and return result
     * @param s
     * @param delim
     * @param elems
     * @return vector<string>
     */
    static vector<string> split2(const string &s, char delim, vector<string> &elems);
    /**
     * Return the path of the file without its name
     * @param s
     * @return string
     */
    static string onlyPath(const string &s);
    /**
     * Cutting a string with separator and result in string vector var and return result
     * @param s
     * @param delim
     * @param elems
     * @return vector<string>
     */
    static vector<string> esplit(const string &s, char delim, vector<string> &elems);
    /**
     * Cutting astring with separator and return result in string vector
     * @param s
     * @param delim
     * @return vector<string>
     */
    static vector<string> split(const string &s, char delim);
    /**
     * Retrun true if file have a .mp3, .ogg or .wav extension
     * @param fileName
     * @return bool
     */
    static bool fileIsAudio(string fileName);
    /**
     * List all audio file in folder (if file have a .mp3, .ogg or .wav extension)
     * @param folder
     * @return vector<string>
     */
    static vector<string> listFilesInFolder(string folder);
    /**
     * Return true if file have a .m3u extension, else false
     * @param fileName
     * @return bool
     */
    static bool fileIsPlaylist(string fileName);
private:

};

#endif	/* UTILITIES_H */

