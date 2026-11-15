# DIRIX — Viva Questions and Answers

## 1. What is DIRIX?
DIRIX is a C console application that simulates a file directory system using a General Tree data structure.

## 2. Why is a tree used?
A directory can contain many files and subdirectories. A tree naturally represents this parent-child hierarchy.

## 3. What is a General Tree?
A tree in which a node can have any number of children.

## 4. Which representation is used?
First-Child / Next-Sibling representation. Each node stores pointers to its first child and its next sibling.

## 5. What is the root?
The topmost node of the tree. In DIRIX, it represents the root directory.

## 6. How is persistence implemented?
The program writes each item as a text record containing its type, absolute path and file size. At startup, it reads the records and rebuilds the tree and pointers.

## 7. Why not save pointers directly?
Memory addresses can change between program executions. Therefore, the program saves logical information such as paths and reconstructs pointers when loading.

## 8. What is recursion?
A function calling itself. DIRIX uses recursion for searching, displaying, statistics and freeing a subtree.

## 9. What is a leaf node?
A node that has no children. Files are leaf nodes in this application.

## 10. How is a circular move prevented?
Before moving a node, the program checks whether the destination directory is the node itself or one of its descendants.

## 11. What is the time complexity of searching?
O(n) in the worst case, where n is the number of nodes.

## 12. Does DIRIX create actual computer folders?
No. It is a virtual simulation stored in a text data file. It does not modify the operating system's file system.
