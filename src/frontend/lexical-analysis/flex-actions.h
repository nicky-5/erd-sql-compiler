#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

// Multiline comment
void BeginCommentPatternAction();
void EndCommentPatternAction();

// Keywords
token EntityKeywordPatternAction();
token KeyKeywordPatternAction();
token RelationKeywordPatternAction();

// Types
token AttributeTypePatternAction(const char* lexeme, const int length, AttributeType type);
token EntityTypePatternAction();
token CompoundTypePatternAction();

// Symbols
token OpenCurlyBracketsPatternAction();
token CloseCurlyBracketsPatternAction();
token OpenSquareBracketsPatternAction();
token CloseSquareBracketsPatternAction();
token OpenParenthesisPatternAction();
token CloseParenthesisPatternAction();
token ColonPatternAction();
token CommaPatternAction();
token QuestionMarkPatternAction();
token OnePatternAction();
token MPatternAction();

// Values
token VarnamePatternAction(const char* lexeme, const int length);

// Unknown pattern
token UnknownPatternAction(const char* lexeme, const int length);

// Ignore pattern
void IgnoredPatternAction(const char* lexeme, const int length);

#endif
