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
        ERR_sdl = 0, // SDL errors
        ERR_img, // SDL Img errors
        ERR_ttf, // SDL TTF errors
    };

    // print specialised error message
    auto errt(Err_type err, const std::string& txt) -> void;

    // log into standard output (stdout, should be configurable in the future)
    template<typename... Ts>
    auto std(Ts... args) -> void
    {
        std::stringstream buf;
        buf << timestamp_nano() << " ";
        (buf << ... << args);
        std::cout << buf.str() << std::endl;
    }

    // log into error output (stderr, should be configurable in the future)
    template<typename... Ts>
    auto err(Ts... args) -> void
    {
        std::stringstream buf;
        buf << timestamp_nano() << " ";
        (buf << ... << args);
        std::cerr << buf.str() << std::endl;
    }
} // namespace logs

#ifndef LOGS_SRC_TRACE_STAMP
    #define LOGS_SRC_TRACE_STAMP __FILE__, ":", __LINE__
#else
    #error LOGS_SRC_TRACE_STAMP already defined
#endif

#ifndef ERRLOG
    #define ERRLOG(...) logs::err("ERROR [", LOGS_SRC_TRACE_STAMP, "] ",\
    __VA_ARGS__);
#else
    #error ERRLOG already defined
#endif

#ifdef DEBUG
    #define DBG(verbocity, ...) if ((DEBUG) >= (verbocity) || (DEBUG) == -1) {\
    logs::std("DBG(", verbocity, ") [", LOGS_SRC_TRACE_STAMP, "] ",\
    __VA_ARGS__);\
}

#else
#   define DBG(...) // nothing here if not a debug build
#endif // DEBUG

#endif // SRC_LOGS_HPP_
