#ifndef NODE_H
#define NODE_H

#include "glib.h"
#define Elem_t char*
#define FMT "\"%s\""
#define PZ ""

typedef enum
{
    LEFT = -1,
    CURRENT = 0,
    RIGHT = 1,
} SIDE;

typedef struct _node_t
{
    Elem_t val;
    struct _node_t* left;
    struct _node_t* right;
} Node_t;

Node_t* _node_create();
int _node_init(Node_t* node, Elem_t val);
Node_t* _node_insert(Node_t* cur_node, Elem_t val, int (*cmp)(void* a, void* b));
int _node_link(Node_t* parent, Node_t* child, SIDE side);      
int _node_clear(Node_t* node);
int _node_destroy(Node_t* node);

#endif