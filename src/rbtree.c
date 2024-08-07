#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void printTree(rbtree *t, node_t *node)
{
  if (t == NULL || node == t->nil)
    return;

  printf("%d(%d) ", node->key, node->color);
  printTree(t, node->left);
  printTree(t, node->right);
}

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
  if (node->left != t->nil) // L자식이 있다면
  {
    delete_node(t, node->left); // L자식을 인자로 재귀
  }
  if (node->right != t->nil) // R자식이 있다면
  {
    delete_node(t, node->right); // R자식을 인자로 재귀
  }
  if (node != t->nil) // 노드가 비어있지 않다면
  {
    free(node);
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  // 새로 넣을 노드 정의
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  // z->parent = NULL;

  // 삽입 위치 찾가
  node_t *y = t->nil;
  node_t *x = t->root;
  // printf("insert start\n");
  while (x != t->nil) // 단말노드까지 반복
  {
    y = x; // 변경되기전 x의 값(부모)를 저장
    if (z->key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  // printf("escape loop\n");
  z->parent = y;
  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }
  z->color = RBTREE_RED; // 삽입 노드는 항상 red
  z->left = t->nil;
  z->right = t->nil;
  RB_Insert_Fixup(t, z); // 규칙 위반 체크
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (x->key == key)
    {
      return x;
    }
    else if (x->key > key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return find_min(t, t->root);
  // printf("*rbtree_min end\n");
}

node_t *find_min(const rbtree *t, node_t *sub_root)
{
  node_t *cur = sub_root;
  while (cur->left != t->nil)
  {
    cur = cur->left;
  }
  // printf("*find_min end\n");
  return cur;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return find_max(t, t->root);
}

node_t *find_max(const rbtree *t, node_t *sub_root)
{
  node_t *cur = sub_root;
  while (cur->right != t->nil)
  {
    cur = cur->right;
  }
  return cur;
}

int rbtree_erase(rbtree *t, node_t *z)
{
  // TODO: implement erase
  node_t *y = z;
  color_t y_origin_color = y->color;
  node_t *x = NULL;
  // printf("erase s\n");
  if (z->left == t->nil)
  {
    x = z->right;
    RB_Transplant(t, z, z->right);
    // printf("erase 1\n");
  }
  else if (z->right == t->nil)
  {
    x = z->left;
    RB_Transplant(t, z, z->left);
    // printf("erase 2\n");
  }
  else
  {
    y = find_min(t, z->right);
    // printf("erase 3\n");
    y_origin_color = y->color;
    x = y->right;

    if (y->parent == z)
    {
      x->parent = y;
    }
    else
    {
      RB_Transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    RB_Transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_origin_color == RBTREE_BLACK)
  {
    RB_Delete_Fixup(t, x);
  }
  free(z);
  return 0;
}

void RB_Delete_Fixup(rbtree *t, node_t *x)
{
  // printf("Fixup s\n");
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    node_t *w;
    if (x == x->parent->left)
    {
      // printf("Fixup -!!\n");
      w = x->parent->right;
      // printf("Fixup -!!!\n");

      if (w->color == RBTREE_RED)
      {
        // printf("Fixup -!!!!\n");
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        // printf("Fixup -1\n");
        Left_Rotate(t, x->parent);
        // printf("Fixup 1\n");
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        // printf("Fixup -!!!!\n");
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          // printf("Fixup -!!!!\n");
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          // printf("Fixup -2\n");
          Right_Rotate(t, w);
          // printf("Fixup 2\n");
          w = x->parent->right;
        }

        // printf("Fixup -!!!!\n");
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        // printf("Fixup -3\n");
        Left_Rotate(t, x->parent);
        // printf("Fixup 3\n");
        x = t->root;
      }
    }
    else
    {
      // 반대
      w = x->parent->left;

      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        // printf("Fixup -4\n");
        Right_Rotate(t, x->parent);
        // printf("Fixup 4\n");
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          // printf("Fixup -5\n");
          Left_Rotate(t, w);
          // printf("Fixup 5\n");
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        // printf("Fixup -6\n");
        Right_Rotate(t, x->parent);
        // printf("Fixup 6\n");
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
  // printf("Fixup end\n");
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
int inorder(node_t *cur, key_t *arr, const rbtree *t, int i, size_t n)
{
  if (i < n)
  {
    if (cur->left != t->nil)
    {
      i = inorder(cur->left, arr, t, i, n);
    }
    arr[i] = cur->key;
    i++;
    if (cur->right != t->nil)
    {
      i = inorder(cur->right, arr, t, i, n);
    }
  }
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  if (!inorder(t->root, arr, t, 0, n))
  {
    return 1;
  }
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
  x->left = y->right;
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
  // printf("fixup start\n");
  while (z->parent->color == RBTREE_RED) // z의 부모가 빨갱이일때
  {
    if (z->parent == z->parent->parent->left)
    {
      // printf("if\n");
      node_t *y = z->parent->parent->right; // 삼촌노드 y
      if (y->color == RBTREE_RED)           // Case: 1 삼촌노드 red
      {
        // printf("if 2\n");
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else // case: 2 삼촌이 검정이고 z가 부모의 오른쪽 자식인 경우
      {
        if (z == z->parent->right)
        {
          // printf("if 3\n");
          z = z->parent;
          Left_Rotate(t, z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        Right_Rotate(t, z->parent->parent);
      }
    }
    else
    {
      // printf("else\n");
      // 반대
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        // printf("else 2\n");
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          // printf("else 3\n");
          z = z->parent;
          Right_Rotate(t, z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        Left_Rotate(t, z->parent->parent);
      }
    }
  }
  // printf("fixup end\n");
  t->root->color = RBTREE_BLACK; // 루트는 항상 BLACK
}