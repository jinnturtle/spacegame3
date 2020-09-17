#ifndef SRC_LOGS_HPP_
#define SRC_LOGS_HPP_

#include <iostream>
#include <sstream>
#include <string>

extern "C" {
#include "timestamp.h"
}

namespace logs {
    enum Err_type {
        ERR_gen = 0, // general errors
        ERR_sdl, // SDL errors
        ERR_img, // SDL Img errors
        ERR_ttf, // SDL TTF errors
    };

    // print error message
    void err(Err_type err, const std::string& txt);
} // namespace logs

#ifdef DEBUG
#define DBG(verbocity, ...) if ((DEBUG) >= (verbocity) || (DEBUG) == -1) {\
    logs::dbg(verbocity, "[", __FILE__, ":", __LINE__, " ", __func__, "] ",\
    __VA_ARGS__);\
}

namespace logs {
    // print debug messages up to DEBUG verbocity level
    template<typename... Ts>
    void dbg(int lvl, Ts... args) // NOLINT(misc-unused-parameters)
    {
            // buffering message before print in case there are multiple threads
            std::stringstream buf;
            buf << timestamp_nano() << " DBG(" << lvl << ")" << " ";
            (buf << ... << args);

            std::cout << buf.str() << std::endl;
    }
} // namespace logs

#else
#   define DBG(...) // nothing here if not a debug build
#endif // DEBUG

#endif // SRC_LOGS_HPP_
