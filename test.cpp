#include "geometry2D.h"
#include <iostream>
using namespace std;

Ellipse e = Ellipse(5, 4);
double f(double theta)
{
    Line l;
    if (abs(theta) < 1e-6)
        l = Line(1, GEO_INF);
    else
        l = Line(1, 1 / tan(theta));
    pair<Point, Point> ans = Common_Point(l, e);
    return abs(ans.first.y - ans.second.y);
}

int main()
{
    Line l = Line(1, GEO_INF);
    pair<Point, Point> ans = Common_Point(l, e);
    cout << abs(ans.first.y - ans.second.y) << endl;
    pair<double, double> out = Best_Arg(f, PI / 9, -PI / 3);
    cout << out.first << ' ' << out.second << endl;
    return 0;
}