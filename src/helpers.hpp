/*******************************************************************************
 * this file is meant to keep general helper functions, structures, etc
 * helpers that can be otherwise categorised should go to a file for that
 * category to avoid having one huge file where things are difficult to find
 ******************************************************************************/

#ifndef SRC_HELPERS_HPP_
#define SRC_HELPERS_HPP_

#include <SDL2/SDL.h> // only used by App_Environment?

#include <string>

enum Txt_raster_type {
    Raster_solid = 0,
    Raster_shaded,
    Raster_blended,
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Coord2 {
    int x;
    int y;
};

struct Coord2d {
    double x;
    double y;
};

struct Size2 {
    int w;
    int h;
};

auto load_surface(const std::string& path) -> SDL_Surface*;
auto load_texture(const std::string& path, SDL_Renderer* ren) -> SDL_Texture*;
auto rasterize_txt(
    const std::string& txt, int size,
    SDL_Color fg, SDL_Color bg,
    const std::string& fpath, SDL_Renderer* ren,
    Txt_raster_type ras_t) -> SDL_Texture*;

// this should go into something like physics.hpp or collisions.hpp etc.
auto check_collision(const Rect* a, const Coord2* b) -> bool;

// this should go into something like physics.hpp or collisions.hpp etc.
// check for colission between point x/y and a 2d box
auto check_collision(int x, int y, const Rect* b) -> bool;

#endif // SRC_HELPERS_HPP_
