#include "line.h"
#include <math.h>
#include "3d.h"

/* Catmull-Rom line and B-Splines*/
void gen_cr_line(Point *p_points, Point *p_tan_points, Point *p_curve_points,
                 Point *p_control_points, int n_clen, int n_frame,
                 const int type) {
    int index = 0;
    if (type == L_CATMULL) {
        for (int c_ix = 0; c_ix < n_clen - 3; ++c_ix) {
            for (int i = 0; i < n_frame; ++i) {
                float u = (float)i * 1.0f / n_frame;
                float u_2 = u * u;
                float u_3 = u_2 * u;
                for (int k = 0; k < 3; ++k) {
                    p_points[index].pos[k] =
                        ((-1.0f * u_3 + 2.0f * u_2 - 1.0f * u + 0.0f) *
                             p_control_points[c_ix].pos[k] +
                         (3.0f * u_3 - 5.0f * u_2 + 0.0f * u + 2.0f) *
                             p_control_points[c_ix + 1].pos[k] +
                         (-3.0f * u_3 + 4.0f * u_2 + 1.0f * u + 0.0f) *
                             p_control_points[c_ix + 2].pos[k] +
                         (1.0f * u_3 - 1.0f * u_2 + 0.0f * u + 0.0f) *
                             p_control_points[c_ix + 3].pos[k]) *
                        0.5f;

                    p_tan_points[index].pos[k] =
                        ((-3.0f * u_2 + 4.0f * u - 1.0f) *
                             p_control_points[c_ix].pos[k] +
                         (9.0f * u_2 - 10.0f * u + 0.0f) *
                             p_control_points[c_ix + 1].pos[k] +
                         (-9.0f * u_2 + 8.0f * u + 1.0f) *
                             p_control_points[c_ix + 2].pos[k] +
                         (3.0f * u_2 - 2.0f * u + 0.0f) *
                             p_control_points[c_ix + 3].pos[k]) *
                        0.5f;

                    p_curve_points[index].pos[k] =
                        ((-6.0f * u + 4.0f) * p_control_points[c_ix].pos[k] +
                         (18.0f * u - 10.0f) *
                             p_control_points[c_ix + 1].pos[k] +
                         (-18.0f * u + 8.0f) *
                             p_control_points[c_ix + 2].pos[k] +
                         (6.0f * u - 2.0f) *
                             p_control_points[c_ix + 3].pos[k]) *
                        0.5f;
                }
                index++;
            }
        }
    } else if (type == L_BSPLINE) {
        for (int c_ix = 0; c_ix < n_clen - 3; ++c_ix) {
            for (int i = 0; i < n_frame; ++i) {
                float u = (float)i * 1.0f / n_frame;
                float u_2 = u * u;
                float u_3 = u_2 * u;
                for (int k = 0; k < 3; ++k) {
                    p_points[index].pos[k] =
                        ((-1.0f * u_3 + 3.0f * u_2 - 3.0f * u + 1.0f) *
                             p_control_points[c_ix].pos[k] +
                         (3.0f * u_3 - 6.0f * u_2 + 0.0f * u + 4.0f) *
                             p_control_points[c_ix + 1].pos[k] +
                         (-3.0f * u_3 + 3.0f * u_2 + 3.0f * u + 1.0f) *
                             p_control_points[c_ix + 2].pos[k] +
                         (1.0f * u_3 + 0.0f * u_2 + 0.0f * u + 0.0f) *
                             p_control_points[c_ix + 3].pos[k]) *
                        1.0f / 6.0f;

                    p_tan_points[index].pos[k] =
                        ((-3.0f * u_2 + 6.0f * u - 3.0f) *
                             p_control_points[c_ix].pos[k] +
                         (9.0f * u_2 - 12.0f * u + 0.0f) *
                             p_control_points[c_ix + 1].pos[k] +
                         (-9.0f * u_2 + 6.0f * u + 3.0f) *
                             p_control_points[c_ix + 2].pos[k] +
                         (3.0f * u_2 + 0.0f * u + 0.0f) *
                             p_control_points[c_ix + 3].pos[k]) *
                        1.0f / 6.0f;

                    p_curve_points[index].pos[k] =
                        ((-6.0f * u + 6.0f) * p_control_points[c_ix].pos[k] +
                         (18.0f * u - 12.0f) *
                             p_control_points[c_ix + 1].pos[k] +
                         (-18.0f * u + 6.0f) *
                             p_control_points[c_ix + 2].pos[k] +
                         (6.0f * u + 0.0f) *
                             p_control_points[c_ix + 3].pos[k]) *
                        1.0f / 6.0f;
                }
                index++;
            }
        }
    }
}
