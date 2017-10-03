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
  return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

/* Convert quaternion to rotation matrix */
void generate_quater_matrix(Vector rotate_vec, float d_degree,
                            Matrix rotate_mat) {
  float w = cos(d_degree / 2.0f);
  float x = rotate_vec.pos[0] * sin(d_degree / 2.0f);
  float y = rotate_vec.pos[1] * sin(d_degree / 2.0f);
  float z = rotate_vec.pos[2] * sin(d_degree / 2.0f);
  rotate_mat[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
  rotate_mat[4] = 2.0f * x * y + 2.0f * w * z;
  rotate_mat[8] = 2.0f * x * z - 2.0f * w * y;
  rotate_mat[1] = 2.0f * x * y + 2.0f * w * z;
  rotate_mat[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
  rotate_mat[9] = 2.0f * y * z + 2.0f * w * x;
  rotate_mat[2] = 2.0f * x * z + 2.0f * w * y;
  rotate_mat[6] = 2.0f * y * z - 2.0f * w * z;
  rotate_mat[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
  rotate_mat[3] = rotate_mat[7] = rotate_mat[11] = rotate_mat[12] =
      rotate_mat[13] = rotate_mat[14] = 0.0f;
  rotate_mat[15] = 1.0f;
}

/* Rotate vector is perpendicular to the plane where cur_vec and tan_vec stay */
void generate_rotate_vec(Vector cur_vec, Vector tan_vec, Vector *rotate_vec) {
  rotate_vec->pos[0] =
      cur_vec.pos[1] * tan_vec.pos[2] - cur_vec.pos[2] * tan_vec.pos[1];
  rotate_vec->pos[1] =
      cur_vec.pos[2] * tan_vec.pos[0] - cur_vec.pos[0] * tan_vec.pos[2];
  rotate_vec->pos[2] =
      cur_vec.pos[0] * tan_vec.pos[1] - cur_vec.pos[1] * tan_vec.pos[0];
}

/* Calculate angle between vector v1 and vector v2. Return float (-PI, PI]
 * Reference:
 * https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
 */
float cal_angle(Vector vec1, Vector vec2, Vector rotate_vec) {
  float dot = dot(vec1, vec2);
  float det = det(vec1, vec2, rotate_vec);
  return atan2(det, dot);
}
