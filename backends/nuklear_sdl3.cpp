#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#include "nuklear_sdl3.h"

NK_API void nk_sdl3_handle_events(struct nk_context* ctx, SDL_Event& event)
{
    int ctrl_down = SDL_GetModState() & (SDL_KMOD_LCTRL | SDL_KMOD_RCTRL);

    switch(event.type)
    {
        case SDL_EVENT_KEY_UP: /* KEYUP & KEYDOWN share same routine */
        case SDL_EVENT_KEY_DOWN:
        {
            int down = event.type == SDL_EVENT_KEY_DOWN;
            switch(event.key.key)
            {
                case SDLK_RSHIFT: /* RSHIFT & LSHIFT share same routine */
                case SDLK_LSHIFT:    nk_input_key(ctx, NK_KEY_SHIFT, down); break;
                case SDLK_DELETE:    nk_input_key(ctx, NK_KEY_DEL, down); break;

                case SDLK_KP_ENTER:
                case SDLK_RETURN:    nk_input_key(ctx, NK_KEY_ENTER, down); break;

                case SDLK_TAB:       nk_input_key(ctx, NK_KEY_TAB, down); break;
                case SDLK_BACKSPACE: nk_input_key(ctx, NK_KEY_BACKSPACE, down); break;
                case SDLK_HOME:      nk_input_key(ctx, NK_KEY_TEXT_START, down);
                                     nk_input_key(ctx, NK_KEY_SCROLL_START, down); break;
                case SDLK_END:       nk_input_key(ctx, NK_KEY_TEXT_END, down);
                                     nk_input_key(ctx, NK_KEY_SCROLL_END, down); break;
                case SDLK_PAGEDOWN:  nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down); break;
                case SDLK_PAGEUP:    nk_input_key(ctx, NK_KEY_SCROLL_UP, down); break;
                case SDLK_Z:         nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && ctrl_down); break;
                case SDLK_R:         nk_input_key(ctx, NK_KEY_TEXT_REDO, down && ctrl_down); break;
                case SDLK_C:         nk_input_key(ctx, NK_KEY_COPY, down && ctrl_down); break;
                case SDLK_V:         nk_input_key(ctx, NK_KEY_PASTE, down && ctrl_down); break;
                case SDLK_X:         nk_input_key(ctx, NK_KEY_CUT, down && ctrl_down); break;
                case SDLK_B:         nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && ctrl_down); break;
                case SDLK_E:         nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && ctrl_down); break;
                case SDLK_UP:        nk_input_key(ctx, NK_KEY_UP, down); break;
                case SDLK_DOWN:      nk_input_key(ctx, NK_KEY_DOWN, down); break;
                case SDLK_A:
                    if (ctrl_down)
                        nk_input_key(ctx,NK_KEY_TEXT_SELECT_ALL, down);
                    break;
                case SDLK_LEFT:
                    if (ctrl_down)
                        nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
                    else nk_input_key(ctx, NK_KEY_LEFT, down);
                    break;
                case SDLK_RIGHT:
                    if (ctrl_down)
                        nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
                    else nk_input_key(ctx, NK_KEY_RIGHT, down);
                    break;
            }
        }

        case SDL_EVENT_MOUSE_BUTTON_UP: /* MOUSEBUTTONUP & MOUSEBUTTONDOWN share same routine */
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                int down = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
                const int x = event.button.x, y = event.button.y;
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        if (event.button.clicks > 1)
                            nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
                        nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down); break;
                    case SDL_BUTTON_MIDDLE: nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down); break;
                    case SDL_BUTTON_RIGHT:  nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down); break;
                }
            }

        case SDL_EVENT_MOUSE_MOTION:
            if (ctx->input.mouse.grabbed) {
                int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
                nk_input_motion(ctx, x + event.motion.xrel, y + event.motion.yrel);
            }
            else nk_input_motion(ctx, event.motion.x, event.motion.y);

        case SDL_EVENT_TEXT_INPUT:
            {
                break;
                nk_glyph glyph;
                memcpy(glyph, event.text.text, NK_UTF_SIZE);
                nk_input_glyph(ctx, glyph);
            }

        case SDL_EVENT_MOUSE_WHEEL:
            nk_input_scroll(ctx,nk_vec2(event.wheel.x, event.wheel.x));
    }
}