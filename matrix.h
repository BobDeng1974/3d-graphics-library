#ifndef _MATRIX_H
#define _MATRIX_H
#include "3d.h"

double IDENTITY_MAT[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};

void gen_quater_matrix(Vector rotate_vec, float d_degree, Matrix rotate_mat);

bool gen_rotate_vec(Vector cur_vec, Vector tan_vec, Vector *rotate_vec);

float cal_angle(Vector vec1, Vector vec2, Vector rotate_vec);

void gen_normal_vec(Vector prev_vec, Vector *cur_vec);

void gen_trans_matrix(Point p_pos, Matrix r);

void gen_4mul_mat(float a[16], float b[16]);

#endif