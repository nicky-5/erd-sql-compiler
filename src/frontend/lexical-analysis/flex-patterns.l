%{

#include "flex-actions.h"

%}

/* Handle unknown lexemmes explicitly */
%option nodefault

/* Scan only one file */
%option noyywrap

/* Enable line number global variable */
%option yylineno

/* Flex contexts */
%x COMMENT

/* Reusable patters */
varname         [[:alpha:]_][[:alnum:]_]{0,29}
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"entity"                                    { return EntityKeywordPatternAction(); }
"!key"                                      { return KeyKeywordPatternAction(); }
"relation"                                  { return RelationKeywordPatternAction(); }

"int"                                       { return AttributeTypePatternAction(yytext, yyleng, INT); }
"numeric"                                   { return AttributeTypePatternAction(yytext, yyleng, NUMERIC); }
"float"                                     { return AttributeTypePatternAction(yytext, yyleng, FLOAT); }
"double"                                    { return AttributeTypePatternAction(yytext, yyleng, DOUBLE); }

"uuid"                                      { return AttributeTypePatternAction(yytext, yyleng, UUID); }
"text"                                      { return AttributeTypePatternAction(yytext, yyleng, TEXT); }
"char"                                      { return AttributeTypePatternAction(yytext, yyleng, CHAR); }

"bool"                                      { return AttributeTypePatternAction(yytext, yyleng, BOOL); }

"date"                                      { return AttributeTypePatternAction(yytext, yyleng, DATE); }
"time"                                      { return AttributeTypePatternAction(yytext, yyleng, TIME); }
"timestamp"                                 { return AttributeTypePatternAction(yytext, yyleng, TIMESTAMP); }
"timestamptz"                               { return AttributeTypePatternAction(yytext, yyleng, TIMESTAMPTZ); }

"entity::"                                  { return EntityTypePatternAction(); }
"compound::"                                { return CompoundTypePatternAction(); }

"{"                                         { return OpenCurlyBracketsPatternAction(); }
"}"                                         { return CloseCurlyBracketsPatternAction(); }
"["                                         { return OpenSquareBracketsPatternAction(); }
"]"                                         { return CloseSquareBracketsPatternAction(); }
"("                                         { return OpenParenthesisPatternAction(); }
")"                                         { return CloseParenthesisPatternAction(); }
":"                                         { return ColonPatternAction(); }
","                                         { return CommaPatternAction(); }
"?"                                         { return QuestionMarkPatternAction(); }
"1"                                         { return OnePatternAction(); }
"M"                                         { return MPatternAction(); }

{varname}                                   { return VarnamePatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
