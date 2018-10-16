#include "AVL.h"
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

inline bool AVLTreeNode::isLeaf()
{
    return !left && !right;
}

inline bool AVLTree::isEmpty()
{
    return !root;
}

void AVLTree::setRoot(AVLTreeNode *r)
{
    root = r;
    root->parent = nullptr;
}

void AVLTree::del_subtree(AVLTreeNode *r)
{
    if (r == nullptr)
        return;
    del_subtree(r->left);
    del_subtree(r->right);
    delete r;
}

AVLTree::~AVLTree()
{
    del_subtree(root);
}

void AVLTree::insert_topest_sites(const std::vector<int> &ids)
{
    assert(ids.size() > 0);
    sort(ids.begin(), ids.end(),
         [this](int i, int j) { return sites[i].x < sites[j].x; });
    int n = ids.size();
    AVLTreeNode *l = nullptr;
    for (int idx : ids)
    {
        if (!l)
            l = root = new AVLTreeNode(nullptr, idx);
        else
            l = repl_leaf(l, idx, true);
    }
}

/* assume *p is an internal node */
inline AVLTreeNode *AVLTree::descend(AVLTreeNode *p, int idx) const
{
    assert(!p->isLeaf());
    int rel = Get_Relation(sites[p->idx], sites[p->idx2], sites[idx]);
    return rel == 1 ? p->right : p->left;
}

/* insert leaf node,
   idx is the index of the corresponding inserted site.
   assume that there is at least one site above the current "sweep line". 
void AVLTree::insert_site(int idx)
{
    assert(!isEmpty());
    AVLTreeNode *r = root;
    while (!r->isLeaf())
        r = descend(r, idx);
    repl_leaf(r, idx);
}*/

/* find which leaf to be replaced by site of idx.
   assume that there is at least one site above the current "sweep line". */
AVLTreeNode *AVLTree::get_leaf(int idx)
{
    assert(!isEmpty());
    AVLTreeNode *r = root;
    while (!r->isLeaf())
        r = descend(r, idx);
    return r;
}

/* replace the leaf *l.
   idx is the index of the inserted site.
   If flag == false, there is at least one site above the current "sweep line";
   If flag == true, current "sweep line" is at the topest layer of sites.
   return: pointer to the new inserted leaf */
AVLTreeNode *AVLTree::repl_leaf(AVLTreeNode *l, int idx, bool flag = false)
{
    assert(l->isLeaf());
    int i = l->idx;
    if (l == root)
    {
        root = new AVLTreeNode(nullptr, i, idx);
        root->left = l;
        l->parent = root;
        if (flag)
        {
            root->right = new AVLTreeNode(root, idx);
        }
        else
        {
            root->right = new AVLTreeNode(root, idx, i);
            AVLTreeNode *r = root->right;
            r->left = new AVLTreeNode(r, idx);
            r->right = new AVLTreeNode(r, i);
            root->bf = 1;
        }
    }
    else
    {
        AVLTreeNode *r = l->parent;
        bool isleft = r->left == l;
        // replace *l with an internal node
        if (isleft)
        {
            r->left = new AVLTreeNode(r, i, idx);
            r = r->left;
        }
        else
        {
            r->right = new AVLTreeNode(r, i, idx);
            r = r->right;
        }
        // add children to this internal node
        r->left = l;
        l->parent = r;
        r->right = flag ? new AVLTreeNode(r, idx) : new AVLTreeNode(r, idx, i);
        r = r->right;
        // check balance
        AVLTreeNode *p_unbl = find_unbalance(r->parent);
        if (p_unbl)
            rotate_after_insert(p_unbl);
        if (!flag)
        {
            // add children to *r
            r->left = new AVLTreeNode(r, idx);
            r->right = new AVLTreeNode(r, i);
            r = r->left;
            // check balance
            p_unbl = find_unbalance(r->parent);
            if (p_unbl)
                rotate_after_insert(p_unbl);
        }
        return r;
    }
}

/* find the nearest unbalanced node, start from p,
   assume that unbalancy is due to node insertion and *p.bf == 0.
   If find, return that node;
   If not find, return nullptr */
AVLTreeNode *AVLTree::find_unbalance(AVLTreeNode *p)
{
    AVLTreeNode *r = p->parent;
    while (r)
    {
        if (r->left == p)
            r->bf--;
        else
            r->bf++;
        if (r->bf == 2 || r->bf == -2)
            return r;
        if (r->bf == 0)
            break;
        p = r;
        r = p->parent;
    }
    return nullptr;
}

/* remove leaf node *l and merge its previous and next internal nodes.
   assume *l have both previous and next nodes. */
