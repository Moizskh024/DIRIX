#include "tree.h"
#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_FILE "data/directory_data.txt"

static void read_line(const char *prompt, char *out, size_t cap) {
    printf("%s", prompt);
    if (!fgets(out, (int)cap, stdin)) { out[0] = '\0'; return; }
    out[strcspn(out, "\n")] = '\0';
}

static int read_int(const char *prompt) {
    char line[40], *end;
    long value;
    read_line(prompt, line, sizeof(line));
    value = strtol(line, &end, 10);
    while (*end && isspace((unsigned char)*end)) ++end;
    return (end == line || *end) ? -1 : (int)value;
}

static Node *get_item(Node *root, const char *path) {
    char copy[DIRIX_PATH_MAX], *last;
    Node *parent, *result;
    char parent_path[DIRIX_PATH_MAX];
    if (!path || path[0] != '/' || strcmp(path, "/") == 0 ||
        strlen(path) >= sizeof(copy)) return NULL;
    strcpy(copy, path);
    last = strrchr(copy, '/');
    if (!last || !last[1]) return NULL;
    if (last == copy) strcpy(parent_path, "/");
    else { *last = '\0'; strcpy(parent_path, copy); }
    parent = tree_resolve_directory(root, parent_path);
    if (!parent) return NULL;
    result = tree_find_child(parent, last == copy ? last + 1 : last + 1);
    return result;
}

static void add_item(Node *root, NodeType type) {
    char parent_path[DIRIX_PATH_MAX], name[DIRIX_NAME_MAX], path[DIRIX_PATH_MAX];
    unsigned long size = 0;
    Node *parent, *node;
    read_line("Parent directory path ( / for root ): ", parent_path, sizeof(parent_path));
    parent = tree_resolve_directory(root, parent_path);
    if (!parent) { puts("Directory not found."); return; }
    read_line("Name: ", name, sizeof(name));
    if (!tree_valid_name(name) || tree_find_child(parent, name)) {
        puts("Invalid name or an item with this name already exists here."); return;
    }
    if (type == NODE_FILE) {
        int v = read_int("File size in bytes: ");
        if (v < 0) { puts("Invalid file size."); return; }
        size = (unsigned long)v;
    }
    node = tree_create_node(name, type, size, parent);
    if (!node || !tree_add_child(parent, node)) {
        free(node); puts("Could not create item (possibly out of memory)."); return;
    }
    tree_print_path(node, path, sizeof(path));
    printf("Created successfully: %s\n", path);
}

static void search_item(Node *root) {
    char path[DIRIX_PATH_MAX], name[DIRIX_NAME_MAX], found_path[DIRIX_PATH_MAX];
    Node *found;
    read_line("Enter exact item name: ", name, sizeof(name));
    found = tree_search_name(root, name);
    if (!found) { puts("No matching item found."); return; }
    tree_print_path(found, found_path, sizeof(found_path));
    printf("Found %s: %s\n", found->type == NODE_DIRECTORY ? "directory" : "file", found_path);
    (void)path;
}

static void rename_item(Node *root) {
    char path[DIRIX_PATH_MAX], new_name[DIRIX_NAME_MAX], parent_path[DIRIX_PATH_MAX];
    char copy[DIRIX_PATH_MAX], *last;
    Node *parent, *node;
    read_line("Item path to rename: ", path, sizeof(path));
    if (strlen(path) >= sizeof(copy)) { puts("Path too long."); return; }
    strcpy(copy, path);
    last = strrchr(copy, '/');
    if (!last || !last[1] || strcmp(path, "/") == 0) { puts("Invalid path."); return; }
    if (last == copy) strcpy(parent_path, "/");
    else { *last = '\0'; strcpy(parent_path, copy); }
    parent = tree_resolve_directory(root, parent_path);
    if (!parent) { puts("Parent directory not found."); return; }
    node = tree_find_child(parent, last + 1);
    if (!node) { puts("Item not found."); return; }
    read_line("New name: ", new_name, sizeof(new_name));
    puts(tree_rename(node, new_name) ? "Renamed successfully." : "Rename failed: invalid or duplicate name.");
}

static void move_item(Node *root) {
    char path[DIRIX_PATH_MAX], dest[DIRIX_PATH_MAX], copy[DIRIX_PATH_MAX];
    char parent_path[DIRIX_PATH_MAX], *last;
    Node *parent, *node, *new_parent;
    read_line("Item path to move: ", path, sizeof(path));
    if (strlen(path) >= sizeof(copy)) { puts("Path too long."); return; }
    strcpy(copy, path);
    last = strrchr(copy, '/');
    if (!last || !last[1] || strcmp(path, "/") == 0) { puts("Invalid path."); return; }
    if (last == copy) strcpy(parent_path, "/");
    else { *last = '\0'; strcpy(parent_path, copy); }
    parent = tree_resolve_directory(root, parent_path);
    if (!parent || !(node = tree_find_child(parent, last + 1))) { puts("Item not found."); return; }
    read_line("Destination directory path: ", dest, sizeof(dest));
    new_parent = tree_resolve_directory(root, dest);
    puts(tree_move(node, new_parent) ? "Moved successfully." :
         "Move failed: invalid destination, duplicate name, or circular move.");
}

