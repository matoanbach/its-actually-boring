// AVL.cpp - Implementation of AVL tree
//
// Created - March, 2024
// Author - Miguel Watler
// Modified - Yiyuan Dong

#include "AVLTree.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

ofstream outfile;

void AVL::makeEmpty(node* t) {
    if (t == NULL)
        return;
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
}

int AVL::max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

int AVL::min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

node* AVL::insert(EmployeeInfo empl, node* t) {
    if (t == NULL) {
        t = new node;
        t->empl.salary = empl.salary;
        t->empl.age = empl.age;
        t->empl.emplNumber = empl.emplNumber;
        t->empl.sin = empl.sin;
        t->height = 0;
        t->left = t->right = NULL;
    }
    else if (empl.sin < t->empl.sin) { // Go down the left tree
        t->left = insert(empl, t->left);
        if (height(t->left) - height(t->right) ==
            2) { // Left is higher than right by two
            if (empl.sin < t->left->empl.sin)
                t = singleRightRotate(
                    t); // will go down the left subtree, do right rotate
            else
                t = doubleRightRotate(
                    t); // will go down the right subtree, do left-right rotate
        }
    }
    else if (empl.sin > t->empl.sin) { // Go down the right tree
        t->right = insert(empl, t->right);
        if (height(t->right) - height(t->left) ==
            2) { // Right is higher than left by two
            if (empl.sin > t->right->empl.sin)
                t = singleLeftRotate(
                    t); // Will go down the right subtree, do left rotate
            else
                t = doubleLeftRotate(
                    t); // Will go down the left subtree, do right-left rotate
        }
    }

    t->height = max(height(t->left), height(t->right)) + 1;
    return t;
}

node* AVL::singleRightRotate(node*& t) { //        8
    //    4             12
    // 2     6     10        14
    // 1 3   5 7   9  11    13  15
    node* u = t->left;
    //          8
    //    4(U)          12
    // 2     6     10        14
    // 1 3   5 7   9  11    13  15
    t->left = u->right;
    //          8
    //    6          12
    // 5     7     10        14
    //            9  11    13  15
    //---------------------------
    //    4(U)
    // 2     6
    // 1 3   5 7
    u->right = t;
    //          4
    //    2           8
    // 1     3    6         12
    //           5 7    10     14
    //                 9  11 13  15
    t->height = max(height(t->left), height(t->right)) + 1;
    u->height = max(height(u->left), t->height) + 1;
    return u;
}

node* AVL::singleLeftRotate(node*& t) { //        8
    //    4             12
    // 2     6     10        14
    // 1 3   5 7   9  11    13  15
    node* u = t->right;
    //          8
    //    4             12(U)
    // 2     6     10        14
    // 1 3   5 7   9  11    13  15
    t->right = u->left;
    //          8
    //    4        10
    // 2     6   9   11
    // 1 3   5 7
    //-------------------------
    //                  12(U)
    //             10        14
    //            9  11    13  15
    u->left = t;
    //              12
    //       8              14
    //   4      10     13        15
    // 2   6   9  11
    // 1 3 5 7
    t->height = max(height(t->left), height(t->right)) + 1;
    u->height = max(height(t->right), t->height) + 1;
    return u;
}

node* AVL::doubleLeftRotate(node*& t) { // Right rotate the right node, then
    // left rotate the current node
    t->right = singleRightRotate(t->right);
    return singleLeftRotate(t);
}

node* AVL::doubleRightRotate(
    node*& t) { // Left rotate the left node, then right rotate the current node
    t->left = singleLeftRotate(t->left);
    return singleRightRotate(t);
}

node* AVL::findMin(node* t) {
    if (t == NULL)
        return NULL;
    else if (t->left == NULL)
        return t;
    else
        return findMin(t->left);
}

node* AVL::findMax(node* t) {
    if (t == NULL)
        return NULL;
    else if (t->right == NULL)
        return t;
    else
        return findMax(t->right);
}

node* AVL::remove(int sin, node* t) {
    node* temp;

    // Element not found
    if (t == NULL)
        return NULL;

    // Searching for element
    else if (sin < t->empl.sin)
        t->left = remove(sin, t->left);
    else if (sin > t->empl.sin)
        t->right = remove(sin, t->right);

    // Element found
    // With 2 children
    else if (t->left && t->right) {
        temp = findMin(t->right);
        t->empl.sin = temp->empl.sin;
        t->right = remove(temp->empl.sin, t->right);
    }
    // With one or zero child
    else {
        temp = t;
        if (t->left == NULL)
            t = t->right;
        else if (t->right == NULL)
            t = t->left;
        delete temp;
    }
    if (t == NULL)
        return t;

    t->height = max(height(t->left), height(t->right)) + 1;

    // If node is unbalanced
    // If left node is deleted, right case
    if (height(t->left) - height(t->right) == 2) {
        // right right case
        if (height(t->left->left) - height(t->left->right) == 1)
            return singleLeftRotate(t);
        // right left case
        else
            return doubleLeftRotate(t);
    }
    // If right node is deleted, left case
    else if (height(t->right) - height(t->left) == 2) {
        // left left case
        if (height(t->right->right) - height(t->right->left) == 1)
            return singleRightRotate(t);
        // left right case
        else
            return doubleRightRotate(t);
    }
    return t;
}

int AVL::height(node* t) { return (t == NULL ? -1 : t->height); }

void AVL::inorder(node* t) {
    if (t == NULL)
        return;
    inorder(t->left);
    outfile << " height:" << t->height << " sin:" << t->empl.sin
        << " employee number:" << t->empl.emplNumber
        << " salary:" << t->empl.salary << " age:" << t->empl.age << endl;
    inorder(t->right);
}

// public

AVL::AVL() { root = NULL; }

void AVL::insert(EmployeeInfo empl) { root = insert(empl, root); }

void AVL::remove(int sin) {
    // cout << "removing sin:" << sin << " on root " << root << endl;
    root = remove(sin, root);
}

void AVL::display() {
    inorder(root);
    outfile << endl;
}

void AVL::display(char file[]) {
    outfile.open(file);
    inorder(root);
    outfile << endl;
    outfile.close();
}

node* AVL::GetRoot() { return root; }
node* AVL::Find(node* node, int sin) {
    if (node == NULL) {
        /* Element is not found */
        return NULL;
    }
    if (sin > node->empl.sin) {
        /* Search in the right sub tree. */
        return Find(node->right, sin);
    }
    else if (sin < node->empl.sin) {
        /* Search in the left sub tree. */
        return Find(node->left, sin);
    }
    else {
        /* Element Found */
        return node;
    }
}

int AVL::getBalance(node* t) {
    if (t == NULL)
        return 0;
    else
        return height(t->left) - height(t->right);
}