void AVLTree::remove_leaf(AVLTreeNode *l)
{
    int idx = l->idx;
    // modify one internal node
    AVLTreeNode *p = l->parent;
    bool isleft = p->left == l;
    if (isleft)
    {
        AVLTreeNode *prev = get_prev(l);
        prev->idx2 = p->idx2;
    }
    else
    {
        AVLTreeNode *next = get_next(l);
        next->idx = p->idx;
    }
    // delete leaf and the other internal node
    delete l;
    l = isleft ? p->right : p->left;
    AVLTreeNode *r = p->parent;
    l->parent = r;
    isleft = r->left == p;
    delete p;
    if (isleft)
        r->left = l;
    else
        r->right = l;
    // balance
    while (r)
    {
        isleft = r->left == l;
        if (isleft)
        {
            r->bf++;
            if (r->bf == 1)
                break;
            else if (r->bf == 2)
            {
                r = rotate_after_remove(r);
                if (r->bf != 0)
                    break;
            }
        }
        else
        {
            r->bf--;
            if (r->bf == -1)
                break;
            else if (r->bf == -2)
            {
                r = rotate_after_remove(r);
                if (r->bf != 0)
                    break;
            }
        }
        l = r;
        r = l->parent;
    }
}

/* find the previous node (w.r.t in-order traversal) of *l.
   assume *l is a leaf node.
   If *l is the first node, return nullptr. */
AVLTreeNode *AVLTree::get_prev(AVLTreeNode *l) const
{
    while (l != root)
    {
        AVLTreeNode *p = l->parent;
        if (p->right == l)
            return p;
        l = p;
    }
    return nullptr;
}

/* find the next node (w.r.t in-order traversal) of *l.
   assume *l is a leaf node.
   If *l is the last node, return nullptr. */
AVLTreeNode *AVLTree::get_next(AVLTreeNode *l) const
{
    while (l != root)
    {
        AVLTreeNode *p = l->parent;
        if (p->left == l)
            return p;
        l = p;
    }
    return nullptr;
}

/* make rotation: *r is an unbalanced node.
       assume unbalancy is due to node insertion. */
void AVLTree::rotate_after_insert(AVLTreeNode *r)
{
    AVLTreeNode *p = r->parent;
    bool isleft;
    if (p)
        isleft = p->left == r;
    if (r->bf == 2)
    {
        if (r->right->bf == 1)
        {
            r = RR_rotate(r);
            r->bf = 0;
            r->left->bf = 0;
        }
        else
        {
            r = RL_rotate(r);
            r->right->bf = int(r->bf == -1);
            r->left->bf = -int(r->bf == 1);
            r->bf = 0;
        }
    }
    else
    {
        if (r->left->bf == -1)
        {
            r = LL_rotate(r);
            r->bf = 0;
            r->right->bf = 0;
        }
        else
        {
            r = LR_rotate(r);
            r->right->bf = int(r->bf == -1);
            r->left->bf = -int(r->bf == 1);
            r->bf = 0;
        }
    }
    if (!p)
        setRoot(r);
    else
    {
        if (isleft)
            p->left = r;
        else
            p->right = r;
        r->parent = p;
    }
}

/* make rotation: *r is an unbalanced node.
       assume unbalancy is due to node deletion. */
AVLTreeNode *AVLTree::rotate_after_remove(AVLTreeNode *r)
{
    AVLTreeNode *p = r->parent;
    bool isleft;
    if (p)
        isleft = p->left == r;
    if (r->bf == 2)
    {
        if (r->right->bf == 0)
        {
            r = RR_rotate(r);
            r->bf = -1;
            r->left->bf = 1;
        }
        else if (r->right->bf == 1)
        {
            r = RR_rotate(r);
            r->bf = 0;
            r->left->bf = 0;
        }
        else
        {
            r = RL_rotate(r);
            r->right->bf = int(r->bf == -1);
            r->left->bf = -int(r->bf == 1);
            r->bf = 0;
        }
    }
    else
    {
        if (r->left->bf == 0)
        {
            r = LL_rotate(r);
            r->bf = 1;
            r->right->bf = -1;
        }
        else if (r->left->bf == -1)
        {
            r = LL_rotate(r);
            r->bf = 0;
            r->right->bf = 0;
        }
        else
        {
            r = LR_rotate(r);
            r->right->bf = int(r->bf == -1);
            r->left->bf = -int(r->bf == 1);
            r->bf = 0;
        }
    }
    if (!p)
        setRoot(r);
    else
    {
        if (isleft)
            p->left = r;
        else
            p->right = r;
        r->parent = p;
    }
    return r;
}

AVLTreeNode *AVLTree::LL_rotate(AVLTreeNode *r)
{
    AVLTreeNode *p = r->left;
    r->left = p->right;
    p->right->parent = r;
    p->right = r;
    r->parent = p;
    return p;
}

AVLTreeNode *AVLTree::RR_rotate(AVLTreeNode *r)
{
    AVLTreeNode *p = r->right;
    r->right = p->left;
    p->left->parent = r;
    p->left = r;
    r->parent = p;
    return p;
}

AVLTreeNode *AVLTree::LR_rotate(AVLTreeNode *r)
{
    AVLTreeNode *p = r->left;
    AVLTreeNode *q = p->right;
    r->left = q->right;
    q->right->parent = r;
    p->right = q->left;
    q->left->parent = p;
    q->left = p;
    p->parent = q;
    q->right = r;
    r->parent = q;
    return q;
}

AVLTreeNode *AVLTree::RL_rotate(AVLTreeNode *r)
{
    AVLTreeNode *p = r->right;
    AVLTreeNode *q = p->left;
    r->right = q->left;
    q->left->parent = r;
    p->left = q->right;
    q->right->parent = p;
    q->left = r;
    r->parent = q;
    q->right = p;
    p->parent = q;
    return q;
}
