#include <sstream>
#include <string>
#include <strings.h>
#include <stdint.h>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string.h>

#ifndef APP_UTILS
#define APP_UTILS

//Universal Event Types
const int OBJ_CRT = 0;
const int OBJ_UPD = 1;
const int OBJ_GET = 2;
const int OBJ_DEL = 3;
const int KILL = 999;
const int PING = 555;

//Error Response Codes
const int NO_ERROR = 100;
const int ERROR = 101;
const int NOT_FOUND = 102;
const int TRANSLATION_ERROR = 110;
const int PROCESSING_ERROR = 120;
const int BAD_MSG_TYPE_ERROR = 121;
const int INSUFF_DATA_ERROR = 122;

//Trim String methods

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

#endif
