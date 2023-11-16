#include "flex-actions.h"

#include <stdlib.h>
#include <string.h>

#include "../../backend/support/logger.h"

char *copyLexeme(const char *lexeme, const int length) {
    char *lexemeCopy = (char *)calloc(length + 1, sizeof(char));
    strncpy(lexemeCopy, lexeme, length);
    return lexemeCopy;
}

void BeginCommentPatternAction() { LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................"); }

void EndCommentPatternAction() { LogDebug("[Flex] [COMMENT] EndCommentPatternAction.............................."); }

// KEYWORDS START
token EntityKeywordPatternAction() {
    LogDebug("[Flex] EntityKeywordPatternAction: 'entity'.");
    yylval.token = ENTITY_KEYWORD;
    return ENTITY_KEYWORD;
}

token KeyKeywordPatternAction() {
    LogDebug("[Flex] KeyKeywordPatternAction: 'key'.");
    yylval.token = KEY_KEYWORD;
    return KEY_KEYWORD;
}

token RelationKeywordPatternAction() {
    LogDebug("[Flex] RelationKeywordPatternAction: 'relation'.");
    yylval.token = RELATION_KEYWORD;
    return RELATION_KEYWORD;
}

token CompoundKeywordPatternAction() {
    LogDebug("[Flex] CompoundKeywordPatternAction: 'compound'.");
    yylval.token = COMPOUND_KEYWORD;
    return COMPOUND_KEYWORD;
}
// KEYWORDS END

// TYPES START
token AttributeTypePatternAction(const char *lexeme, const int length, AttributeType type) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] AttributeTypePatternAction: '%s'.", lexemeCopy);
    free(lexemeCopy);
    yylval.attributeType = type;
    return ATTRIBUTE_TYPE;
}
// TYPES END

// SYMBOLS START
token OpenCurlyBracketsPatternAction() {
    LogDebug("[Flex] OpenCurlyBracketsPatternAction: '{'.");
    yylval.token = OPEN_CURLY_BRACKETS;
    return OPEN_CURLY_BRACKETS;
}

token CloseCurlyBracketsPatternAction() {
    LogDebug("[Flex] CloseCurlyBracketsPatternAction: '}'.");
    yylval.token = CLOSE_CURLY_BRACKETS;
    return CLOSE_CURLY_BRACKETS;
}

token OpenSquareBracketsPatternAction() {
    LogDebug("[Flex] OpenSquareBracketsPatternAction: '['.");
    yylval.token = OPEN_SQUARE_BRACKETS;
    return OPEN_SQUARE_BRACKETS;
}

token CloseSquareBracketsPatternAction() {
    LogDebug("[Flex] CloseSquareBracketsPatternAction: ']'.");
    yylval.token = CLOSE_SQUARE_BRACKETS;
    return CLOSE_SQUARE_BRACKETS;
}

token OpenParenthesisPatternAction() {
    LogDebug("[Flex] OpenParenthesisPatternAction: '('.");
    yylval.token = OPEN_PARENTHESIS;
    return OPEN_PARENTHESIS;
}

token CloseParenthesisPatternAction() {
    LogDebug("[Flex] CloseParenthesisPatternAction: ')'.");
    yylval.token = CLOSE_PARENTHESIS;
    return CLOSE_PARENTHESIS;
}

token ColonPatternAction() {
    LogDebug("[Flex] ColonPatternAction: ':'.");
    yylval.token = COLON;
    return COLON;
}

token CommaPatternAction() {
    LogDebug("[Flex] CommaPatternAction: ','.");
    yylval.token = COMMA;
    return COMMA;
}

token QuestionMarkPatternAction() {
    LogDebug("[Flex] QuestionMarkPatternAction: '?'.");
    yylval.token = QUESTION_MARK;
    return QUESTION_MARK;
}

token ExclamationMarkPatternAction() {
    LogDebug("[Flex] ExclamationMarkPatternAction: '!'.");
    yylval.token = EXCLAMATION_MARK;
    return EXCLAMATION_MARK;
}

token OnePatternAction() {
    LogDebug("[Flex] OnePatternAction: '1'.");
    yylval.token = NUM_ONE;
    return NUM_ONE;
}

token MPatternAction() {
    LogDebug("[Flex] MPatternAction: 'M'.");
    yylval.token = LETTER_M;
    return LETTER_M;
}
// SYMBOLS END

// VALUES START
token VarnamePatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] VarnamePatternAction: '%s' (length = %d).", lexemeCopy, length);
    strncpy(yylval.varname, lexemeCopy, NAMEDATALEN);
    free(lexemeCopy);
    return VARNAME;
}
// VALUES END

// OTHER START
token UnknownPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
    free(lexemeCopy);
    yylval.token = ERROR;
    return ERROR;
}

void IgnoredPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
    LogText(lexemeCopy, length);
    LogRaw("' (length = %d).\n", length);
    free(lexemeCopy);
}
// OTHER END
