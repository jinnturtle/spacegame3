#include "main_window.hpp"

#include <array>

#include <SDL2/SDL.h>

#include "FPS_manager.hpp"
#include "logs.hpp"
#include "helpers.hpp"
#include "itoa.hpp"
#include "Vessel.hpp"

auto run_main_window(App_environment* app) -> void
{
    DBG(3, "running main window");

    FPS_manager fps_man;

    Coord2d ship1_start_pos {.x = 400.0, .y = 300.0};
    Coord2d ship1_start_v {.x = 0.0, .y = 0.0};
    Vessel ship1(ship1_start_pos, ship1_start_v);
    Coord2d tgt {.x = 0.0, .y = 0.0};

    // main loop
    bool exit {false};
    bool show_fps {true};
    SDL_Event event {0};
    Coord2 mouse_pos {};
    std::array<char, 8> fps_buf {0};
    Coord2 fps_pos {.x = 0, .y = 0};
    while (!exit) {
        // input stage

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
                DBG(9, "mouse click at ", mouse_pos.x, "x", mouse_pos.y);

                tgt.x = static_cast<double>(mouse_pos.x);
                tgt.y = static_cast<double>(mouse_pos.y);
                ship1.set_nav_tgt(&tgt);
                ship1.set_nav_mode(NAV_approach);
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
                DBG(9, "mouse unclick at ", mouse_pos.x, "x", mouse_pos.y);
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_f) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {
                        fps_man.toggle_cap();
                    } else {
                        show_fps = !show_fps;
                        if (show_fps) {
                            DBG(9, "show fps counter");
                        } else {
                            DBG(9, "hide fps counter");
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_q) {
                    exit = true;
                }
            } else if (event.type == SDL_QUIT) { exit = true; }
        }

        // update stage

        ship1.update();

        // render stage

        SDL_SetRenderDrawColor(app->ren, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(app->ren);

        if (show_fps) {
            /* size - 2 so we keep the null at the end to represent text as a
             * null-terminated string */
            app->fonts[DEF_FONT_mono_fast]->render(
                // TODO - implement and use utoa (fps is unsigned)
                itoa(fps_man.get_fps(), &fps_buf[fps_buf.size()-2]),
                &fps_pos,
                app->ren);
        }

        SDL_Rect ship1_rect{
            static_cast<int>(ship1.get_pos().x),
            static_cast<int>(ship1.get_pos().y),
            16, 16};
        SDL_RenderCopy(app->ren,app->texs[DEF_TEX_ship_icon], nullptr, &ship1_rect);

        SDL_RenderPresent(app->ren);

        fps_man.end_frame();
    }

    DBG(3, "end of main window");
}
