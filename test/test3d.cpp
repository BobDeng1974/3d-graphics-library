#include <stdio.h>
#include <stdlib.h>
#include "3d.h"

using namespace std;

int main() {
  Point *p_points = (Point *)malloc(60 * sizeof(Point));
  Point *p_tan_points = (Point *)malloc((7 - 3) * 60 * sizeof(Point));
  Point *p_cpoints = (Point *)malloc(7 * sizeof(Point));
  init_point(0.0, 0.0, 0.0, &p_cpoints[0]);
  init_point(0.5, 0.5, 0.7, &p_cpoints[1]);
  init_point(0.0, 8.0, 0.1, &p_cpoints[2]);
  init_point(10.0, 0.2, 0.5, &p_cpoints[3]);
  init_point(5.0, 1.2, 4.5, &p_cpoints[4]);
  init_point(6.0, 4.2, 3.2, &p_cpoints[5]);
  init_point(9.0, 7.7, -9.2, &p_cpoints[6]);
  generate_cr_line(p_points, 60, p_tan_points, 60, p_cpoints, 7, 60);
  for (int i = 0; i < (7 - 3) * 60; ++i) {
    printf("point:  %f %f %f\n", p_points[i].pos[0], p_points[i].pos[1],
           p_points[i].pos[2]);
    printf("direct: %f %f %f\n", p_tan_points[i].pos[0], p_tan_points[i].pos[1],
           p_tan_points[i].pos[2]);
  }
  return 0;
}