#include <windows.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "features.h"
#include "gui/gui.h"
#include "video.h"

#ifdef USE_ANSI
#   define ANSI_GREEN   "\033[1;32m"
#   define ANSI_BLUE    "\033[1;34m"
#   define ANSI_ORANGE  "\033[1;33m"
#   define ANSI_RED     "\033[1;31m"
#   define ANSI_RESET   "\033[0m"
#else
#   define ANSI_GREEN
#   define ANSI_BLUE
#   define ANSI_ORANGE
#   define ANSI_RED
#   define ANSI_RESET
#endif

#define NAME ANSI_GREEN "sjzih" ANSI_RESET

static const char usage_fmt[] = 
"golsim: game of life simulator, by: " NAME "\n"
"Usage:%s <options>\n"
"List of options:\n"
"\t-w <integer> Specify the width of the grid in cells  [default: 100]\n"
"\t-h <integer> Specify the height of the grid in cells [default: 100]\n"
"\t-nogui       Run without GUI                         [default: off]\n"
"\t-f <file>    Initialize grid with the contents of the given file\n"
"\t-h           Show this message\n"
"\t-n <integer> Number of simulation rounds to do\n";

#define DIE(cond, ...) do{               \
    if (cond) {\
     log_printf(LOG_ERROR, __VA_ARGS__);\
     exit(EXIT_FAILURE);\
    }\
} while(0)

struct gol_settings {
    unsigned height, width;
    bool nogui;
    const char *filename;
    unsigned niter;
};

struct gol_context {
    unsigned width, height;
};

static int gol_errno;
static char gol_errorstr[80];

struct gol_settings DEFAULT_SETTINGS = {
    .height   = 100, 
    .width    = 100,
    .nogui    = false,
    .filename = NULL,
    .niter    = 0,
};

static const char *
get_error_msg(void)
{
    if (gol_errno)
        return gol_errorstr;
    const char *msg = SDL_GetError();
    if (msg[0])
        return msg;
    if (errno)
        return strerror(errno);
    return "no error";
}

enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

static void 
draw_grid(Uint32 t)
{
    for (int i=0; i<1024; i++)
        for (int j=0; j<768; j++)
            framebuffer[i*768 + j] = (i+t) << 8;
}


static void
log_printf(int type, const char *fmt, ...)
{
    static const char LOGSTR[][32] = {
        "[" ANSI_BLUE   " info  " ANSI_RESET "] ",
        "[" ANSI_ORANGE "warning" ANSI_RESET "] ",
        "[" ANSI_RED    " error " ANSI_RESET "] ",
    };
    size_t len = strlen(fmt);
    va_list ap;

    va_start(ap, fmt);
    fputs(LOGSTR[type], stderr);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (type != LOG_INFO) {
        if (len && fmt[len-1] == ':') {
            fprintf(stderr, " %s", get_error_msg());
        }
    }
    fputc('\n', stderr);
}

#define HANDLE_ARG(shortn, longn, var, hasarg) \
    case (shortn): {                           \
        if (!strcmp(arg, longn)) {             \
            char *tmp_ = NULL;              \
            if (hasarg){                    \
                tmp_ = arg[1] ? arg+1 : (--argc, *++argv);\
                if (!tmp_)                   \
                    goto error_noarg;        \
            }                                \
            (var) = _Generic((var),          \
                        unsigned: atol(tmp_),\
                        int : atol(tmp_),    \
                        char *: tmp_,        \
                        const char *: tmp_,  \
                        bool : true          \
                    );\
            break;    \
        }\
    }\

static int
parse_args(struct gol_settings *settings, int argc, char *argv[])
{
    for (char *arg; arg = *++argv, --argc;) {
        if (*arg == '-') {
            switch(*++arg) {
            HANDLE_ARG('w',    "w", settings->width ,   true);
            HANDLE_ARG('h',    "h", settings->height,   true);
            HANDLE_ARG('n', "nogui", settings->nogui ,  false);
            HANDLE_ARG('f',    "f", settings->filename, true);
            default:
                goto error_unknownopt;
            }
        }
    }

    return 0;

error_unknownopt:
    return -1;

error_noarg:
    return -1;
}

#define FPS 60
#define FRAME_DT (1000/FPS)

static void
main_loop(GOL_State state)
{
    int running = true;
    Uint32 t0, t1, dt = 0;
    SDL_Event event;

    while (running) {
        t0 = SDL_GetTicks();
        {
            gui_handle_events();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    running = false;
            }

            draw_grid(t0);

            video_render(state);
        }
        t1 = SDL_GetTicks();

        dt = t1 - t0;
        if (dt > FRAME_DT)
            log_printf(LOG_WARNING, "video output can't keep up: %ums/frame", dt);
        if (dt < FRAME_DT)
            SDL_Delay(FRAME_DT - dt);
    }
}

static int
gol_init(GOL_State state, unsigned w, unsigned h)
{
    state->grid = calloc(w, h);
    if (!state->grid)
        return -1;
    state->width = w;
    state->height = h;
    return 0;
}

int main(int argc, char *argv[])
{
    struct gol_settings settings = DEFAULT_SETTINGS;
    struct gol_state state;

    if (parse_args(&settings, argc, argv) < 0) {
        fprintf(stderr, usage_fmt, argv[0]);
        log_printf(LOG_ERROR, "Error parsing arguments");
        return EXIT_FAILURE;
    }

    if (!settings.nogui) {
        if (gui_init() < 0) {
            log_printf(LOG_ERROR, "failed to initialize GUI");
            log_printf(LOG_INFO,  "falling back to CLI");
        }
    }

    if (video_init(VIDEO_WINDOW) < 0)
        log_printf(LOG_ERROR, "failed to initialized video:");

    gol_init(&state, settings.width, settings.height);

    main_loop(&state);
    return 0;
}
