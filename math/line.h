#ifndef _LINE_H
#define _LINE_H
#include "3d.h"

void gen_cr_line(Point *p_points, Point *p_tan_points, Point *p_curve_points,
                 Point *p_control_points, int n_clen, int n_frame,
                 const int type);
#endif