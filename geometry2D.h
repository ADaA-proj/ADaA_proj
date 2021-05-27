#include "shape2D.h"
#include <vector>

// norm denotes to what norm to compute, -1 denotes to l_infinity norm
double Distance(Point p, Line l, int norm = 2);

Polygon ConvexHall(std::vector<Point> point_list);

Polygon InterSection(std::vector<Polygon> polygon_list);
Point InterSection(Line l_1, Line L_2);
