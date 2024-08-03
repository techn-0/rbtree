#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum
{
  RBTREE_RED,
  RBTREE_BLACK
} color_t;

typedef int key_t;

typedef struct node_t
{
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct
{
  node_t *root;
  node_t *nil; // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

// 추가
void printTree(rbtree *, node_t *);

void delete_node(rbtree *t, node_t *node);
void RB_Delete_Fixup(rbtree *t, node_t *x);
void RB_Transplant(rbtree *t, node_t *u, node_t *v);
void Left_Rotate(rbtree *t, node_t *x);
void Right_Rotate(rbtree *t, node_t *x);
void RB_Insert_Fixup(rbtree *t, node_t *z);
node_t *find_min(const rbtree *t, node_t *sub_root);
#endif // _RBTREE_H_
