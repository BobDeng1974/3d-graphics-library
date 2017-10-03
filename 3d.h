#ifndef _3D_H
#define _3D_H

struct _point {
  float pos[4];
};

typedef struct _point Point;
typedef struct _point Vector;

typedef float* Matrix;

static inline void init_point(float x, float y, float z, Point *p) {
  p->pos[0] = x;
  p->pos[1] = y;
  p->pos[2] = z;
}

void generate_cr_line(Point *p_points, int n_len, Point *p_tan_points,
                      int n_tan_len, Point *p_control_points, int n_clen,
                      int n_frame);

#endif