#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include <random>
#include <sstream>
#include <cctype>

struct Node
{
	int value;
	Node *left;
	Node *right;
};

/// Get functions
int get_height(Node* node);
int get_balance_factor(Node* node);

/// Rebalancing functions
void rebalance(Node*& node);
Node *rotation_ll(Node *x);
Node *rotation_rr(Node *x);
Node *rotation_lr(Node *x);
Node *rotation_rl(Node *x);

/// Tree manipulation functions
Node *new_node(int value);
void add(Node*& node, int value, int& elements);
void generate_tree_from_vec(std::vector<int>& vec, bool display_tree, bool display_row);
void release_tree(Node* node);

/// Displaying finctions
void display_as_row(Node* node);
void display_as_tree(Node* node, int level = 0);

/// Median functions
void get_median_vec(Node* node, int elements, int& index, std::vector<float>& median);
float median(Node* root, int& elements);
float gt_median(std::vector<int> random_vector);

/// Test function
void run_test(int number_of_tests, int data_struct_lenght, bool display_tree, bool display_row);

/// Utility function
bool is_number(std::string& s);