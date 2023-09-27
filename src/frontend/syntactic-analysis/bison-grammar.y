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
	Program* program;

	StatementList* statementList;
	Statement* statement;

	Entity* entity;
	Relation* relation;

	AttributeList* attributeList;
	Attribute* attribute;
	AttributeType attributeType;

	RelationEntityList* relationEntityList;
	RelationEntity* relationEntity;

	// Terminales.
	token token;
	char varname[64];
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
// Keywords
%token <token> ENTITY_KEYWORD
%token <token> RELATION_KEYWORD

// Symbols
%token <token> OPEN_CURLY_BRACKETS
%token <token> CLOSE_CURLY_BRACKETS
%token <token> COLON
%token <token> COMMA
%token <token> QUESTION_MARK
%token <token> PK_KEYWORD

// Values
%token <varname> VARNAME
%token <attributeType> ATTRIBUTE_TYPE
%token <varname> ENTITY_TYPE

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program

%type <statementList> statementList
%type <statement> statement

%type <entity> entity
%type <relation> relation

%type <attributeList> attributeList
%type <attribute> attribute

%type <relationEntityList> relationEntityList
%type <relationEntity> relationEntity

// Reglas de asociatividad y precedencia (de menor a mayor).

// El símbolo inicial de la gramatica.
%start program

%%

program
	: /* empty program */																{ $$ = ProgramGrammarAction(NULL); }
	| statementList																		{ $$ = ProgramGrammarAction($1); }
	;

statementList
	: statement																			{ $$ = StatementListGrammarAction($1, NULL); }
	| statement statementList															{ $$ = StatementListGrammarAction($1, $2); }
	;

statement
	: entity																			{ $$ = EntityStatementGrammarAction($1); }
	| relation																			{ $$ = RelationStatementGrammarAction($1); }
	;

entity
	: ENTITY_KEYWORD VARNAME OPEN_CURLY_BRACKETS 
	  attributeList 
	  CLOSE_CURLY_BRACKETS																{ $$ = EntityGrammarAction($2, $4); }
	;

attributeList
	: attribute																			{ $$ = AttributeListGrammarAction($1, NULL); }
	| attribute COMMA attributeList														{ $$ = AttributeListGrammarAction($1, $3); }
	;

attribute
	: VARNAME COLON ATTRIBUTE_TYPE														{ $$ = AttributeGrammarAction($1, $3, NOTNULL); }
	| VARNAME COLON ATTRIBUTE_TYPE QUESTION_MARK										{ $$ = AttributeGrammarAction($1, $3, NULLABLE); }
	| VARNAME COLON ATTRIBUTE_TYPE PK_KEYWORD											{ $$ = AttributeGrammarAction($1, $3, PK); }
	;

relation
	: RELATION_KEYWORD VARNAME OPEN_CURLY_BRACKETS
	  relationEntityList 
	  CLOSE_CURLY_BRACKETS																{ $$ = RelationGrammarAction($2, $4); }
	;

relationEntityList
	: relationEntity																	{ $$ = RelationEntityListGrammarAction($1, NULL); }
	| relationEntity COMMA relationEntityList											{ $$ = RelationEntityListGrammarAction($1, $3); }
	;

relationEntity
	: VARNAME COLON ENTITY_TYPE															{ $$ = RelationEntityGrammarAction($1, $3); }
	;

%%
