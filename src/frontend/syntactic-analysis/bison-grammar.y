%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas.
%union {
	// No-terminales.
	Program* program;

	ObjectList* objectList;
	Object* object;

	AttributeList* attributeList;
	Attribute* attribute;
	AttributeType attributeType;

	Link* link;
	Link** linkArray;
	LinkModifier linkModifier;

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
%token <token> KEY_KEYWORD

// Symbols
%token <token> OPEN_CURLY_BRACKETS
%token <token> CLOSE_CURLY_BRACKETS
%token <token> OPEN_SQUARE_BRACKETS
%token <token> CLOSE_SQUARE_BRACKETS
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> COLON
%token <token> COMMA
%token <token> QUESTION_MARK
%token <token> NUM_ONE
%token <token> LETTER_M

// Values
%token <varname> VARNAME
%token <attributeType> ATTRIBUTE_TYPE
%token <varname> ENTITY_TYPE

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program

%type <objectList> objectList
%type <object> object

%type <object> entity
%type <object> relation

%type <attributeList> attributeList
%type <attribute> attribute

%type <attributeList> relationAttributes
%type <attributeList> relationAttributeList
%type <attribute> relationAttribute

%type <link> linkAttribute
%type <linkModifier> linkModifier

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
	: entity																			{ $$ = $1; }
	| relation																			{ $$ = $1; }
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
	| VARNAME COLON OPEN_SQUARE_BRACKETS ATTRIBUTE_TYPE CLOSE_SQUARE_BRACKETS COMMA		{ $$ = AttributeGrammarAction($1, $3, MULTI); }
	;

relation
	: RELATION_KEYWORD VARNAME[name]
	  OPEN_CURLY_BRACKETS
	  linkAttribute[link0]
	  relationAttributes[attrs] 
	  CLOSE_CURLY_BRACKETS																{ $$ = RelationGrammarAction($name, LinkArrayGrammarAction($link0, NULL, NULL), $attrs); }

	| RELATION_KEYWORD VARNAME[name]
	  OPEN_CURLY_BRACKETS
	  linkAttribute[link0]
	  linkAttribute[link1]
	  relationAttributes[attrs] 
	  CLOSE_CURLY_BRACKETS																{ $$ = RelationGrammarAction($name, LinkArrayGrammarAction($link0, $link1, NULL), $attrs); }

	| RELATION_KEYWORD VARNAME[name]
	  OPEN_CURLY_BRACKETS
	  linkAttribute[link0]
	  linkAttribute[link1]
	  linkAttribute[link2]
	  relationAttributes[attrs] 
	  CLOSE_CURLY_BRACKETS																{ $$ = RelationGrammarAction($name, LinkArrayGrammarAction($link0, $link1, $link2), $attrs); }
	;

linkAttribute
	: VARNAME[name] COLON ENTITY_TYPE[entityType] linkModifier[linkMod] COMMA			{ $$ = LinkGrammarAction($name, $entityType, $linkMod); }
	;

linkModifier
	: OPEN_PARENTHESIS NUM_ONE CLOSE_PARENTHESIS										{ $$ = ONE; }
	| OPEN_PARENTHESIS LETTER_M CLOSE_PARENTHESIS										{ $$ = MANY; }
	;

relationAttributes
	: /* no attributes */																{ $$ = NULL; }
	| relationAttributeList																{ $$ = $1; }
	;

relationAttributeList
	: relationAttribute																	{ $$ = AttributeListGrammarAction($1, NULL); }
	| relationAttribute relationAttributeList											{ $$ = AttributeListGrammarAction($1, $2); }
	;

relationAttribute
	: VARNAME COLON ATTRIBUTE_TYPE COMMA												{ $$ = AttributeGrammarAction($1, $3, NOTNULL); }
	| VARNAME COLON ATTRIBUTE_TYPE QUESTION_MARK COMMA									{ $$ = AttributeGrammarAction($1, $3, NULLABLE); }
	;

%%
