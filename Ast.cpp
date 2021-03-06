#include "Ast.h"

Ast::Ast()
{
	tree = nullptr;
}

Ast::~Ast()
{

}

void Ast::descent_recursive(Node* current_node)
{
	if (current_node == nullptr) {
		return;
	}

	descent_recursive(current_node->operand1);
	descent_recursive(current_node->operand2);
	descent_recursive(current_node->operand3);
	descent_recursive(current_node->operand4);
}

void Ast::print_recursive(Node* current_node, size_t level)
{
	if (current_node == nullptr) {
		return;
	}

	for (int i = 0; i < level; ++i) {
		cout_log("  ");
	}

	cout_log("+-");

	switch (current_node->type){
		case node_type::PROG:{
			cout_log("PROG");
			break;
		}
		case node_type::EXPR:{
			cout_log("EXPR");
			break;
		}
		case node_type::DCLRT: {
			cout_log("DCLRT");
			break;
		}
		case node_type::SEQ: {
			cout_log("SEQ");
			break;
		}
		case node_type::SET: {
			cout_log("SET");
			break;
		}
		case node_type::IF: {
			cout_log("IF");
			break;
		}
		case node_type::ELSE: {
			cout_log("ELSE");
			break;
		}
		case node_type::FOR: {
			cout_log("FOR");
			break;
		}
		case node_type::VARIABLE: {
			cout_log("VARIABLE ");
			cout_log("(");
			cout_log(current_node->value);
			cout_log(")");
			break;
		}
		case node_type::NUMBER:{
			cout_log("NUMBER ");
			cout_log("(");
			cout_log(current_node->value);
			cout_log(")");
			break;
		}
		case node_type::MORE: {
			cout_log("MORE");
			break;
		}
		case node_type::LESS: {
			cout_log("LESS");
			break;
		}
		case node_type::EQUALITY: {
			cout_log("EQUALITY");
			break;
		}
		case node_type::ADD:{
			cout_log("ADD");
			break;
		}
		case node_type::SUB:{
			cout_log("SUB");
			break;
		}
		case node_type::MUL:{
			cout_log("MUL");
			break;
		}
		case node_type::DIV:{
			cout_log("DIV");
			break;
		}
		case node_type::PRINTLN: {
			cout_log("PRINTLN");
			break;
		}
	}

	endl_log();

	print_recursive(current_node->operand1, level + 1);
	print_recursive(current_node->operand2, level + 1);
	print_recursive(current_node->operand3, level + 1);
	print_recursive(current_node->operand4, level + 1);

}

void Ast::print() {
	cout_log("------ AST ------");
	endl_log();
	print_recursive(tree, 0);
	endl_log();

	cout << "INFO: AST writed successfully" << endl;
}
