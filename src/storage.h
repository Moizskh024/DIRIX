#ifndef DIRIX_STORAGE_H
#define DIRIX_STORAGE_H
#include "tree.h"

int storage_save(Node *root, const char *filename);
Node *storage_load(const char *filename);

#endif
