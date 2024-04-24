#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t* nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = 1;
  nil->left = nil;
  nil->right = nil;
  p->root = nil;
  p->nil = nil;
  nil->left = nil->right = nil;
  return p;
}

void RecursiveDelete(rbtree *t, node_t* node) {
  if(t == NULL) return;
  if(node == t->nil)  return;
  if(node->left != t->nil)  RecursiveDelete(t, node->left);
  if(node->right != t->nil) RecursiveDelete(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  RecursiveDelete(t, t->root);
  free(t->nil);
  free(t);
}

void rotate_left(rbtree *t, node_t* x) {
  node_t* y = x->right;
  node_t* nil = t->nil;
  x->right = y->left;
  if(y->left != nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == nil) {
    t->root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void rotate_right(rbtree *t, node_t* y) {
  node_t* x = y->left;
  node_t* nil = t->nil;
  y->left = x->right;
  if(x->right != nil) {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if(y->parent == nil) {
    t->root = x;
  }
  else if(y == y->parent->right) {
    y->parent->right = x;
  }
  else {
    y->parent->left = x;
  }
  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree* t, node_t* z) {
  while(z->parent->color == 0) {                  // z의 색 : 빨강, 부모의 색 : 빨강 인 동안
    if(z->parent == z->parent->parent->left) {
      node_t* y = z->parent->parent->right;
      if(y->color == 0) {                         // case 1
        z->parent->color = 1;
        y->color = 1;
        z->parent->parent->color = 0;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->right) {               // case 2
          z = z->parent;
          rotate_left(t, z);
        }
        z->parent->color = 1;                     // case 3
        z->parent->parent->color = 0;
        rotate_right(t, z->parent->parent);
      }
    }
    else {
      node_t* y = z->parent->parent->left;
      if(y->color == 0) {                         // case 1
        z->parent->color = 1;
        y->color = 1;
        z->parent->parent->color = 0;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->left) {                // case 2
            z = z->parent;
            rotate_right(t, z);
        }
          z->parent->color = 1;                   // case 3
          z->parent->parent->color = 0;
          rotate_left(t, z->parent->parent);
      }
    }
  }
  t->root->color = 1;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t* newNode = (node_t*)malloc(sizeof(node_t));
  node_t* nil = t->nil;
  newNode->key = key;
  newNode->left = nil;
  newNode->right = nil;
  newNode->color = 0;

  node_t* parNode = nil;
  node_t* curNode = t->root;
  // find location to insert
  while(curNode != nil) {
    parNode = curNode;
    if(curNode->key > key) {
      curNode = curNode->left;
    }
    else{
      curNode = curNode->right;
    }
  }
  newNode->parent = parNode;
  if(parNode == nil) {
    t->root = newNode;
  }
  else if(parNode->key > key) {
    parNode->left = newNode;
  }
  else {
    parNode->right = newNode;
  }
  // balancing
  rbtree_insert_fixup(t, newNode);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *curNode = t->root;
  while(curNode != t->nil) {
    if(curNode->key == key) {
      return curNode;
    }
    else if(curNode->key > key) {
      curNode = curNode->left;
    }
    else {
      curNode = curNode->right;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *curNode = t->root;
  node_t *nil = t->nil;
  if(curNode == nil) {
    return nil;
  }
  while(curNode->left != nil) {
    curNode = curNode->left;
  }
  return curNode;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *curNode = t->root;
  node_t *nil = t->nil;
  if(curNode == nil) {
    return nil;
  }
  while(curNode->right != nil) {
    curNode = curNode->right;
  }
  return curNode;
}

void transplant(rbtree* t, node_t* u, node_t* v) {
  if(u->parent == t->nil) {
    t->root = v;
  }
  else if(u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree* t, node_t* x) {
  while(x != t->root && x->color == 1) {
    if(x == x->parent->left) {
      node_t* w = x->parent->right;
      if(w->color == 0) {                         // Case 1
        w->color = 1;
        x->parent->color = 0;
        rotate_left(t, x->parent);
        w = x->parent->right;
      }
      if(w->left->color == 1 && w->right->color == 1) {   // Case 2
        w->color = 0;
        x = x->parent;
      }
      else {
        if(w->right->color == 1) {                  // Case 3
          w->left->color = 1;
          w->color = 0;
          rotate_right(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;                // Case 4
        x->parent->color = 1;
        w->right->color = 1;
        rotate_left(t, x->parent);
        x = t->root;
      }
    }
    else {
      node_t* w = x->parent->left;
      if(w->color == 0) {                         // Case 1
        w->color = 1;
        x->parent->color = 0;
        rotate_right(t, x->parent);
        w = x->parent->left;
      }
      if(w->right->color == 1 && w->left->color == 1) {   // Case 2
        w->color = 0;
        x = x->parent;
      }
      else {
        if(w->left->color == 1) {                  // Case 3
          w->right->color = 1;
          w->color = 0;
          rotate_left(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;                // Case 4
        x->parent->color = 1;
        w->left->color = 1;
        rotate_right(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = 1;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t* y = p;
  node_t* x;
  int y_origin_color = y->color;
  if(p->left == t->nil) {                 // 삭제하려는 노드의 왼쪽이 nil이므로
    x = p->right;
    transplant(t, p, p->right);           // 삭제하려는 노드의 부모와 오른쪽 자식을 이어줌
  }
  else if(p->right == t->nil) {           // 삭제하려는 노드의 오른쪽이 nil이므로
    x = p->left;
    transplant(t, p, p->left);            // 삭제하려는 노드의 부모와 왼쪽 자식을 이어줌
  }
  else {                                  // 삭제하려는 노드의 양자식이 모두 있는 경우
    node_t* temp_root = t->root;
    t->root = p->right;
    y = rbtree_min(t);                    // predecessor을 찾음
    t->root = temp_root;
    y_origin_color = y->color;
    x = y->right;
    if(y->parent == p) {                  // predecessor가 삭제하려는 노드의 자식인 경우
      x->parent = y;
    }
    else {                                // predecessor가 삭제하려는 노드와 떨어져있는 경우
      transplant(t, y, y->right);         // predecessor의 부모와 그의 자식을 연결
      y->right = p->right;                // predecessor의 오른쪽 자식을 삭제하려는 노드의 오른쪽 자식으로
      y->right->parent = y;               // 오른쪽 자식의 부모를 predecessor로
    }
    transplant(t, p, y);                  // 삭제하려는 노드의 부모와 predecessor을 연결
    y->left = p->left;                    // 삭제하려는 노드의 왼쪽 자식을 predecessor의 왼쪽 자식으로
    y->left->parent = y;                  // 왼쪽 자식의 부모를 predecessor로
    y->color = p->color;                  // color도 삭제하려는 노드의 색으로
  }
  if(y_origin_color == 1) {               // 삭제되는 색이 BLACK인 경우 rebalancing이 필요함
    rbtree_delete_fixup(t, x);
  }
  free(p);
  return 0;
}

void fillarr(node_t* nil, node_t* cur, key_t *arr, int* filled, const size_t n) {
  if(*filled == n)  return;
  if(cur->left != nil) {
    fillarr(nil, cur->left, arr, filled, n);
  }
  arr[*filled] = cur->key;
  (*filled)++;
  if(cur->right != nil) {
    fillarr(nil, cur->right, arr, filled, n);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int filled = 0;
  fillarr(t->nil, t->root, arr, &filled, n);
  return 0;
}