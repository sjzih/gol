#include <SDL2/SDL.h>

#include "video.h"

#define TEXTURE_WIDTH 1024
#define TEXTURE_HEIGHT 768

static SDL_Window *window_g;
static SDL_Renderer *renderer_g;
static SDL_Texture *texture_g;
static Uint32 *framebuffer;

extern int
video_init(GUI_Window ctx)
{
    if (ctx == NULL)
        return 0;

    if (SDL_Init(SDL_INIT_VIDEO))
        return -1;

    window_g = SDL_CreateWindowFrom(ctx);
    if (window_g == NULL)
        return -1;

    renderer_g = SDL_CreateRenderer(window_g, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_g == NULL)
        return -1;

    texture_g = SDL_CreateTexture(renderer_g, 
                    SDL_PIXELFORMAT_RGBA4444,
                    SDL_TEXTUREACCESS_STREAMING,
                    TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (!texture_g)
        return -1;

    framebuffer = calloc(sizeof(Uint32 [TEXTURE_HEIGHT]), TEXTURE_WIDTH);
    if (!framebuffer)
        return -1;

    return 0;
}

extern void
video_render(GOL_State state)
{
    //unsigned w = state->width, h = state->height;

    SDL_UpdateTexture(texture_g, NULL, framebuffer, sizeof(Uint32 [TEXTURE_WIDTH]));
    SDL_RenderCopy(renderer_g, texture_g, NULL, NULL);
    SDL_RenderPresent(renderer_g);
}
