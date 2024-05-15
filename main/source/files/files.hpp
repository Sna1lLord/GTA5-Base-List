#ifndef _FILES_HPP
#define _FILES_HPP

#include <common.hpp>

class File {
public:
    File(std::string path) {
        if (!CreateDirectoryA(path.c_str(), NULL))
            return;
    }
};

#endif