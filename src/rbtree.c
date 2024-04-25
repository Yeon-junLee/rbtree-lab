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

void rbtree_insert_fixup(rbtree* t, node_t* z) {  // z : 삽입된 노드
  while(z->parent->color == 0) {                  // z의 색 : 빨강, 부모의 색 : 빨강 인 동안
    if(z->parent == z->parent->parent->left) {    // z의 부모가 할아버지의 왼쪽 자식일 때
      node_t* y = z->parent->parent->right;       // y : z의 삼촌 노드
      if(y->color == 0) {                         // case 1 : 삼촌 노드가 빨간색인 경우
        z->parent->color = 1;                     // 부모를 black으로
        y->color = 1;                             // 삼촌도 black으로
        z->parent->parent->color = 0;             // 할아버지를 red로 바꾼 뒤
        z = z->parent->parent;                    // 할아버지 단에서 해결하도록 넘김
      }
      else {
        if(z == z->parent->right) {               // case 2 : 삽입된 노드가 할아버지 => 부모 => 삽입된 노드로 일직선이 아니라 꺾여있는 경우
          z = z->parent;                          // rotate와 balancing의 기준점을 부모로 바꾼 뒤
          rotate_left(t, z);                      // 부모 기준으로 left rotate(일자로 펴주는 작업)
        }
                                                  // case 3 : 삽입된 노드가 일직선으로 이어진 경우
        z->parent->color = 1;                     // 부모의 색을 black으로
        z->parent->parent->color = 0;             // 조부모의 색을 red로
        rotate_right(t, z->parent->parent);       // 조부모를 기준으로 right rotate(5번 조건을 맞추기 위해 black으로 바꾼 부모를 위로 올림)
      }
    }
    // 이하 동일(방향만 반대)
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
  // insertion
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

void transplant(rbtree* t, node_t* u, node_t* v) {                  // u의 부모와 v를 이어주는 과정(삭제할 때 대체하는 노드를 올리는 작업)
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
  while(x != t->root && x->color == 1) {          // 삭제된 자리가 root가 아니고 블랙인 경우에만(삭제된 색이 red면 그대로 삭제하고 끝)
    if(x == x->parent->left) {                    // 삭제된 자리가 부모의 왼쪽에 위치할 때
      node_t* w = x->parent->right;               // w : 삭제된 자리의 형제 노드
      if(w->color == 0) {                         // Case 1 : 형제가 red일 때
        w->color = 1;                             // 형제의 색을 black으로
        x->parent->color = 0;                     // 부모의 색을 red로
        rotate_left(t, x->parent);                // 부모를 기준으로 left rotate하여 doubly black의 형제를 black으로 만들어 case 2, 3, 4중 하나로 만듦
        w = x->parent->right;                     // rotate후 바뀐 black이 된 형제를 다시 가리킴
      }
      if(w->left->color == 1 && w->right->color == 1) {   // Case 2 : 형제의 색이 black이고 형제의 양 자식의 색이 black일 때
        w->color = 0;                                     // 형제의 색을 red로 만들어 doubly black을 부모에게 올림
        x = x->parent;                                    // case를 부모로 옮김
      }
      else {
        if(w->right->color == 1) {                  // Case 3 : doubly black의 형제가 black, doubly black과 가까운 조카 노드(반대 방향 자식이 아님)가 red인 경우
          w->left->color = 1;                       // 해당 조카 노드의 색을 black으로
          w->color = 0;                             // 형제 노드의 색을 red로
          rotate_right(t, w);                       // 형제 노드를 기준으로 right rotate하여 case 4로 만듦
          w = x->parent->right;                     // 형제 노드 갱신
        }
        w->color = x->parent->color;                // Case 4 : doubly black의 형제가 black, doubly black과 먼 쪽의 조카(정방향 자식)가 red인 경우
        x->parent->color = 1;                       // 부모의 색을 black으로
        w->right->color = 1;                        // 형제의 오른쪽 자식을 black으로
        rotate_left(t, x->parent);                  // 부모를 기준으로 left rotate하여 red가 doubly black 위로 오게 만든 뒤, red를 black으로 만드는 과정이 위 포함임
        x = t->root;                                // delete rebalancing 기준을 root로 바꿈(while문 탈출 조건 성립)
      }
    }
    // 이하 위와 동일(방향만 반대)
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
      x->parent = y;                      // x가 nil인 경우 delete_fixup에서 부모를 설정 안하면 segmentation fault가 일어남(x의 부모를 참조하는 경우 초기화 안하면 NULL을 참조)
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
    rbtree_delete_fixup(t, x);            // delete rebalancing
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
  int filled = 0;                           // 현재까지 arr에 들어간 원소의 개수
  fillarr(t->nil, t->root, arr, &filled, n);
  return 0;
}