#include <math.h>
#include "3d.h"

/* Catmull-Rom line */
void generate_cr_line(Point *p_points, int n_len,
                      Point *p_tan_points, int n_tan_len,
                      Point *p_control_points, int n_clen, int n_frame) {
  int index = 0;
  for (int c_ix = 0; c_ix < n_clen - 3; ++c_ix) {
    for (int i = 0; i < n_len; ++i) {
      float u = (float)i * 1.0 / n_frame;
      float u_2 = u * u;
      float u_3 = u_2 * u;
      for (int k = 0; k < 3; ++k) {
        p_points[index].pos[k] = ((-1.0f * u_3 + 2.0f * u_2 - 1.0f * u + 0.0f) *
                                      p_control_points[c_ix].pos[k] +
                                  (3.0f * u_3 - 5.0f * u_2 + 0.0f * u + 2.0f) *
                                      p_control_points[c_ix + 1].pos[k] +
                                  (-3.0f * u_3 + 4.0f * u_2 + 1.0f * u + 0.0f) *
                                      p_control_points[c_ix + 2].pos[k] +
                                  (1.0f * u_3 - 1.0f * u_2 + 0.0f * u + 0.0f) *
                                      p_control_points[c_ix + 3].pos[k]) *
                                 0.5f;

        p_tan_points[index].pos[k] =
            ((-3.0f * u_2 + 4.0f * u - 1.0f) * p_control_points[c_ix].pos[k] +
             (9.0f * u_2 - 10.0f * u + 0.0f) *
                 p_control_points[c_ix + 1].pos[k] +
             (-9.0f * u_2 + 8.0f * u + 1.0f) *
                 p_control_points[c_ix + 2].pos[k] +
             (3.0f * u_2 - 2.0f * u + 0.0f) *
                 p_control_points[c_ix + 3].pos[k]) *
            0.5f;
      }
      index++;
    }
  }
}
