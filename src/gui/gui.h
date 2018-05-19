#ifndef GUI_H_
#define GUI_H_

#define DEFAULT_WINW 640
#define DEFAULT_WINH 480

typedef void *GUI_Window;
extern GUI_Window VIDEO_WINDOW;

extern int gui_init(void);
extern int gui_handle_events(void);

#endif
