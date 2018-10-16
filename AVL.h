#ifndef AVL_H
#define AVL_H

/* head file for class AVLTree:
   This is a modified AVL Tree used to represent the current "beach line":
   1. leaf nodes represent "arcs";
   2. internal nodes represent "break points".
   Some properties:
   1. every internal node has both left and right child;
   2. insertion is only available at some leaf node: Replace the leaf with 
      (internal(leaf1)(leaf2)) or (internal1(leaf1)(internal2(leaf2)(leaf3))))
   3. deletion is only available at some leaf node: Delete the leaf, 
      then merge its previous node and its next node */

#include "geom_utils.h"
#include <vector>

struct AVLTreeNode
{
  int idx;             // index of the corresponding site in some list/vector, if it is a leaf node;
                       // index of the left-side arc at this break point, if it is an internal node
  int idx2;            // -1, if it is a leaf node;
                       // index of the right-side arc at this break point, if it is an internal node
  int bf;              // balance factor
  AVLTreeNode *left;   // left child
  AVLTreeNode *right;  // right child
  AVLTreeNode *parent; // parent node
  AVLTreeNode(AVLTreeNode *prnt, int idx, int idx2 = -1)
      : idx(idx), idx2(idx2), bf(0), left(nullptr), right(nullptr), parent(prnt) {}
  inline bool isLeaf();
};

class AVLTree
{
private:
  AVLTreeNode *root;          // root node
  const vector<Point> &sites; // reference to an outer list of sites

public:
  explicit AVLTree(const vector<Point> &sites)
      : root(nullptr), sites(sites) {}
  ~AVLTree();
  inline bool isEmpty();
  //void insert_site(int idx);
  AVLTreeNode *get_leaf(int idx);
  AVLTreeNode *repl_leaf(AVLTreeNode *l, int idx, bool flag = false);
  void remove_leaf(AVLTreeNode *l);
  void insert_topest_sites(const std::vector<int> &ids);

private:
  void setRoot(AVLTreeNode *r);
  void del_subtree(AVLTreeNode *r);                           // used in desctructor
  inline AVLTreeNode *descend(AVLTreeNode *p, int idx) const; // used in insert_leaf
  AVLTreeNode *find_unbalance(AVLTreeNode *p);
  void rotate_after_insert(AVLTreeNode *r);
  AVLTreeNode *rotate_after_remove(AVLTreeNode *r);
  AVLTreeNode *LL_rotate(AVLTreeNode *r);
  AVLTreeNode *RR_rotate(AVLTreeNode *r);
  AVLTreeNode *LR_rotate(AVLTreeNode *r);
  AVLTreeNode *RL_rotate(AVLTreeNode *r);
  AVLTreeNode *get_prev(AVLTreeNode *l) const;
  AVLTreeNode *get_next(AVLTreeNode *l) const;
};

#endif