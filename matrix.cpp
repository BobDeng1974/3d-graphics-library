#include <math.h>
#include "3d.h"

inline float det(Vector v1, Vector v2, Vector norm) {
    return ((v1.pos[0] * v2.pos[1] * norm.pos[2]) +
            (v1.pos[1] * v2.pos[2] * norm.pos[0]) +
            (v1.pos[2] * v2.pos[0] * norm.pos[1]) -
            (v1.pos[0] * v2.pos[2] * norm.pos[1]) -
            (v1.pos[1] * v2.pos[0] * norm.pos[2]) -
            (v1.pos[2] * v2.pos[1] * norm.pos[0]));
}

inline float dot(Vector v1, Vector v2) {
    return (v1.pos[0] * v2.pos[0] + v1.pos[1] * v2.pos[1] +
            v1.pos[2] * v2.pos[2]);
}

inline float mod(Vector v) {
    return sqrt(v.pos[0] * v.pos[0] + v.pos[1] * v.pos[1] +
                v.pos[2] * v.pos[2]);
}

/* Convert quaternion to rotation matrix. */
void gen_quater_matrix(Vector rotate_vec, float d_degree, Matrix rotate_mat) {
    float w = cos(d_degree / 2.0f);
    float x = rotate_vec.pos[0] * sin(d_degree / 2.0f);
    float y = rotate_vec.pos[1] * sin(d_degree / 2.0f);
    float z = rotate_vec.pos[2] * sin(d_degree / 2.0f);
    rotate_mat[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
    rotate_mat[4] = 2.0f * x * y - 2.0f * w * z;
    rotate_mat[8] = 2.0f * x * z + 2.0f * w * y;
    rotate_mat[1] = 2.0f * x * y + 2.0f * w * z;
    rotate_mat[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
    rotate_mat[9] = 2.0f * y * z - 2.0f * w * x;
    rotate_mat[2] = 2.0f * x * z - 2.0f * w * y;
    rotate_mat[6] = 2.0f * y * z + 2.0f * w * x;
    rotate_mat[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
    rotate_mat[3] = rotate_mat[7] = rotate_mat[11] = rotate_mat[12] =
        rotate_mat[13] = rotate_mat[14] = 0.0f;
    rotate_mat[15] = 1.0f;
}

/* generate translation matrix for [x, y, z]
 * | 1 0 0 x |
 * | 0 1 0 y |
 * | 0 0 1 z |
 * | 0 0 0 1 |
*/
void gen_translate_matrix(Point p_pos, Matrix r) {
    for (int i = 0; i < 16; ++i) r[i] = 0.0f;
    r[0] = r[5] = r[10] = r[15] = 1.0f;
    r[12] = p_pos.pos[0];
    r[13] = p_pos.pos[1];
    r[14] = p_pos.pos[2];
}

/* Rotate unit vector is perpendicular to the plane where cur_vec and tan_vec
 * stay */
bool gen_rotate_vec(Vector cur_vec, Vector tan_vec, Vector *rotate_vec) {
    rotate_vec->pos[0] =
        cur_vec.pos[1] * tan_vec.pos[2] - cur_vec.pos[2] * tan_vec.pos[1];
    rotate_vec->pos[1] =
        cur_vec.pos[2] * tan_vec.pos[0] - cur_vec.pos[0] * tan_vec.pos[2];
    rotate_vec->pos[2] =
        cur_vec.pos[0] * tan_vec.pos[1] - cur_vec.pos[1] * tan_vec.pos[0];
    float f_mod = mod(*rotate_vec);
    if (f_mod - 0.0f < 1e-6) return false;
    rotate_vec->pos[0] /= f_mod;
    rotate_vec->pos[1] /= f_mod;
    rotate_vec->pos[2] /= f_mod;
    return true;
}

/* If angle between current normal vector and previous one greater than 180,
    reverse it */
void gen_normal_vec(Vector prev_vec, Vector *cur_vec) {
    if (dot(prev_vec, *cur_vec) < 0.0f) {
        cur_vec->pos[0] = -cur_vec->pos[0];
        cur_vec->pos[1] = -cur_vec->pos[1];
        cur_vec->pos[2] = -cur_vec->pos[2];
    }
}

/* Calculate angle between vector v1 and vector v2. Return float (-PI, PI]
 * Reference:
 * https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
 */
float cal_angle(Vector vec1, Vector vec2, Vector rotate_vec) {
    float f_dot = dot(vec1, vec2);
    float f_det = det(vec1, vec2, rotate_vec);
    return atan2(f_det, f_dot);
}

/* calculate 4 X 4 matrix a multiply b row-major, save result to a */
void gen_4mul_mat(float a[16], float b[16]) {
    float t[16]; /* (tmp result) */
    int i, j, k;

    for (i = 0; i < 4; i++) {
        const float *ai = a + (i * 4);
        float *ti = t + (i * 4);

        for (j = 0; j < 4; j++) {
            float tij = 0.0;
            for (k = 0; k < 4; k++) tij += ai[k] * b[k * 4 + j];
            ti[j] = tij; /* r[i][j] (inner product) */
        }
    }

    for (i = 0; i < 16; i++) /* copy elements back to result: */
        a[i] = t[i];
}
