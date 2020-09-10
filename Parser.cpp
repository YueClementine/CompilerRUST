#include "Parser.h"

Parser::Parser(const char* file_path)
{
	lex = new Lexer(file_path);
	ast = new Ast();
	var_table = new Variable_table();
}

Parser::~Parser()
{
	delete lex;
	delete ast;
}

void Parser::parse()
{
	lex->split(); // Receiving tokens
	lex->print(); // Write tokens to the log file

	ast->tree = program(); // Call Recursive descent parser
	ast->print(); // Write AST to the log file

	var_table->designate_variables_recursive(ast->tree); // fill the var_table
	var_table->print_vars(); // Write all vars to the log file
}

string Parser::eat(token_type type)
{
	if (!tryEat(type)) {
		error(to_string((int)type) + " expected!");
	}

	string value = lex->current_token_value();

	return value;
}

bool Parser::tryEat(token_type type)
{
	return lex->current_token_type() == type;
}

Node* Parser::program()
{
	Node* statement_node = statement();
	return statement_node;
}

Node* Parser::statement()
{
	if (tryEat(token_type::LBRACE)) {
		Node* compound_node = compound_statement();
		return compound_node;
	}
	else if (tryEat(token_type::LET)) {
		Node* declaration_node = new Node(node_type::DCLRT, declaration());
		return declaration_node;
	}
	else if (tryEat(token_type::IDENTIFIER)) {
		Node* assign_node = set();
		return assign_node;
	}
	else if (tryEat(token_type::IF_TOKEN)) {
		Node* selection_node = selection_statement();
		return selection_node;
	}
	else{
		Node* expression_node = new Node(node_type::EXPR, expression());

		eat(token_type::SEMICOLON);
		lex->next_token();

		return expression_node;
	}
}

Node* Parser::declaration() // ������� ������� ����������
{
	lex->next_token();
	eat(token_type::MUT);
	lex->next_token();

	Node* identifier_node = identifier();

	eat(token_type::SEMICOLON);
	lex->next_token();

	return identifier_node;
}

Node* Parser::set()
{
	Node* identifier_node = identifier();
	
	eat(token_type::EQUALLY);
	lex->next_token();

	Node* expression_node = expression();
	Node* set_node = new Node(node_type::SET, identifier_node, expression_node);

	eat(token_type::SEMICOLON);
	lex->next_token();
	
	return set_node;
}

Node* Parser::selection_statement()
{
	lex->next_token();

	Node* expression_node = expression();
	Node* compound_node = compound_statement();

	Node* selection_node = new Node(node_type::IF, expression_node, compound_node);

	if (tryEat(token_type::ELSE_TOKEN)) {
		lex->next_token();

		Node* compound_else_node = compound_statement();
		Node* else_node = new Node(node_type::ELSE, compound_else_node);
		selection_node->operand3 = else_node;
	}

	return selection_node;
}

Node* Parser::compound_statement()
{
	lex->next_token();

	Node* statement_node = new Node(node_type::SEQ);
	Node* last_node = statement_node;
	if (!tryEat(token_type::RBRACE)) last_node->operand1 = statement();

	while (!tryEat(token_type::RBRACE)) {

		last_node->operand2 = new Node(node_type::SEQ, statement());

		last_node = last_node->operand2;
	}
	lex->next_token();
	
	return statement_node;
}

Node* Parser::expression()
{
	Node* additive_expression_node = additive_expression();

	return additive_expression_node;
}

Node* Parser::additive_expression()
{
	Node* multiplicative_node = multiplicative_expression();

	if (tryEat(token_type::PLUS)) {
		lex->next_token();

		Node* additive_temp_node = additive_expression();

		Node* additive = new Node(node_type::ADD, multiplicative_node,
			additive_temp_node);

		return additive;
	}
	else if (tryEat(token_type::MINUS)) { 
		lex->next_token();

		Node* additive_temp_node = additive_expression();

		Node* additive = new Node(node_type::SUB, multiplicative_node,
			additive_temp_node);

		return additive;
	}

	return multiplicative_node;
}

Node* Parser::multiplicative_expression()
{
	Node* primary_node = primary_expression();

	if (tryEat(token_type::STAR)) {
		lex->next_token();

		Node* multiplicative_temp_node = multiplicative_expression();

		Node* multiplicative_expression = new Node(node_type::MUL, primary_node,
			multiplicative_temp_node);

		return multiplicative_expression;
	}
	else if (tryEat(token_type::SLASH)) {
		lex->next_token();

		Node* multiplicative_temp_node = multiplicative_expression();

		Node* multiplicative_expression = new Node(node_type::DIV, primary_node,
			multiplicative_temp_node);

		return multiplicative_expression;
	}
	return primary_node;
}

Node* Parser::primary_expression()
{
	if (tryEat(token_type::NUMBER_TOKEN)) {
		Node* number_node = number();
		
		return number_node;
	}
	else if (tryEat(token_type::IDENTIFIER)) {
		Node* identifier_node = identifier();

		return identifier_node;
	}
	else if (tryEat(token_type::LPAR)) {
		Node* parenthesized_expression_node = parenthesized_expression();

		return parenthesized_expression_node;
	}
	
	error("Primary_expression expected");
}

Node* Parser::parenthesized_expression()
{
	eat(token_type::LPAR);
	lex->next_token();
	
	Node* expression_node = expression();

	eat(token_type::RPAR);
	lex->next_token();

	return expression_node;
}

Node* Parser::number()
{
	eat(token_type::NUMBER_TOKEN);

	string number_str = lex->current_token()->get_lexeme();
	lex->next_token();

	Node* number_node = new Node(node_type::NUMBER, nullptr, nullptr, number_str);
	return number_node;
}

Node* Parser::identifier()
{
	eat(token_type::IDENTIFIER);

	string variable_str = lex->current_token()->get_lexeme();
	lex->next_token();

	Node* identifier_node = new Node(node_type::VARIABLE, nullptr, nullptr, variable_str);
	return identifier_node;
}

void Parser::error(const string message)
{
	cout << "PARSE ERROR!" << message << " ";
	cout << "Current token: " << lex->current_token()->get_lexeme() << endl;

	throw logic_error(message);
}
