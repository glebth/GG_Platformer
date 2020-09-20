#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    static size_t Split(std::string &txt, std::vector<std::string> &strings, char delim) {

        size_t initPos = 0;

        size_t pos = txt.find(delim);

        strings.clear();

        while ( pos  != std::string::npos ) {
            strings.push_back( txt.substr(initPos, pos - initPos + 1) );

            initPos = pos + 1;

            pos = txt.find(delim, initPos);
        }

        //Add last one
        strings.push_back( txt.substr(initPos, std::min<int>(pos, txt.size() - initPos - 1)) );

        return strings.size();
    }

    static constexpr unsigned int str2int(const char* str, int h = 0)
    {
        return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
    }
};

#endif