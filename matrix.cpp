#include <math.h>
#include "3d.h"

/* convert quaternion to rotation matrix */
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

/* rotate vector is perpendicular to the plane where cur_vec and tan_vec stay */
void generate_rotate_vec(Vector cur_vec, Vector tan_vec, Vector *rotate_vec) {
  rotate_vec->pos[0] =
      cur_vec.pos[1] * tan_vec.pos[2] - cur_vec.pos[2] * tan_vec.pos[1];
  rotate_vec->pos[1] =
      cur_vec.pos[2] * tan_vec.pos[0] - cur_vec.pos[0] * tan_vec.pos[2];
  rotate_vec->pos[2] =
      cur_vec.pos[0] * tan_vec.pos[1] - cur_vec.pos[1] * tan_vec.pos[0];
}