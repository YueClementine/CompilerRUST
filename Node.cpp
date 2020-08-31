#include "Node.h"

Node::Node(node_type type, const string value, Node* operand1,
	Node* operand2, Node* operand3, Node* operand4)
{
	this->type = type;
	this->value = value;
	this->operand1 = operand1;
	this->operand2 = operand2;
	this->operand3 = operand3;
	this->operand4 = operand4;
}

Node::Node(node_type type, Node* operand1, Node* operand2,
	Node* operand3, Node* operand4)
{
	this->type = type;
	this->operand1 = operand1;
	this->operand2 = operand2;
	this->operand3 = operand3;
	this->operand4 = operand4;
}
