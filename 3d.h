#ifndef _3D_H
#define _3D_H

#include <math.h>

#define PI atan(1) * 4

struct _point {
    float pos[4];
};

typedef struct _point Point;
typedef struct _point Vector;

typedef float *Matrix;

static inline void init_point(float x, float y, float z, Point *p) {
    p->pos[0] = x;
    p->pos[1] = y;
    p->pos[2] = z;
}

static inline void init_point_p(Point t, Point *p) {
    p->pos[0] = t.pos[0];
    p->pos[1] = t.pos[1];
    p->pos[2] = t.pos[2];
}

void gen_transform_matrix(Point cur_tan, Point nxt_tan, Point nxt_curve,
                          Point *cur_norm, Matrix result);
void gen_rotate_matrix(Vector frm_vec, Vector to_vec, Matrix result);

#endif