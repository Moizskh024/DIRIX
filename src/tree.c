#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tree_valid_name(const char *name) {
    if (!name || !name[0] || strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        return 0;
    if (strchr(name, '/') || strchr(name, '\\') || strchr(name, '|') ||
        strchr(name, '\n') || strchr(name, '\r'))
        return 0;
    return strlen(name) < DIRIX_NAME_MAX;
}

Node *tree_create_node(const char *name, NodeType type, unsigned long size, Node *parent) {
    Node *node;
    if (!tree_valid_name(name)) return NULL;
    node = (Node *)malloc(sizeof(*node));
    if (!node) return NULL;
    snprintf(node->name, sizeof(node->name), "%s", name);
    node->type = type;
    node->file_size = (type == NODE_FILE) ? size : 0;
    node->parent = parent;
    node->first_child = NULL;
    node->next_sibling = NULL;
    return node;
}

Node *tree_create_root(void) {
    Node *root = (Node *)malloc(sizeof(*root));
    if (!root) return NULL;
    strcpy(root->name, "DIRIX");
    root->type = NODE_DIRECTORY;
    root->file_size = 0;
    root->parent = NULL;
    root->first_child = NULL;
    root->next_sibling = NULL;
    return root;
}

Node *tree_find_child(Node *directory, const char *name) {
    Node *current;
    if (!directory || directory->type != NODE_DIRECTORY) return NULL;
    for (current = directory->first_child; current; current = current->next_sibling)
        if (strcmp(current->name, name) == 0) return current;
    return NULL;
}

int tree_add_child(Node *parent, Node *child) {
    Node *last;
    if (!parent || parent->type != NODE_DIRECTORY || !child) return 0;
    if (tree_find_child(parent, child->name)) return 0;
    child->parent = parent;
    child->next_sibling = NULL;
    if (!parent->first_child) parent->first_child = child;
    else {
        last = parent->first_child;
        while (last->next_sibling) last = last->next_sibling;
        last->next_sibling = child;
    }
    return 1;
}

Node *tree_resolve_directory(Node *root, const char *path) {
    char copy[DIRIX_PATH_MAX];
    char *token;
    Node *current = root;
    if (!root || !path || path[0] != '/') return NULL;
    if (strcmp(path, "/") == 0) return root;
    if (strlen(path) >= sizeof(copy)) return NULL;
    strcpy(copy, path);
    token = strtok(copy, "/");
    while (token) {
        current = tree_find_child(current, token);
        if (!current || current->type != NODE_DIRECTORY) return NULL;
        token = strtok(NULL, "/");
    }
    return current;
}

Node *tree_search_name(Node *root, const char *name) {
    Node *child, *found;
    if (!root) return NULL;
    if (strcmp(root->name, name) == 0) return root;
    for (child = root->first_child; child; child = child->next_sibling) {
        found = tree_search_name(child, name);
        if (found) return found;
    }
    return NULL;
}

void tree_print_path(Node *node, char *buffer, size_t size) {
    const char *parts[128];
    size_t count = 0, used = 0;
    if (!node || node->parent == NULL) {
        snprintf(buffer, size, "/");
        return;
    }
    while (node && node->parent && count < 128) {
        parts[count++] = node->name;
        node = node->parent;
    }
    if (size) buffer[0] = '\0';
    while (count > 0 && used < size) {
        int n = snprintf(buffer + used, size - used, "/%s", parts[--count]);
        if (n < 0 || (size_t)n >= size - used) break;
        used += (size_t)n;
    }
    if (used == 0 && size) snprintf(buffer, size, "/");
}

int tree_rename(Node *node, const char *new_name) {
    if (!node || !node->parent || !tree_valid_name(new_name)) return 0;
    if (strcmp(node->name, new_name) == 0) return 1;
    if (tree_find_child(node->parent, new_name)) return 0;
    snprintf(node->name, sizeof(node->name), "%s", new_name);
    return 1;
}

static int is_descendant(Node *possible_child, Node *ancestor) {
    Node *p;
    for (p = possible_child; p; p = p->parent)
        if (p == ancestor) return 1;
    return 0;
}

static int detach(Node *node) {
    Node *p, *cur, *prev = NULL;
    if (!node || !(p = node->parent)) return 0;
    for (cur = p->first_child; cur && cur != node; cur = cur->next_sibling)
        prev = cur;
    if (!cur) return 0;
    if (prev) prev->next_sibling = node->next_sibling;
    else p->first_child = node->next_sibling;
    node->parent = NULL;
    node->next_sibling = NULL;
    return 1;
}

int tree_move(Node *node, Node *new_parent) {
    Node *old_parent, *cur, *prev = NULL;
    if (!node || !new_parent || !node->parent || new_parent->type != NODE_DIRECTORY)
        return 0;
    if (is_descendant(new_parent, node)) return 0;
    if (tree_find_child(new_parent, node->name)) return 0;
    if (node->parent == new_parent) return 1;
    old_parent = node->parent;
    for (cur = old_parent->first_child; cur && cur != node; cur = cur->next_sibling)
        prev = cur;
    if (!cur) return 0;
    if (prev) prev->next_sibling = node->next_sibling;
    else old_parent->first_child = node->next_sibling;
    node->parent = new_parent;
    node->next_sibling = NULL;
    if (!new_parent->first_child) new_parent->first_child = node;
    else {
        cur = new_parent->first_child;
        while (cur->next_sibling) cur = cur->next_sibling;
        cur->next_sibling = node;
    }
    return 1;
}

void tree_free(Node *node) {
    Node *child, *next;
    if (!node) return;
    child = node->first_child;
    while (child) {
        next = child->next_sibling;
        tree_free(child);
        child = next;
    }
    free(node);
}

int tree_delete(Node *node) {
    if (!node || !node->parent || !detach(node)) return 0;
    tree_free(node);
    return 1;
}

void tree_display(Node *node, int depth) {
    Node *child;
    int i;
    if (!node) return;
    for (i = 0; i < depth; ++i) printf("    ");
    if (!node->parent) printf("[ROOT] %s/\n", node->name);
    else if (node->type == NODE_DIRECTORY) printf("[DIR]  %s/\n", node->name);
    else printf("[FILE] %s (%lu bytes)\n", node->name, node->file_size);
    for (child = node->first_child; child; child = child->next_sibling)
        tree_display(child, depth + 1);
}

void tree_statistics(Node *node, size_t *directories, size_t *files, unsigned long *bytes) {
    Node *child;
    if (!node) return;
    if (node->type == NODE_DIRECTORY) ++(*directories);
    else {
        ++(*files);
        *bytes += node->file_size;
    }
    for (child = node->first_child; child; child = child->next_sibling)
        tree_statistics(child, directories, files, bytes);
}
