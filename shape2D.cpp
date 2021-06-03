#include "shape2D.h"

inline bool eq(double a,double b)
{
    return ((a-b)<eps)||((b-a)<eps);
}

Point::Point(double x_, double y_) : x(x_), y(y_) {}
double Point::getx()
{
    return x;
}
double Point::gety()
{
    return y;
}

Line::Line(double interceptx_or_k, double intercepty_or_b, bool use_kb = 0)
{
    if (use_kb == 0)
    {
        intercept_x = interceptx_or_k, intercept_y = intercepty_or_b;
        k = -intercept_y / intercept_x;
    }
    else
    {
        k = interceptx_or_k, intercept_y = intercepty_or_b;
        intercept_x = -intercept_y / k;
    }
}
Line::Line(Point a, Point b)
{
    if (eq(a.gety(), b.gety()))
        intercept_x = GEO_INF, intercept_y = a.gety(), k = 0;
    else if (eq(a.getx(), b.getx()))
        intercept_x = a.getx(), intercept_y = GEO_INF, k = GEO_INF;
    else
    {
        intercept_x = a.getx() - a.gety() * (a.getx() - b.getx()) / (a.gety() - b.gety());
        intercept_y = a.gety() - a.getx() * (a.gety() - b.gety()) / (a.getx() - b.getx());
        k = -intercept_y / intercept_x;
    }
}

Polygon::Polygon(std::vector<Point> &p_list_)//稍微修改，可能传引用比较节省时间
{
    p_list.assign(p_list_.begin(), p_list_.end());
}

Point Common_point(Line a,Line b)
{
    if(eq(a.k,b.k)==1)return Point(GEO_INF,GEO_INF);
    double interX,interY;
    if(eq(a.k,GEO_INF)){
        interX=a.intercept_x;
        interY=b(interX);
    }
    else if(eq(b.k,GEO_INF)){
        interX=b.intercept_x;
        interY=a(interX);
    }
    else{
        interX=-(b.intercept_y-a.intercept_y)/(b.k-a.k);
        interY=a(interX);
    }
    return Point(interX,interY);
}

Polygon::Polygon(std::vector<Line> &l_list_)//暂时假设他的传输都是按照一定顺序,后面可以通过凸包、半平面交等方法不要求顺序
{
    for(int i=1;i<l_list_.size();++i)
    {
        p_list.push_back(Common_point(l_list_[i-1],l_list_[i]));
    }
    p_list.push_back(Common_point(l_list_[l_list_.size()-1],l_list_[0]));
}

inline double cross_prod(Point a,Point b)
{//need checking
    return b.gety()*a.getx()-b.getx()*a.gety();
}

inline double sqr(double a){return a*a;}

double distance(Point a,Point b)
{
    return std::sqrt(sqr(a.getx()-b.getx())+sqr(a.gety()-b.gety()));
}

double Polygon::Area()
{
    double ans=0;
    if(p_list.size()<=1){
        printf("empty polygon!");
        throw "empty polygon!";
        return 0;
    }
    for(int i=1;i<p_list.size();++i)
        ans+=cross_prod(p_list[i],p_list[i-1]);
    ans+=cross_prod(p_list[0],p_list[p_list.size()-1]);
    return abs(ans);
}

double Polygon::Perimeter()
{
    double ans=0;
    if(p_list.size()<=1){
        printf("empty polygon!");
        throw "empty polygon!";
        return 0;
    }
    for(int i=1;i<p_list.size();++i)
        ans+=distance(p_list[i],p_list[i-1]);
    ans+=distance(p_list[0],p_list[p_list.size()-1]);
    return ans;
}



double ConicSection::Perimeter()
{
    if (e >= 1)
        return GEO_INF;
    else if (e == 0)
        return 2 * PI * a;
    else
    {
        // An approximate formula for the perimeter of an ellipse
        double Q = a + b;
        double h = std::pow((a - b) / (a + b), 2);
        double area = PI * Q * (1 + 3 * h / (10 + std::sqrt(4 - 3 * h)));
        if (b < a / 2)
        {
            double m = 22 / 7 / PI - 1;
            double n = std::pow(1 - b / a, 33.697);
            area *= (1 + n * m);
        }
        return area;
    }
}
double ConicSection::Area()
{
    if (e >= 1)
        return GEO_INF;
    return PI * a * b;
}
int main()
{

}
