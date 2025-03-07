#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

const int NUM = 250000;

typedef struct EmployeeInfo {
    int salary;
    int age;
    int emplNumber;
    int sin; // search by social insurance number
} EmployeeInfo;

typedef struct node {
    EmployeeInfo empl;
    node* left;
    node* right;
    int height;
} node;

class AVL {
private:
    node* root;

    // Private helper functions
    void makeEmpty(node* t);
    int max(int a, int b);
    int min(int a, int b);
    node* insert(EmployeeInfo empl, node* t);
    node* singleRightRotate(node* &t);
    node* singleLeftRotate(node* &t);
    node* doubleLeftRotate(node* &t);
    node* doubleRightRotate(node* &t);
    node* remove(int sin, node* t);
    int height(node* t);
    void inorder(node* t);

public:
    AVL();
    ~AVL();  // Destructor to free memory
    void insert(EmployeeInfo empl);
    void remove(int sin);
    void display();
    node* GetRoot();
    node* Find(node* t, int sin);
};

#endif // AVL_TREE_H
