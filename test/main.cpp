#include "nuklear_sdl3.h"
#include "nuklear_blend2d.h"
#include <iostream>
using std::cerr;

int width = 800;
int height = 600;

int main()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        cerr << "ERROR: Failed to initialize SDL3\n" << SDL_GetError() << "\n";
    }

    SDL_Window* window = SDL_CreateWindow("NuklearPort test", width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY| SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    BLImage img(width, height, BL_FORMAT_PRGB32);
    BLContext bctx(img);

    BLFontFace face;
    BLFont font;
    // Load the default Segoe UI font on Windows
    // You must change it on other platforms
    face.createFromFile("C:/Windows/Fonts/segoeui.ttf");
    font.createFromFace(face, 14);

    struct nk_context ctx = nk_blend2d_init(font);

    enum {EASY, HARD};
    static int op = EASY;
    static float value = 0.6f;
    static int i =  20;

    SDL_Event event;
    bool running = true;
    while(running)
    {
        nk_input_begin(&ctx);
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
                running = false;
            if(event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                width = event.window.data1;
                height = event.window.data2;

                SDL_DestroyTexture(texture);
                texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            }
            nk_sdl3_handle_events(&ctx, event);
        }
        nk_input_end(&ctx);

        nk_clear(&ctx);

        if(nk_begin(&ctx, "Sample window",  nk_rect(50, 50, 220, 220),
                    NK_WINDOW_SCALABLE|NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
        {
            // fixed widget pixel width
            nk_layout_row_static(&ctx, 30, 80, 1);
            if (nk_button_label(&ctx, "button")) {
                // event handling
            }

            // fixed widget window ratio width
            nk_layout_row_dynamic(&ctx, 30, 2);
            if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;

            // custom widget pixel width
            nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(&ctx, 50);
                nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
                nk_layout_row_push(&ctx, 110);
                nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
            }
            nk_layout_row_end(&ctx);
            nk_end(&ctx);
        }

        // Render everything
        {
            nk_blend2d_render(&ctx, bctx);
            BLImageData data;
            img.getData(&data);

            SDL_Rect srcRect = {0, 0, data.size.w, data.size.h};
            SDL_UpdateTexture(texture, &srcRect, data.pixelData, srcRect.w * 4);

            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }
             
    }

    nk_free(&ctx);
    SDL_Quit();
    return 0;
}
