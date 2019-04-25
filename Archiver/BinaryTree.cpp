#include "BinaryTree.h"

Node::Node(char c, size_t freq, Node * left, Node * right)
{
	symbol = c;
	frequency = freq;
	leftChild = left;
	rightChild = right;
}

size_t& Node::GetFrequency()
{
	return frequency;
}

char& Node::GetSymbol()
{
	return symbol;
}

void Node::Add(Node * child)
{
	if (!leftChild)
	{
		leftChild = child;
	}
	else
	{
		if (leftChild->frequency > child->frequency)
		{
			rightChild = leftChild;
			leftChild = rightChild;
		}
	}
}


 BinaryTree::BinaryTree(Node * node)
{
	root = node;
}

 BinaryTree::BinaryTree(const BinaryTree & tree)
 {
	root = new Node();
	CopyObect(root, tree.root);
	 
 }

Node * BinaryTree::GetRoot()
 {
	 return root;
 }


void BinaryTree::RecursiveStringConvert(std::vector<std::string>& tree, Node * node, int deep)
 {
	 if (tree.size() < deep + 1)
	 {
		 tree.push_back(std::string());
	 }
	 tree[deep].push_back(node->GetSymbol());
	 tree[deep].push_back(' ');
	 if (node->GetLeft())
	 {
		 RecursiveStringConvert(tree, node->GetLeft(), deep + 1);
		 if (node->GetRight())
		 {
			 RecursiveStringConvert(tree, node->GetRight(), deep + 1);
		 }
	 }
 }

//Takes root as argument
void BinaryTree::CopyObect(Node * node, Node * source)
{
	node->GetSymbol() = source->GetSymbol();
	node->GetFrequency() = source->GetFrequency();
	if (source->GetLeft())
	{
		node->GetLeft() = new Node();
		CopyObect(node->GetLeft(), source->GetLeft());
		if (source->GetRight())
		{
			node->GetRight() = new Node();
			CopyObect(node->GetRight(), source->GetRight());
		}
	}
}

BinaryTree::BinaryTree()
{
	root = new Node();
}

size_t BinaryTree::GetFrequency()
 {
	 return root->GetFrequency();
 }

bool BinaryTree::operator<(BinaryTree & b)
{
	return GetFrequency() < b.GetFrequency();
}

BinaryTree BinaryTree::operator=(BinaryTree & tree)
{
	if (root)
	{
		this->~BinaryTree();
		root = new Node();
	}
	CopyObect(root, tree.GetRoot());
	return *this;
}

BinaryTree BinaryTree::operator+(BinaryTree & tree)
{
	BinaryTree result(new Node(0, GetFrequency() + tree.GetFrequency(), new Node(), new Node()));
	Node *left = root;
	Node *right = tree.GetRoot();
	if (left->GetFrequency() > right->GetFrequency())
	{
		std::swap(left, right);
	}
	CopyObect(result.GetRoot()->GetLeft(), left);
	CopyObect(result.GetRoot()->GetRight(), right);
	return result;
}

void BinaryTree::Print()
 {
	 std::vector<std::string> tree;
	 RecursiveStringConvert(tree, root, 0);
	 for (auto level : tree)
	 {
		 std::cout << level << '\n';
	 }
 }

BinaryTree::~BinaryTree()
{
}
