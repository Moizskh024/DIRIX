#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Preorder text format: D|path|0 or F|path|size. Names cannot contain '|'. */
static int save_nodes(FILE *fp, Node *node, const char *parent_path) {
    Node *child;
    char path[DIRIX_PATH_MAX];
    if (!node) return 1;
    if (node->parent == NULL) snprintf(path, sizeof(path), "/");
    else if (strcmp(parent_path, "/") == 0)
        snprintf(path, sizeof(path), "/%s", node->name);
    else
        snprintf(path, sizeof(path), "%s/%s", parent_path, node->name);

    if (node->parent != NULL) {
        if (fprintf(fp, "%c|%s|%lu\n",
                    node->type == NODE_DIRECTORY ? 'D' : 'F',
                    path, node->file_size) < 0) return 0;
    }
    for (child = node->first_child; child; child = child->next_sibling)
        if (!save_nodes(fp, child, path)) return 0;
    return 1;
}

int storage_save(Node *root, const char *filename) {
    FILE *fp;
    int ok;
    if (!root || !filename) return 0;
    fp = fopen(filename, "w");
    if (!fp) return 0;
    ok = save_nodes(fp, root, "/");
    if (fclose(fp) != 0) ok = 0;
    return ok;
}

Node *storage_load(const char *filename) {
    FILE *fp = fopen(filename, "r");
    Node *root;
    char line[DIRIX_PATH_MAX + 64];
    if (!fp) return NULL;
    root = tree_create_root();
    if (!root) { fclose(fp); return NULL; }

    while (fgets(line, sizeof(line), fp)) {
        char kind, path[DIRIX_PATH_MAX], *last;
        unsigned long size;
        char *p1 = strchr(line, '|'), *p2;
        Node *parent, *node;
        char parent_path[DIRIX_PATH_MAX];

        if (!p1) continue;
        *p1 = '\0';
        kind = line[0];
        p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        snprintf(path, sizeof(path), "%s", p1 + 1);
        size = strtoul(p2 + 1, NULL, 10);

        last = strrchr(path, '/');
        if (!last || last[1] == '\0') continue;
        char item_name[DIRIX_NAME_MAX];
        snprintf(item_name, sizeof(item_name), "%s", last + 1);
        if (last == path) strcpy(parent_path, "/");
        else { *last = '\0'; snprintf(parent_path, sizeof(parent_path), "%s", path); }

        parent = tree_resolve_directory(root, parent_path);
        if (!parent || tree_find_child(parent, item_name)) continue;
        node = tree_create_node(item_name,
                 kind == 'D' ? NODE_DIRECTORY : NODE_FILE, size, parent);
        if (node && !tree_add_child(parent, node)) free(node);
    }
    fclose(fp);
    return root;
}
