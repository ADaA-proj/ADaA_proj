#pragma once

#include "shape2D.h"
#include <vector>

double cross(Vec p, Vec q);
double degree(Point p, Point q); //两点表示向量的有向角度

// norm denotes to what norm to compute, -1 denotes to l_infinity norm
double Distance(Point p, const Line &l, int norm = 2);
double Distance(Point p, Point q);
double Distance(Point p, LineSegment l);

Point Common_point(const Line &a, const Line &b);

Polygon ConvexHall(std::vector<Point> point_list);
Polygon InterSection(const Polygon &a, const Polygon &b);
Polygon HalfPlaneIntersection(std::vector<HalfPlane> Line_list);
Polygon Merge(const Polygon &a, const Polygon &b);
double MinDistance(const Polygon &a, const Polygon &b); // checking

std::pair<Point, Point> CommonPoint(const Line &l, const Ellipse &e);