// AVLTree.h - Header file for AVL Tree

#ifndef AVL_TREE_H
#define AVL_TREE_H

 // AVLTree.cpp: AVL Tree Implementation in C++   */
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

typedef struct EmployeeInfo {
	int salary;
	int age;
	int emplNumber;
	int sin;//search by social insurance number
}EmployeeInfo;

typedef struct node {
	EmployeeInfo empl;
	node* left;
	node* right;
	int height;
}node;

class AVL
{
	node* root;
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
	void insert(EmployeeInfo empl);
	void remove(int sin);
	void display(char filename[]);
	node * GetRoot();
	node * Find(node *node, int sin);
	void makeEmpty(node* t);
	int getBalance(node* t);
	node* findMin(node* t);
	node* findMax(node* t);
};

#endif // AVL_TREE_H