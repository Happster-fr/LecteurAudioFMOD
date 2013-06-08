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
    static void errorLogger(string err);
    static void ERRCHECK(FMOD_RESULT result);
    static int check_version(unsigned int version);
    static bool fileExist(string filename);
    static vector<string> split2(const string &s, char delim, vector<string> &elems);
    static string onlyPath(const string &s);
private:

};

#endif	/* UTILITIES_H */

