#include "avl_tree.h"

/// Get height of the current node. Leafs has height=1
int get_height(Node* node)
{
	if (node == NULL)
	{
		return 0;
	}

	return std::max(get_height(node->left), get_height(node->right)) + 1;
}

/// Get balance factor of the current node
int get_balance_factor(Node* node)
{
	if (node == NULL)
	{
		return 0;
	}

	return get_height(node->left) - get_height(node->right);
}

/// Create new node
Node *new_node(int value)
{
	Node *new_node = new Node();
	new_node->value = value;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}

/// Releasing tree
void release_tree(Node* node)
{
	if (node == NULL)
	{
		return;
	}

	release_tree(node->left);
	release_tree(node->right);

	delete node;
}

/// Add new node
void add(Node*& node, int value, int& elements)
{
	if (node == NULL)
	{
		node = new_node(value);
		elements++;
	}

	if (value < node->value)
	{
		//std::cout << "Adding element: " << value << std::endl;
		add(node->left, value, elements);
		rebalance(node);
	}
	else if (value > node->value)
	{
		//std::cout << "Adding element: " << value << std::endl;
		add(node->right, value, elements);
		rebalance(node);
	}
}

/// Generating tree from input vector and releasing memory at the end
void generate_tree_from_vec(std::vector<int>& vec, bool display_tree, bool display_row)
{
	// Init new root node
	Node *root = NULL;
	int elements = 0;

	// Add elements to the tree
	std::cout << "Adding the following elements: ";
	for (int elem_index = 0; elem_index < vec.size(); elem_index++)
	{
		std::cout << vec[elem_index] << " ";
		add(root, vec[elem_index], elements);
	}
	std::cout << std::endl << std::endl;


	if (display_tree)
	{
		std::cout << "Display as tree (rotated): " << std::endl;
		display_as_tree(root);
		std::cout << std::endl;
	}

	if (display_row)
	{
		std::cout << "Display as array: " << std::endl;
		display_as_row(root);
		std::cout << std::endl;
	}

	// Calculate median and compare it with so called 'groundtruth median'
	std::cout << std::endl;
	float calculated_median = median(root, elements);
	float groundtruth_median = gt_median(vec);
	std::cout << "Calculated  median: " << calculated_median << std::endl;
	std::cout << "Groundtruth median: " << groundtruth_median << std::endl;

	if (calculated_median == groundtruth_median)
	{
		std::cout << std::endl << "Status [SUCCEDDED]" << std::endl;
	}
	else
	{
		std::cout << std::endl << "Status [FAILED] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	}

	release_tree(root);
}


/// Rebalancing criteria:
///
/// If X is node with balance factor -2 or +2 
/// and
/// Z is X's child
///
/// There are 4 possible rebalancing rotations:
/// Rotation LL: X balance factor = -2 and Z balance factor = -1
/// Rotation RR: X balance factor =  2 and Z balance factor =  1
/// Rotation LR: X balance factor =  2 and Z balance factor = -1
/// Rotation RL: X balance factor = -2 and Z balance factor =  1
void rebalance(Node*& node)
{
	int parent_node_bf = get_balance_factor(node);
	int left_node_bf = get_balance_factor(node->left);
	int right_node_bf = get_balance_factor(node->right);

	if (parent_node_bf > 1)
	{
		if (left_node_bf > 0)
			node = rotation_rr(node);
		else
			node = rotation_lr(node);
	}
	else if (parent_node_bf < -1)
	{
		if (right_node_bf < 0)
			node = rotation_ll(node);
		else
			node = rotation_rl(node);
	}
}

/// Rotation LL
///
///     (x)bf=-2                (z)
///       \                     / \
///       (z)bf=-1      =>    (x) (y)
///         \                    
///         (y)bf=0   
Node *rotation_ll(Node *x)
{
	Node *z = x->right;
	x->right = z->left;
	z->left = x;
	return z;
}

/// Ratation RR
///
///         (x)bf=2              (z)
///         /                    / \
///       (z)bf=1       =>     (y) (x)
///       /                       
///     (y)bf=0                  
Node *rotation_rr(Node *x)
{
	Node *z = x->left;
	x->left = z->right;
	z->right = x;
	return z;
}

/// Ratation LR
///
///         (x)bf=2              (y)
///         /                    / \
///       (z)bf=-1       =>    (z) (x)
///         \                      
///         (y)bf=0                  
Node *rotation_lr(Node *x)
{
	Node *y;
	y = x->left;
	x->left = rotation_ll(y);
	return rotation_rr(x);
}

/// Ratation RL
///
///         (x)bf=-2              (y)
///           \                   / \
///           (z)bf=1      =>   (x) (z)
///           /                     
///         (y)bf=0                  
Node *rotation_rl(Node *x)
{
	Node *y;
	y = x->right;
	x->right = rotation_rr(y);
	return rotation_ll(x);
}

/// Display as array
void display_as_row(Node* node)
{
	if (node == NULL)
	{
		return;
	}

	display_as_row(node->left);
	std::cout << node->value << " ";
	//std::cout << node->value << " height:" << get_height(node) << " factor:" << get_balance_factor(node) << std::endl;
	display_as_row(node->right);

}

/// Display as tree
void display_as_tree(Node* node, int level)
{
	if (node == NULL)
	{
		return;
	}

	display_as_tree(node->right, level + 1);
	std::cout << std::string(level * 7, ' ');
	std::cout << node->value << std::endl;
	//std::cout << get_balance_factor(node) << "(" << node->value << ")" << get_height(node) << std::endl;
	display_as_tree(node->left, level + 1);
}

/// Get median wrapper
float median(Node* root, int& elements)
{
	int index = 0;
	std::vector<float> median;
	get_median_vec(root, elements, index, median);

	if (median.size() == 1)
	{
		return median[0];
	}
	else
	{
		return (median[0] + median[1]) / 2;
	}

}

/// Get 'median vector' consisting of the central or the two central values 
void get_median_vec(Node* node, int elements, int& index, std::vector<float>& median)
{
	int reminder = elements % 2;
	int position = (int)(elements / 2) + 1;

	if (node != NULL && median.size() != (2 - reminder))
	{
		get_median_vec(node->left, elements, index, median);

		index++;

		if (reminder == 1 ? (index == position) : (index == position || index == position - 1))
		{
			median.push_back(node->value);
		}

		get_median_vec(node->right, elements, index, median);
	}
}

/// Get called 'groundtruth median' calculated by sorting input vector and returning the central value or average of the two central values
float gt_median(std::vector<int> random_vector)
{
	std::sort(random_vector.begin(), random_vector.end());
	
	int reminder = random_vector.size() % 2;
	int position = (int)(random_vector.size() / 2);

	if (reminder == 1)
	{
		return random_vector[position];
	}
	else
	{
		return ((float)(random_vector[position - 1] + random_vector[position])) / 2;
	}
}

/// Run random test
void run_test(int number_of_tests, int data_struct_lenght, bool display_tree, bool display_row)
{
	for (int i = 0; i < number_of_tests; i++)
	{
		std::cout << "#########################################################################" << std::endl;
		std::cout << " Performing test " << i + 1 << std::endl;
		std::cout << "#########################################################################" << std::endl;
	
		// Init random number engine
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> rand_range_min_distribution(-100, 100);
		int min = rand_range_min_distribution(rng);
		std::uniform_int_distribution<int> distribution(min, min + data_struct_lenght * 10);

		// Generate vector with unique random values 
		std::unordered_set<int> random_set;
		while (random_set.size() < data_struct_lenght)
		{
			random_set.insert(distribution(rng));
		}
		std::vector<int> random_vector(random_set.size());
		std::copy(begin(random_set), end(random_set), begin(random_vector));

		// Generate tree
		generate_tree_from_vec(random_vector, display_tree, display_row);

		std::cout << "#########################################################################" << std::endl << std::endl;
	}

	std::cout << std::endl;
}

bool is_number(std::string& s)
{
	std::string::const_iterator it = s.begin();

	if (*it == '-')
	{
		++it;
		if (it == s.end())
		{
			return false;
		}
	}

	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

int main()
{
	std::cout << "#########################################################################" << std::endl;
	std::cout << " C++ implementation of avl tree." << std::endl;
	std::cout << " It is chosen because of its O(long) insert and search complexity." << std::endl;
	std::cout << "#########################################################################" << std::endl << std::endl;

	while (true)
	{
		int choice;

		Node *root = NULL;
		int elements = 0;

		std::cout << "#########################################################################" << std::endl;
		std::cout << "1. Add elements" << std::endl;
		std::cout << "2. Run automatic tests" << std::endl;
		std::cout << "3. Exit" << std::endl;
		std::cout << "#########################################################################" << std::endl;
		std::cout << std::endl;
		std::cout << "Enter your Choice: ";
		std::cin >> choice;
		switch (choice)
		{
			case 1:
			{
				std::cout << "#########################################################################" << std::endl;
				std::string line, number;
				std::vector<int> numbers;
				std::cout << "Enter real values to be inserted (space separated): ";
				std::cin.ignore();
				std::getline(std::cin, line);
				std::istringstream iss(line);

				bool input_error = false;
				while (iss >> number)
				{
					if (is_number(number))
					{
						numbers.push_back(std::stoi(number));
					}
					else
					{
						std::cout << "You should insert only real numbers!" << std::endl;
						input_error = true;
					}
				}

				if (input_error)
				{
					break;
				}

				// Remove duplications
				std::cout << std::endl << "All duplications will be removed..." << std::endl << std::endl;
				std::unordered_set<int> s(numbers.begin(), numbers.end());
				numbers.assign(s.begin(), s.end());

				// Generate tree
				generate_tree_from_vec(numbers, true, true);

				std::cout << std::endl;
				break;
			}
			case 2:
			{
				int tests = 0;
				int lenght = 0;
				std::cout << "#########################################################################" << std::endl;
				std::cout << "Number of tests: ";
				std::cin >> tests;
				std::cout << "Number of the randomly generated values: ";
				std::cin >> lenght;

				if (tests < 1)
				{
					std::cout << "You should choose minimum 1 test!" << std::endl;
					break;
				}

				if (lenght < 1)
				{
					std::cout << "Size of the randomly generated values must be greater than 0!" << std::endl;
					break;
				}

				// Run tests
				run_test(tests, lenght, true, true);
				break;
			}
			case 3:
			{
				exit(1);
				break;
			}
			default:
			{
				std::cout << "Wrong choice" << std::endl;
			}
		}
	}

	return 0;
}