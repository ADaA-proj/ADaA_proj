#pragma once

#include "shape2D.h"
#include <vector>

// norm denotes to what norm to compute, -1 denotes to l_infinity norm
double Distance(Point p, Line l, int norm = 2);
double Distance(Point p, Point q);

Polygon ConvexHall(std::vector<Point> point_list);

Polygon InterSection(std::vector<Polygon> polygon_list);
Point Common_point(Line a, Line b);
