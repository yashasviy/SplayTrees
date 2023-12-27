#include <iostream>
#include <cstdlib>



// Data Structure for Splay Tree
typedef struct splay{
	int key;
	struct splay* lchild;
	struct splay* rchild;
}splay;

//List of Functions
splay* newnode(int key);
inline splay* RR(splay* k2);
inline splay* LL(splay* k2);
splay* Splay(int key, splay* root);
splay* Insert(int key, splay* root);
splay* Delete(int key, splay* root);
splay* Search(int key, splay* root);
void InOrder(splay* root);




// Right-Right Rotation (RR):
// Rotate the subtree rooted at node a1 to the right.
inline splay* RR(splay* a1)
{
	splay* b1 = a1->lchild;
	a1->lchild = b1->rchild;
	b1->rchild = a1;
	return b1;
}

// Left-Left Rotation (LL):
// Rotate the subtree rooted at node a1 to the left.
inline splay* LL(splay* a1)
{
	splay* b1 = a1->rchild;
	a1->rchild = b1->lchild;
	b1->lchild = a1;
	return b1;
}

// Top-down Splay Tree Implementation:
// If the key is in the tree, the node containing the key will be rotated to the root.
// Otherwise, the last non-NULL node on the search path will be rotated to the root.
splay* Splay(int key, splay* root)
{
	if (!root)
		return nullptr;

	// Helper nodes for assembling left and right subtrees
	splay head;
	head.lchild = head.rchild = nullptr;
	splay* maxLeft = &head;
	splay* minRight = &head;

	while (true)
	{
		if (key < root->key)
		{
			if (!root->lchild)
				break;

			if (key < root->lchild->key)
			{
				root = RR(root);
				if (!root->lchild)
					break;
			}

			// Link to the right subtree
			minRight->lchild = root;
			minRight = minRight->lchild;
			root = root->lchild;
			minRight->lchild = nullptr;
		}
		else if (key > root->key)
		{
			if (!root->rchild)
				break;

			if (key > root->rchild->key)
			{
				root = LL(root);
				if (!root->rchild)
					break;
			}

			// Link to the left subtree
			maxLeft->rchild = root;
			maxLeft = maxLeft->rchild;
			root = root->rchild;
			maxLeft->rchild = nullptr;
		}
		else
			break;
	}

	// Assemble left, middle, and right subtrees together
	maxLeft->rchild = root->lchild;
	minRight->lchild = root->rchild;
	root->lchild = head.rchild;
	root->rchild = head.lchild;

	return root;
}

// Create a new node with the given key
splay* newnode(int key)
{
	splay* p = new splay;
	if (!p)
	{
		std::cerr << "Out of memory!\n";
		exit(1);
	}
	p->key = key;
	p->lchild = p->rchild = nullptr;
	return p;
}

// Insert a key into the Splay Tree
splay* Insert(int key, splay* root)
{
	// Reuse existing node or create a new one
	static splay* p = nullptr;
	if (!p)
		p = newnode(key);
	else
		p->key = key;

	// If the tree is empty, the new node becomes the root
	if (!root)
	{
		root = p;
		p = nullptr;
		return root;
	}

	// Perform Splay operation on the root
	root = Splay(key, root);

	// Create a new node for the key
	splay* new_node = newnode(key);

	// If the key is less than the root's key, update the tree accordingly
	if (key < root->key)
	{
		new_node->lchild = root->lchild;
		new_node->rchild = root;
		root->lchild = nullptr;
		root = new_node;
	}
	// If the key is greater than the root's key, update the tree accordingly
	else if (key > root->key)
	{
		new_node->rchild = root->rchild;
		new_node->lchild = root;
		root->rchild = nullptr;
		root = new_node;
	}
	// If the key is equal to the root's key, do not insert duplicates

	p = nullptr;
	return root;
}

// Delete a key from the Splay Tree
splay* Delete(int key, splay* root)
{
	splay* temp;
	if (!root)
		return nullptr;

	// Perform Splay operation on the root
	root = Splay(key, root);

	// If the key is not found in the tree, return the root directly
	if (key != root->key)
		return root;
	else
	{
		// If the left subtree is empty, replace root with the right subtree
		if (!root->lchild)
		{
			temp = root;
			root = root->rchild;
		}
		else
		{
			temp = root;
			// After Splay(key, root->lchild), the tree has no right child (key > any key in root->lchild)
			root = Splay(key, root->lchild);
			root->rchild = temp->rchild;
		}
		delete temp;
		return root;
	}
}

// Search for a key in the Splay Tree
splay* Search(int key, splay* root)
{
	return Splay(key, root);
}

// InOrder Traversal of the Splay Tree
void InOrder(splay* root)
{
	if (root)
	{
		InOrder(root->lchild);
		std::cout << "key: " << root->key;
		if (root->lchild)
			std::cout << "    left child:  " << root->lchild->key;
		if (root->rchild)
			std::cout << "    right child:  " << root->rchild->key;
		std::cout << "\n";
		InOrder(root->rchild);
	}
}

// // Example usage 
// int main()
// {
	
// 	splay* root = nullptr;
// 	//Insertion
// 	root = Insert(10, root);
// 	root = Insert(5, root);
// 	root = Insert(15, root);

// 	std::cout << "InOrder Traversal:\n";
// 	InOrder(root);

// 	root = Search(5, root);
// 	std::cout << "\nAfter making Splay tree(5):\n";
// 	InOrder(root);

// 	root = Delete(10, root);
// 	std::cout << "\nAfter Deletion(10):\n";
// 	InOrder(root);

// 	return 0;
// }
