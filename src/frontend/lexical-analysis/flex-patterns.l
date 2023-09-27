%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
varname         [[:alpha:]_][[:alnum:]_]{0,29}
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"entity"                                    { return EntityKeywordPatternAction(); }
"!key"                                      { return PrimaryKeyKeywordPatternAction(); }
"relation"                                  { return RelationKeywordPatternAction(); }

"int"                                       { return AttributeTypePatternAction(yytext, yyleng, INT); }
"integer"                                   { return AttributeTypePatternAction(yytext, yyleng, INT); }
"float"                                     { return AttributeTypePatternAction(yytext, yyleng, FLOAT); }
"double"                                    { return AttributeTypePatternAction(yytext, yyleng, DOUBLE); }

"text"                                      { return AttributeTypePatternAction(yytext, yyleng, TEXT); }
"char"                                      { return AttributeTypePatternAction(yytext, yyleng, CHAR); }

"bool"                                      { return AttributeTypePatternAction(yytext, yyleng, BOOL); }
"boolean"                                   { return AttributeTypePatternAction(yytext, yyleng, BOOL); }

"date"                                      { return AttributeTypePatternAction(yytext, yyleng, DATE); }
"time"                                      { return AttributeTypePatternAction(yytext, yyleng, TIME); }

"entity::"{varname}                         { return EntityTypePatternAction(yytext, yyleng); }

"{"                                         { return OpenCurlyBracketsPatternAction(); }
"}"                                         { return CloseCurlyBracketsPatternAction(); }
":"                                         { return ColonPatternAction(); }
","                                         { return CommaPatternAction(); }
"?"                                         { return QuestionMarkPatternAction(); }

{varname}                                   { return VarnamePatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
