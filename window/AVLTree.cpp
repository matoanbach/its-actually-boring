#include "AVLTree.h"
#include <cstdlib>
#include <algorithm>

// Global outfile for outputting the inorder traversal.
ofstream outfile;

AVL::AVL() {
    root = NULL;
}

AVL::~AVL() {
    makeEmpty(root);
}

void AVL::makeEmpty(node* t) {
    if (t == NULL)
        return;
    makeEmpty(t->left);
    makeEmpty(t->right);
    delete t;
}

int AVL::max(int a, int b) {
    return (a > b ? a : b);
}

int AVL::min(int a, int b) {
    return (a < b ? a : b);
}

int AVL::height(node* t) {
    return (t == NULL ? -1 : t->height);
}

node* AVL::insert(EmployeeInfo empl, node* t) {
    if (t == NULL) {
        t = new node;
        t->empl = empl;
        t->height = 0;
        t->left = t->right = NULL;
    }
    else if (empl.sin < t->empl.sin) {
        t->left = insert(empl, t->left);
        if (height(t->left) - height(t->right) == 2) {
            if (empl.sin < t->left->empl.sin)
                t = singleRightRotate(t);
            else
                t = doubleRightRotate(t);
        }
    }
    else if (empl.sin > t->empl.sin) {
        t->right = insert(empl, t->right);
        if (height(t->right) - height(t->left) == 2) {
            if (empl.sin > t->right->empl.sin)
                t = singleLeftRotate(t);
            else
                t = doubleLeftRotate(t);
        }
    }
    t->height = max(height(t->left), height(t->right)) + 1;
    return t;
}

void AVL::insert(EmployeeInfo empl) {
    root = insert(empl, root);
}

node* AVL::singleRightRotate(node* &t) {
    node* u = t->left;
    t->left = u->right;
    u->right = t;
    t->height = max(height(t->left), height(t->right)) + 1;
    u->height = max(height(u->left), t->height) + 1;
    return u;
}

node* AVL::singleLeftRotate(node* &t) {
    node* u = t->right;
    t->right = u->left;
    u->left = t;
    t->height = max(height(t->left), height(t->right)) + 1;
    u->height = max(height(t->right), t->height) + 1;
    return u;
}

node* AVL::doubleLeftRotate(node* &t) {
    t->right = singleRightRotate(t->right);
    return singleLeftRotate(t);
}

node* AVL::doubleRightRotate(node* &t) {
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
    if (t == NULL)
        return NULL;
    else if (sin < t->empl.sin)
        t->left = remove(sin, t->left);
    else if (sin > t->empl.sin)
        t->right = remove(sin, t->right);
    else if (t->left && t->right) {
        temp = findMin(t->right);
        t->empl.sin = temp->empl.sin;
        t->right = remove(t->empl.sin, t->right);
    }
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
    if (height(t->left) - height(t->right) == 2) {
        if (height(t->left->left) - height(t->left->right) == 1)
            return singleLeftRotate(t);
        else
            return doubleLeftRotate(t);
    }
    else if (height(t->right) - height(t->left) == 2) {
        if (height(t->right->right) - height(t->right->left) == 1)
            return singleRightRotate(t);
        else
            return doubleRightRotate(t);
    }
    return t;
}

void AVL::remove(int sin) {
    root = remove(sin, root);
}

void AVL::inorder(node* t) {
    if (t == NULL)
        return;
    inorder(t->left);
    outfile << " height:" << t->height
            << " sin:" << t->empl.sin
            << " employee number:" << t->empl.emplNumber
            << " salary:" << t->empl.salary
            << " age:" << t->empl.age << "\n";
    inorder(t->right);
}

void AVL::display() {
    inorder(root);
    outfile << "\n";
}

node* AVL::GetRoot() {
    return root;
}

node* AVL::Find(node* t, int sin) {
    if (t == NULL)
        return NULL;
    if (sin > t->empl.sin)
        return Find(t->right, sin);
    else if (sin < t->empl.sin)
        return Find(t->left, sin);
    else
        return t;
}

