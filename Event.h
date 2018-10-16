#ifndef EVENT_H
#define EVENT_H

#include "AVL.h"
#include "geom_utils.h"

enum EventType
{
    SiteEvent,
    CircleEvent
};

struct Event
{
    EventType type;
    Point p;
    int idx;           // index of this site in some list/vector, if this event is of type SiteEvent;
                       // -1, if this event is of type CircleEvent
    AVLTreeNode *leaf; // if this event is of type CircleEvent, this pointer will
                       // point to the corresponding leaf node in AVLTree
    Event(EventType t, double x, double y, int idx = -1,
          AVLTreeNode *l = nullptr) : type(t), p(x, y), idx(idx), leaf(l) {}
    Event(EventType t, const Point &p, int idx = -1,
          AVLTreeNode *l = nullptr) : type(t), p(p), idx(idx), leaf(l) {}
    Event(const Event &) = default;
    inline double get_y() const { return p.y; }
};

/* compare priorities of two events */
bool cmp_event(const Event &e1, const Event &e2);

#endif