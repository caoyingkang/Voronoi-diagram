#ifndef GEOM_UTILS_H
#define GEOM_UTILS_H

/* geom_utils: utilities for geometric calculation */

struct Point
{
    double x, y; // x- and y-component
    Point() {}
    Point(const Point &) = default;
    Point(double x, double y) : x(x), y(y) {}
};

/* Given three different points pi, pj, pk, assuming:
   pk is not higher (with respect to y-component) than pi and pj, respectively,
   this function decides the relationship between x-components of
   pk and the break point BP(pi,pj) (note: different from BP(pj,pi)).
   return:
   0:  x_k == x_BP
   1:  x_k > x_BP
   -1: x_k < x_BP */
int Get_Relation(const Point &pi, const Point &pj, const Point &pk);

/* return whether p1 is above p2 (w.r.t y-component) */
inline bool above(const Point &p1, const Point &p2);

/* return whether p1 is left to p2 (w.r.t x-component) */
inline bool leftto(const Point &p1, const Point &p2);

#endif