#pragma once

#include "shape2D.h"
#include <vector>
#include <cstdlib>

//typedef std::pair<double, double> P;
//typedef double (*func_t)(Line, Ellipse);

double cross(Vec p, Vec q);
double degree(Point p, Point q); //两点表示向量的有向角度

// norm denotes to what norm to compute, -1 denotes to l_infinity norm
double Distance(Point p, const Line &l, int norm = 2);
double Distance(Point p, Point q);
double Distance(Point p, LineSegment l);
double Distance(std::pair<Point, Point>);

Point Common_point(const Line &a, const Line &b);

Polygon ConvexHall(std::vector<Point> point_list);
Polygon InterSection(const Polygon &a, const Polygon &b);
Polygon HalfPlaneIntersection(std::vector<HalfPlane> Line_list);
double MinDistance(const Polygon &a, const Polygon &b); // checking

std::pair<Point, Point> Common_Point(const Line &l, const Ellipse &e);

template <class type>
std::pair<double, type> Best_Arg(bool (*cmp)(type, type), type (*op)(double), double max_arg, double min_arg);
std::pair<double, double> Best_Arg(double (*op)(double), double max_arg, double min_arg);
