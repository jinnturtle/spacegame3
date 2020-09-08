#include "helpers.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>

#include "logs.hpp"

auto check_collision(const Rect* a, const Point2* b) -> bool
{
    return !(
        b->x > a->x + a->w
        || b->x < a->x
        || b->y > a->y + a->h
        || b->y < a->y);
}

auto check_collision(int x, int y, const Rect* b) -> bool
{
    return !(
        x > b->x + b->w
        || x < b->x
        || y > b->y + b->h
        || y < b->y);
}

auto load_surface(const std::string& path) -> SDL_Surface*
{
    DBG(10, "loading surface from '", path, "'");

    SDL_Surface* sfc {IMG_Load(path.c_str())};
    if (sfc == nullptr) {
        // TODO - make a variadic logs::err
        // logs::err(cru::logs::ERR_IMG, "could not load surface from ", path);
        logs::err(logs::ERR_img, "could not load surface from " + path);
        return nullptr;
    }

    return sfc;
}

auto load_texture(const std::string& path, SDL_Renderer* ren) -> SDL_Texture*
{
    DBG(2, "loading texture from '", path, "'");

    SDL_Surface* sfc {load_surface(path)};
    if (sfc == nullptr) { return nullptr; }

    SDL_Texture* tex {SDL_CreateTextureFromSurface(ren, sfc)};
    SDL_FreeSurface(sfc);
    sfc = nullptr;

    if (tex == nullptr) {
        // TODO - make and use a variadic logs::err
        logs::err(logs::ERR_sdl, "could not create texture form surface "+path);

        return nullptr;
    }

    return tex;
}

auto rasterize_txt(
    const std::string& txt, int size,
    SDL_Color fg, SDL_Color bg,
    const std::string& fpath, SDL_Renderer* ren,
    Txt_raster_type ras_t) -> SDL_Texture*
{
    DBG(9, "rasterizing text: '", txt, "'");

    TTF_Font* font {TTF_OpenFont(fpath.c_str(), size)};
    if (font == nullptr) {
        logs::err(logs::ERR_ttf, "could not open font " + fpath);
        return nullptr;
    }

    SDL_Surface* sfc {nullptr};
    switch (ras_t) {
        case Raster_solid:
            sfc = TTF_RenderText_Solid(font, txt.c_str(), fg);
            break;
        case Raster_shaded:
            sfc = TTF_RenderText_Shaded(font, txt.c_str(), fg, bg);
            break;
        case Raster_blended:
            sfc = TTF_RenderText_Blended(font, txt.c_str(), fg);
            break;
        default:
            logs::err(logs::ERR_gen, "invlid text raster method selected");
            return nullptr;
    }

    TTF_CloseFont(font);
    font = nullptr;
    if (sfc == nullptr) {
        logs::err(logs::ERR_ttf, "could not render text onto surface");

        return nullptr;
    }

    SDL_Texture* tex {SDL_CreateTextureFromSurface(ren, sfc)};
    SDL_FreeSurface(sfc);
    sfc = nullptr;
    if (tex == nullptr) {
        logs::err(logs::ERR_sdl,
                  "(rasterize) could not create texture from surface");

        return nullptr;
    }

    return tex;
}
