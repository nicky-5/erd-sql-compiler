%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas.
%union {
	// No-terminales.
	Program* program;

	ObjectList* objectList;
	Object* object;

	Entity* entity;
	Relation* relation;

	AttributeList* attributeList;
	Attribute* attribute;
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
%token <token> RELATION_KEYWORD

// Symbols
%token <token> OPEN_CURLY_BRACKETS
%token <token> CLOSE_CURLY_BRACKETS
%token <token> COLON
%token <token> COMMA
%token <token> QUESTION_MARK
%token <token> KEY_KEYWORD

// Values
%token <varname> VARNAME
%token <attributeType> ATTRIBUTE_TYPE
%token <varname> ENTITY_TYPE

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program

%type <objectList> objectList
%type <object> object

%type <entity> entity
%type <relation> relation

%type <attributeList> attributeList
%type <attribute> attribute

%type <attributeList> relationAttributeList
%type <attribute> relationAttribute

// El símbolo inicial de la gramatica.
%start program

%%

program
	: /* empty program */																{ $$ = ProgramGrammarAction(NULL); }
	| objectList																		{ $$ = ProgramGrammarAction($1); }
	;

objectList
	: object																			{ $$ = ObjectListGrammarAction($1, NULL); }
	| object objectList 																{ $$ = ObjectListGrammarAction($1, $2); }
	;

object
	: entity																			{ $$ = EntityObjectGrammarAction($1); }
	| relation																			{ $$ = RelationObjectGrammarAction($1); }
	;

entity
	: ENTITY_KEYWORD VARNAME[name] OPEN_CURLY_BRACKETS 
	  attributeList[attrs] 
	  CLOSE_CURLY_BRACKETS																{ $$ = EntityGrammarAction($name, $attrs); }
	;

attributeList
	: attribute																			{ $$ = AttributeListGrammarAction($1, NULL); }
	| attribute attributeList															{ $$ = AttributeListGrammarAction($1, $2); }
	;

attribute
	: VARNAME COLON ATTRIBUTE_TYPE COMMA												{ $$ = AttributeGrammarAction($1, $3, NOTNULL); }
	| VARNAME COLON ATTRIBUTE_TYPE QUESTION_MARK COMMA									{ $$ = AttributeGrammarAction($1, $3, NULLABLE); }
	| VARNAME COLON ATTRIBUTE_TYPE KEY_KEYWORD COMMA									{ $$ = AttributeGrammarAction($1, $3, KEY); }
	;

relation
	: RELATION_KEYWORD VARNAME OPEN_CURLY_BRACKETS
	  relationAttributeList 
	  CLOSE_CURLY_BRACKETS																{ $$ = RelationGrammarAction($2, $4); }
	;

relationAttributeList
	: relationAttribute																	{ $$ = AttributeListGrammarAction($1, NULL); }
	| relationAttribute relationAttributeList											{ $$ = AttributeListGrammarAction($1, $2); }
	;

relationAttribute
	: VARNAME COLON ENTITY_TYPE COMMA													{ $$ = SymbolAttributeGrammarAction($1, $3); }
	;

%%
