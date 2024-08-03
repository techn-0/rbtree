#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed1
  // 닐노드 생성 및 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  // tree의 닐과 루트를 닐 노드로 설정
  p->root = nil;
  p->nil = nil;
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  node_t *node = t->root;
  if (node != t->nil) // 비어있지 않다면
  {
    // 자녀노드 주소 삭제하는 함수 실행
    delete_node(t, node);
  }

  free(t->nil);
  free(t);
}
// 트리 삭제 보조 함수
void delete_node(rbtree *t, node_t *node)
{
  if (node->left != t->nil)
  {
    delete_node(t, node->left);
  }
  if (node->right != t->nil)
  {
    delete_node(t, node->right);
  }
  free(node);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  // 새로 넣을 노드 정의
  node_t *z;
  z->color = RBTREE_RED; // 삽입 노드는 항상 red
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;

  // 삽입 위치 찾가
  node_t *y = t->nil;
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;
  RB_Insert_Fixup(t, z); // 규칙 위반 체크
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *y = p;
  node_t *x;
  color_t y_origin_color = y->color;
  if (p->left == t->nil)
  {
    x = p->right;
    RB_Transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    RB_Transplant(t, p, p->left);
  }
  else
  {
    y = rbtree_min(p->right);
    y_origin_color = y->color;
    x = y->right;
    if (y->parent == p)
      x->parent = y;
    else
    {
      RB_Transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    RB_Transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_origin_color == RBTREE_BLACK)
    RB_Delete_Fixup(t, x);
  return 0;
}
void RB_Delete_Fixup(rbtree *t, node_t *x)
{
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x->parent->left)
    {
      w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        Left_Rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else if (w->right->color == RBTREE_BLACK)
      {
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        Right_Rotate(t, w);
        w = x->parent->right;
      }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->right->color = RBTREE_BLACK;
      Left_Rotate(t, x->parent);
      x = t->root;
    }
    else
    {
      1;
    }
  }
  x->color = RBTREE_BLACK;
}

void RB_Transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}

void Left_Rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent; // x의 부모를 y로
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x; // x를 y의 L로
  x->parent = y;
}

void Right_Rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->left;
  if (y->right != t->nil)
    y->right->parent = x;
  y->parent = x->parent; // y의 부모를 x로
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;
  y->right = x; // x를 y의 R로
  x->parent = y;
}

void RB_Insert_Fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED) // z의 부모가 빨갱이일때
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *y = z->parent->parent->right; // 삼촌노드 y
      if (y->color == RBTREE_RED)           // Case: 1 삼촌노드 red(이중빨갱이 사태)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (z == z->parent->right) // case: 2 삼촌이 검정이고 z가 부모의 오른쪽 자식인 경우
      {
        z == z->parent;
        Left_Rotate(t, z);
      }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      Right_Rotate(t, z->parent->parent);
    }
    else
    {
      // 반대
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (z == z->parent->left)
      {
        z == z->parent;
        Right_Rotate(t, z);
      }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      Left_Rotate(t, z->parent->parent);
    }
  }
  t->root->color = RBTREE_BLACK; // 루트는 항상 BLACK
}