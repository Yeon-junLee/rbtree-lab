#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root = NULL;
  // node_t* nil = (node_t *)calloc(1, sizeof(node_t));
  // nil->color = 1;
  // nil->key = -9999999999;
  // nil->parent = NULL;
  // nil->left = NULL;
  // nil->right = NULL;
  p->nil = NULL;
  return p;
}

void RecursiveDelete(node_t* node) {
  if(node == NULL)  return;
  if(node->left != NULL)  RecursiveDelete(node->left);
  if(node->right != NULL) RecursiveDelete(node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  RecursiveDelete(t->root);
  free(t);
}

node_t* rotate_left(rbtree *t, node_t* x) {
  node_t* y = x->right;
  x->right = y->right;
  if(y->left != NULL) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == NULL) {
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

  return y;
}

node_t* rotate_right(rbtree *t, node_t* x) {
  node_t* x = y->left;
  y->left = x->left;
  if(x->right != NULL) {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if(y->parent == NULL) {
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

  return x;
}

void rbtree_insert_fixup(rbtree* t, node_t* z) {
  while(z->parent->color == 0) {
    if(z->parent == z->parent->parent->left) {
      node_t* y = z->parent->parent->right;
      if(y->color == 0) {
        z->parent->color = 1;
        y->color = 1;
        z->parent->parent->color = 0;
        z = z->parent->parent;
      }
      else if(z == z->parent->right) {
          z = z->parent;
          rotate_left(t, z);
        z->parent->color = 1;
        z->parent->parent->color = 0;
        rotate_right(t, z->parent->parent);
      }
    }
    else {
      
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t* newNode = (node_t*)malloc(sizeof(node_t));
  newNode->color = 0;
  newNode->key = key;
  newNode->parent = NULL;
  newNode->left = t->nil;
  newNode->right = t->nil;

  node_t parNode = NULL;
  node_t curNode = t->root;
  // find location to insert
  while(curNode != NULL) {
    parNode = curNode;
    if(curNode->key >= key) {
      curNode = curNode->left;
    }
    else{
      curNode = curNode->right;
    }
  }
  newNode->parent = parNode;
  if(parNode == NULL) {
    t->root = curNode;
  }
  else if(parNode->key > key) {
    parNode->left = curNode;
  }
  else {
    parNode->right = curNode;
  }
  curNode->parent = parNode;
  
  rbtree_insert_fixup(t, newNode);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
