#include "Event.h"
#include <cassert>

using namespace std;

/* compare priorities of two events */
bool cmp_event(const Event &e1, const Event &e2)
{
    if (e1.get_y() != e2.get_y())
        return e1.get_y() < e2.get_y();
    else
    {
        assert(e1.type != e2.type);
        return e1.type == CircleEvent;
    }
}