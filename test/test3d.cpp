#include <stdio.h>
#include <stdlib.h>
#include "3d.h"

using namespace std;

void TEST_1() {
  Point *p_points = new Point[60];
  Point *p_tan_points = new Point[(7 - 3) * 60];
  Point *p_cpoints = new Point[7];
  init_point(0.0, 0.0, 0.0, &p_cpoints[0]);
  init_point(0.5, 0.5, 0.7, &p_cpoints[1]);
  init_point(0.0, 8.0, 0.1, &p_cpoints[2]);
  init_point(10.0, 0.2, 0.5, &p_cpoints[3]);
  init_point(5.0, 1.2, 4.5, &p_cpoints[4]);
  init_point(6.0, 4.2, 3.2, &p_cpoints[5]);
  init_point(9.0, 7.7, -9.2, &p_cpoints[6]);
  generate_cr_line(p_points, 60, p_tan_points, 60, p_cpoints, 7, 60);
  // for (int i = 0; i < (7 - 3) * 60; ++i) {
  //   printf("point:  %f %f %f\n", p_points[i].pos[0], p_points[i].pos[1],
  //          p_points[i].pos[2]);
  //   printf("direct: %f %f %f\n", p_tan_points[i].pos[0],
  //   p_tan_points[i].pos[1],
  //          p_tan_points[i].pos[2]);
  // }
  Vector v1, v2, v;
  init_point(1.0, 0.0, 0.0, &v1);
  init_point(0.0, 1.0, 0.0, &v2);
  generate_rotate_vec(v1, v2, &v);
  // printf("vector: %f %f %f\n", v.pos[0], v.pos[1], v.pos[2]);
  
  float mat[16];
  generate_quater_matrix(v, 20.0 * PI / 180.0, mat);

  // for (int i = 0; i < 4; ++ i) {
  //   for (int j = 0; j < 4; ++ j) {
  //     printf("%4f ", mat[i * 4 + j]);
  //   }
  //   printf("\n");
  // }

  delete[] p_points;
  delete[] p_tan_points;
  delete[] p_cpoints;
}

int main() {
  TEST_1();
  return 0;
}