static void delete_item(Node *root) {
    char path[DIRIX_PATH_MAX], copy[DIRIX_PATH_MAX], parent_path[DIRIX_PATH_MAX], *last;
    Node *parent, *node;
    read_line("Item path to delete: ", path, sizeof(path));
    if (strlen(path) >= sizeof(copy)) { puts("Path too long."); return; }
    strcpy(copy, path);
    last = strrchr(copy, '/');
    if (!last || !last[1] || strcmp(path, "/") == 0) { puts("Invalid path; root cannot be deleted."); return; }
    if (last == copy) strcpy(parent_path, "/");
    else { *last = '\0'; strcpy(parent_path, copy); }
    parent = tree_resolve_directory(root, parent_path);
    if (!parent || !(node = tree_find_child(parent, last + 1))) { puts("Item not found."); return; }
    puts(tree_delete(node) ? "Deleted successfully." : "Delete failed.");
}

static void show_properties(Node *root) {
    char path[DIRIX_PATH_MAX], copy[DIRIX_PATH_MAX], parent_path[DIRIX_PATH_MAX], *last;
    Node *parent, *node;
    size_t dirs = 0, files = 0;
    unsigned long bytes = 0;
    read_line("Item path: ", path, sizeof(path));
    if (strcmp(path, "/") == 0) node = root;
    else {
        if (strlen(path) >= sizeof(copy)) { puts("Path too long."); return; }
        strcpy(copy, path);
        last = strrchr(copy, '/');
        if (!last || !last[1]) { puts("Invalid path."); return; }
        if (last == copy) strcpy(parent_path, "/");
        else { *last = '\0'; strcpy(parent_path, copy); }
        parent = tree_resolve_directory(root, parent_path);
        node = parent ? tree_find_child(parent, last + 1) : NULL;
    }
    if (!node) { puts("Item not found."); return; }
    tree_print_path(node, copy, sizeof(copy));
    printf("Path: %s\nType: %s\n", copy,
           node->type == NODE_DIRECTORY ? "Directory" : "File");
    if (node->type == NODE_FILE) printf("Size: %lu bytes\n", node->file_size);
    else {
        tree_statistics(node, &dirs, &files, &bytes);
        printf("Directories (including selected): %zu\nFiles: %zu\nTotal file size: %lu bytes\n",
               dirs, files, bytes);
    }
}

static void show_stats(Node *root) {
    size_t dirs = 0, files = 0;
    unsigned long bytes = 0;
    tree_statistics(root, &dirs, &files, &bytes);
    printf("\nDirectories: %zu\nFiles: %zu\nTotal items: %zu\nStored file size: %lu bytes\n",
           dirs, files, dirs + files - 1, bytes);
}

static void menu(void) {
    puts("\n========== DIRIX ==========");
    puts("1. Create Directory");
    puts("2. Add File");
    puts("3. Search File / Directory");
    puts("4. Display Directory Tree");
    puts("5. Rename File / Directory");
    puts("6. Move File / Directory");
    puts("7. Delete File / Directory");
    puts("8. View Item Properties");
    puts("9. Directory Statistics");
    puts("10. Save Now");
    puts("0. Save and Exit");
    puts("===========================");
}

int main(void) {
    Node *root = storage_load(DATA_FILE);
    int choice;
    if (!root) {
        root = tree_create_root();
        if (!root) { fprintf(stderr, "Unable to allocate root directory.\n"); return EXIT_FAILURE; }
        puts("No saved directory data found. A new DIRIX workspace was created.");
    } else puts("Saved DIRIX workspace loaded successfully.");

    do {
        menu();
        choice = read_int("Enter choice: ");
        switch (choice) {
            case 1: add_item(root, NODE_DIRECTORY); break;
            case 2: add_item(root, NODE_FILE); break;
            case 3: search_item(root); break;
            case 4: tree_display(root, 0); break;
            case 5: rename_item(root); break;
            case 6: move_item(root); break;
            case 7: delete_item(root); break;
            case 8: show_properties(root); break;
            case 9: show_stats(root); break;
            case 10:
                puts(storage_save(root, DATA_FILE) ? "Data saved successfully." : "Save failed.");
                break;
            case 0:
                if (!storage_save(root, DATA_FILE)) puts("Warning: data could not be saved.");
                else puts("Data saved. Goodbye!");
                break;
            default: puts("Invalid choice. Select a listed option.");
        }
    } while (choice != 0);

    tree_free(root);
    return 0;
}
