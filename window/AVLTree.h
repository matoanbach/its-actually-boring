// AVL.h - Header file for AVL Tree
//
// Created - March, 2024
// Author - Yiyuan Dong


#ifndef AVL_TREE_H
#define AVL_TREE_H

using namespace std;

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

    node* root;

    // Empty the tree
    void makeEmpty(node* t);
    // Find the maximum of two numbers
    int max(int a, int b);
    // Find the minimum of two numbers
    int min(int a, int b);
    // Insert a node into the tree
    node* insert(EmployeeInfo empl, node* t);
    // Rotate the tree to the right
    node* singleRightRotate(node*& t);
    // Rotate the tree to the left
    node* singleLeftRotate(node*& t);
    // Double rotate the tree to the left
    node* doubleLeftRotate(node*& t);
    // Double rotate the tree to the right
    node* doubleRightRotate(node*& t);
    // Find the minimum node in the tree
    node* findMin(node* t);
    // Find the maximum node in the tree
    node* findMax(node* t);
    // Remove a node from the tree
    node* remove(int sin, node* t);
    // Find the height of a node
    int height(node* t);

    // Inorder traversal of the tree
    void inorder(node* t);

    friend class DatabaseTestSuite;

public:
    // Constructor
    AVL();
    // Insert a EmployeeInfo into the tree
    void insert(EmployeeInfo empl);
    // Remove a node from the tree by SIN
    void remove(int sin);
    // Display the tree
    void display();
    // Get the root node of the tree
    void display(char file[]);
    node* GetRoot();
    // Find a node in the tree by SIN
    node* Find(node* node, int sin);
    // Get the balance of a node
    int getBalance(node* t);
};

#endif // AVL_TREE_H