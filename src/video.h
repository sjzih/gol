#ifndef VIDEO_H_
#define VIDEO_H_

#include "gui/gui.h"
#include "gol.h"

extern Uint32 *framebuffer;

extern int  video_init(GUI_Window ctx);
extern void video_render(GOL_State state);

#endif
