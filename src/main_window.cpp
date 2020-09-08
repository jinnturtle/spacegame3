#include "main_window.hpp"

#include <array>

#include <SDL2/SDL.h>

#include "FPS_manager.hpp"
#include "logs.hpp"
#include "helpers.hpp"
#include "itoa.hpp"

auto run_main_window(App_environment* app) -> void
{
    DBG(3, "running main window");

    FPS_manager fps_man;

    // main loop
    bool exit {false};
    bool show_fps {true};
    SDL_Event event {0};
    int mouse_x {0};
    int mouse_y {0};
    std::array<char, 8> fps_buf {0};
    Point2 fps_pos {.x = 0, .y = 0};
    while (!exit) {
        // input stage

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                DBG(9, "mouse click at ", mouse_x, "x", mouse_y);
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                DBG(9, "mouse unclick at ", mouse_x, "x", mouse_y);
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

        SDL_RenderPresent(app->ren);

        fps_man.end_frame();
    }

    DBG(3, "end of main window");
}

