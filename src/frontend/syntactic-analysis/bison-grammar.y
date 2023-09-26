%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	Program program;

	StatementSequence* statementSequence;
	Statement statement;

	Entity entity;

	AttributeSequence* attributeSequence;
	Attribute attribute;
	AttributeType attributeType;

	// Terminales.
	token token;
	char varname[64];
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
// Keywords
%token <token> ENTITY_KEYWORD

// Symbols
%token <token> OPEN_CURLY_BRACKETS
%token <token> CLOSE_CURLY_BRACKETS
%token <token> COLON
%token <token> COMMA
%token <token> QUESTION_MARK

// Values
%token <varname> VARNAME
%token <attributeType> ATTRIBUTE_TYPE

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program

%type <statementSequence> statementSequence
%type <statement> statement

%type <entity> entity

%type <attributeSequence> attributeSequence
%type <attribute> attribute

// Reglas de asociatividad y precedencia (de menor a mayor).

// El símbolo inicial de la gramatica.
%start program

%%

program
	: /* empty program */																{ $$ = ProgramGrammarAction(NULL); }
	| statementSequence																	{ $$ = ProgramGrammarAction($1); }
	;

statementSequence
	: statement																			{ $$ = StatementSequenceGrammarAction($1, NULL); }
	| statement statementSequence														{ $$ = StatementSequenceGrammarAction($1, $2); }
	;

statement
	: entity																			{ $$ = EntityStatementGrammarAction($1); }
	;

entity
	: ENTITY_KEYWORD VARNAME OPEN_CURLY_BRACKETS attributeSequence CLOSE_CURLY_BRACKETS	{ $$ = EntityGrammarAction($2, $4); }
	;

attributeSequence
	: attribute																			{ $$ = AttributeSequenceGrammarAction($1, NULL); }
	| attribute COMMA attributeSequence													{ $$ = AttributeSequenceGrammarAction($1, $3); }
	;

attribute
	: VARNAME COLON ATTRIBUTE_TYPE														{ $$ = AttributeGrammarAction($1, $3, NOTNULL); }
	| VARNAME COLON ATTRIBUTE_TYPE QUESTION_MARK										{ $$ = AttributeGrammarAction($1, $3, NULLABLE); }
	;

%%
