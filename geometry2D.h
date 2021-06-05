#pragma once

#include "shape2D.h"
#include <vector>

// norm denotes to what norm to compute, -1 denotes to l_infinity norm
double Distance(Point p, Line l, int norm = 2);
double Distance(Point p, Point q);
double Distance(Point p, Point a, Point b);

Point Common_point(Line a, Line b);

Polygon ConvexHall(std::vector<Point> point_list);
Polygon InterSection(const Polygon &a, const Polygon &b);
Polygon Merge(const Polygon &a, const Polygon &b);
