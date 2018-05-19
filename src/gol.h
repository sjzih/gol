#ifndef GOL_H_
#define GOL_H_

typedef struct gol_state *GOL_State;

struct gol_state {
    unsigned width, height;
    uint8_t *grid;
};

#endif
