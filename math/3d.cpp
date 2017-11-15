#include "3d.h"
#include <stdlib.h>
#include "line.h"
#include "matrix.h"

/* generate transform matrix by quaternion */
void gen_transform_matrix(Point cur_tan, Point nxt_tan, Point nxt_curve,
                          Point cur_norm, Matrix m1, Matrix m2, const int rotate_type) {
    // Matrix m1 = new float[16];
    // Matrix m2 = new float[16];

    /* 2.  rotate cur_tan to nxt_tan */
    // gen_rotate_matrix(cur_tan, nxt_tan, temp_res);

    /* 3. rotate cur_norm to nxt_norm */

    /* 3.1 let nxt_norm is rotate_tan. Accoring to Frenet-frame next normal
    * vector is
    * orthogonal vector of f'(x): nxt_tan and f''(x): nxt_curve.
    */
    Vector nxt_norm;
    gen_rotate_vec(nxt_tan, nxt_curve, &nxt_norm);
    gen_rotate_vec(nxt_tan, nxt_norm, &nxt_norm);

    /* 3.2 generate normal vector of cur_norm and nxt_norm */
    /* TODO: bug when try rotate norm vector */

    //gen_rotate_matrix(cur_norm, nxt_norm, m1, rotate_type);
    gen_rotate_matrix(cur_tan, nxt_tan, m2, rotate_type);

    // gen_4mul_mat(temp_res, temp_mat);

    // for (int i = 0; i < 16; ++i) {
    //    result[i] = temp_res[i];
    //}
}

/* rotate frm_vec to to_vec */
void gen_rotate_matrix(Vector frm_vec, Vector to_vec, Matrix result, const int rotate_type) {
    Matrix temp_mat = new float[16];
    /* 1.1 generte normal vector of frm_vec and to_vec */
    Vector rotate_vec;
    if (!gen_rotate_vec(frm_vec, to_vec, &rotate_vec)) {
        /* cur_tan and nxt_tan coincide */
        // for (int i = 0; i < 16; ++i) {
        //    result[i] = IDENTITY_MAT[i];
        //}
        // return;
    }
    /* 1.2 calculate angle between frm_vec and to_vec */
    float angle = cal_angle(frm_vec, to_vec, rotate_vec);
    if (angle > 0.3) printf("%f\n", angle);
    /* 1.3 generate rotate matrix */
    if (rotate_type == R_QUART) {
        gen_quater_matrix(rotate_vec, angle, temp_mat);
    } else if (rotate_type == R_EULER) {
        gen_euler_matrix(rotate_vec, angle, temp_mat);
    }
    /* cur_tan and nxt_tan coincide */
    for (int i = 0; i < 16; ++i) {
        result[i] = temp_mat[i];
    }
}