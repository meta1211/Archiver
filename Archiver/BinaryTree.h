#pragma once
#include <vector>
#include <string>
#include <iostream>
class Node
{
public:
	Node(char c = (char)0, size_t freq = 0, Node *left = nullptr, Node *right = nullptr);
	Node(const Node& node);
	size_t& GetFrequency();
	char& GetSymbol();
	void Add(Node *child);
	//void operator =(const Node &node);
	Node *&GetLeft()
	{
		return leftChild;
	}
	Node *&GetRight()
	{
		return rightChild;
	}
	~Node() {};

private:
	char symbol;
	size_t frequency;
	Node *leftChild;
	Node *rightChild;
};

class BinaryTree
{
	void RecursiveStringConvert(std::vector<std::string> &tree, Node *node, int deep);
	//Takes root as argument
	void CopyObect(Node *node, Node *source);
public:
	BinaryTree();
	BinaryTree(Node *node);
	BinaryTree(const BinaryTree& tree);
	Node *GetRoot();
	size_t GetFrequency();
	bool operator <(BinaryTree &b);
	BinaryTree operator=(BinaryTree &tree);
	BinaryTree operator+(BinaryTree &tree);
	void Print();
	//TO DO : recursive desctructor
	~BinaryTree();
private:
	Node *root;
};

