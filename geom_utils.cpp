#include "geom_utils.h"
#include <cassert>

using namespace std;

/* used in Get_Relation */
inline int myfunc(double a, double b)
{
    if (a == b)
        return 0;
    else
        return b > a ? 1 : -1;
}

/* Given three different points pi, pj, pk, assuming:
   1. current "beach line" is y=y_k;
   2. pk is not higher (with respect to y-component) than pi and pj, respectively;
   3. pi and pj has a break point BP(i,j) (note: different from BP(j,i));
   this function decides the relationship between x-components of pk and BP(i,j).
   return:
   0:  x_k == x_BP
   1:  x_k > x_BP
   -1: x_k < x_BP */
int Get_Relation(const Point &pi, const Point &pj, const Point &pk)
{
    double xi = pi.x, yi = pi.y;
    double xj = pj.x, yj = pj.y;
    double xk = pk.x, yk = pk.y;
    if (yi == yk)
    {
        return myfunc(xi, xk);
    }
    else if (yj == yk)
    {
        return myfunc(xj, xk);
    }
    // pi and pj are all above pk
    else if (yi == yj)
    {
        assert(xi < xj);
        return myfunc(0.5 * (xi + xj), xk);
    }
    else if (xi == xj)
    {
        if (xk > xi && yi > yj)
            return 1;
        if (xk < xi && yi < yj)
            return -1;
        double d2 = (0.5 * (yi + yj) - yk) * (0.5 * (yi + yj) - yk);
        double dk2 = (0.5 * (yi - yj)) * (0.5 * (yi - yj)) + (xi - xk) * (xi - xk);
        if (xk > xi)
            return myfunc(d2, dk2);
        else
            return myfunc(dk2, d2);
    }
    else
    {
        double yki = (xk - xi) * (xk - xi) / (yi - yk) + yi;
        double ykj = (xk - xj) * (xk - xj) / (yj - yk) + yj;
        if (yi > yj)
        {
            if (yki == ykj)
                return xk < xj ? 0 : 1;
            else if (yki > ykj)
                return 1;
            else
                return xk < xj ? -1 : 1;
        }
        else
        {
            if (yki == ykj)
                return xk > xi ? 0 : 1;
            else if (yki < ykj)
                return -1;
            else
                return xk < xi ? -1 : 1;
        }
    }
}

/* return whether p1 is above p2 (w.r.t y-component) */
inline bool above(const Point &p1, const Point &p2)
{
    return p1.y > p2.y;
}

/* return whether p1 is left to p2 (w.r.t x-component) */
inline bool leftto(const Point &p1, const Point &p2)
{
    return p1.x < p2.x;
}