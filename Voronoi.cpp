#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include "AVL.h"
#include "Event.h"
#include "geom_utils.h"

using namespace std;

const string input_file = "input_file.txt";

/* list of sites (i.e. given fixed points) */
vector<Point> sites;

/* priority queue of pending events to be tackled:
   1. those with larger y-component have higher priority;
   2. SiteEvents have higher priority than CircleEvents */
priority_queue<Event, vector<Event>, decltype(cmp_event)> pqueue;

/* modified AVL Tree used to represent the current "beach line":
   leaf nodes represent "arcs";
   internal nodes represent "break points" */
AVLTree tree(sites);

void Tackle_SiteEvent(const Event &event)
{
    int idx = event.idx;
    AVLTreeNode *l = tree.get_leaf(idx);
    tree.repl_leaf(l, idx);
}

void Tackle_CircleEvent(const Event &event) {}

int main(int argc, char const *argv[])
{
    int n; // # of sites (i.e. fixed points)
    /* 初始化双向链接边表D ：将其置空 */

    /* read data from file */
    {
        ifstream fin(input_file);
        fin >> n;
        for (int i = 0; i < n; ++i)
        {
            double x, y;
            fin >> x >> y;
            sites.emplace_back(x, y);
        }
        fin.close();
    }
    /* initialize pqueue (i.e. events) */
    for (int i = 0; i < n; ++i)
    {
        pqueue.emplace(SiteEvent, sites[i], i);
    }

    /* tackle the topest layer of site events */
    {
        double ymax = pqueue.top().p.y;
        vector<int> ids;
        while (!pqueue.empty() && pqueue.top().p.y == ymax)
        {
            ids.push_back(pqueue.top().idx);
            pqueue.pop();
        }
        tree.insert_topest_sites(ids);
    }

    /* main algorithm */
    while (!pqueue.empty())
    {
        Event curr_event = pqueue.top();
        pqueue.pop();
        if (curr_event.type == SiteEvent)
            Tackle_SiteEvent(curr_event);
        else
            Tackle_CircleEvent(curr_event);
    }

    return 0;
}
