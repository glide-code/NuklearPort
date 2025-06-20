// --------------------------------------------------------------------------------
// SDL3 window backend for Nuklear
// by GlideCode (https://github.com/glide-code)
//
// Useful links:
// Nuklear : https://github.com/Immediate-Mode-UI/Nuklear
// SDL3 : https://github.com/libsdl-org/sdl
//
// Version: 1.0.0
// - SDL3 implementation of default SDL2 backend provided with Nuklear
// - No support for window resizing. You need to handle it manually :)
// --------------------------------------------------------------------------------

#ifndef NUKLEAR_SDL3_H
#define NUKLEAR_SDL3_H

#include <SDL3/SDL.h>

#ifndef NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FIXED_TYPES
#endif // NK_INCLUDE_FIXED_TYPES
#include "nuklear.h"

// --------------------------------------------------------------------------------
// Handle common window events emitted by SDL3
//
// Parameters:
// ctx : pointer to an nk_context struct to add events
// event : reference to an SDL_Event object that holds event data from SDL3
//
// Return:
// It doesn't return anything. It's obvious from the 'void' type, isn't it? :)
// --------------------------------------------------------------------------------

NK_API void nk_sdl3_handle_events(struct nk_context* ctx, SDL_Event& event);

#endif // NUKLEAR_SDL3_H
