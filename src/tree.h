#ifndef DIRIX_TREE_H
#define DIRIX_TREE_H

#include <stddef.h>

#define DIRIX_NAME_MAX 64
#define DIRIX_PATH_MAX 512

typedef enum {
    NODE_DIRECTORY = 1,
    NODE_FILE = 2
} NodeType;

typedef struct Node {
    char name[DIRIX_NAME_MAX];
    NodeType type;
    unsigned long file_size;
    struct Node *parent;
    struct Node *first_child;
    struct Node *next_sibling;
} Node;

Node *tree_create_root(void);
Node *tree_create_node(const char *name, NodeType type, unsigned long size, Node *parent);
Node *tree_find_child(Node *directory, const char *name);
Node *tree_resolve_directory(Node *root, const char *path);
Node *tree_search_name(Node *root, const char *name);
int tree_add_child(Node *parent, Node *child);
int tree_rename(Node *node, const char *new_name);
int tree_move(Node *node, Node *new_parent);
int tree_delete(Node *node);
void tree_display(Node *node, int depth);
void tree_print_path(Node *node, char *buffer, size_t size);
void tree_statistics(Node *node, size_t *directories, size_t *files, unsigned long *bytes);
void tree_free(Node *node);
int tree_valid_name(const char *name);

#endif
