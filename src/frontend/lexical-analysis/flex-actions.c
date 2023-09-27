#include "flex-actions.h"

#include <stdlib.h>
#include <string.h>

#include "../../backend/support/logger.h"

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char *copyLexeme(const char *lexeme, const int length) {
    char *lexemeCopy = (char *)calloc(length + 1, sizeof(char));
    strncpy(lexemeCopy, lexeme, length);
    return lexemeCopy;
}

void BeginCommentPatternAction() { LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................"); }

void EndCommentPatternAction() { LogDebug("[Flex] [COMMENT] EndCommentPatternAction.............................."); }

token EntityKeywordPatternAction() {
    LogDebug("[Flex] EntityKeywordPatternAction: 'entity'.");
    yylval.token = ENTITY_KEYWORD;
    return ENTITY_KEYWORD;
}

token PrimaryKeyKeywordPatternAction() {
    LogDebug("[Flex] PrimaryKeyKeywordPatternAction: 'PK'.");
    yylval.token = PK_KEYWORD;
    return PK_KEYWORD;
}

token RelationKeywordPatternAction() {
    LogDebug("[Flex] RelationKeywordPatternAction: 'relation'.");
    yylval.token = RELATION_KEYWORD;
    return RELATION_KEYWORD;
}

token AttributeTypePatternAction(const char *lexeme, const int length, AttributeType type) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] AttributeTypePatternAction: '%s'.", lexemeCopy);
    free(lexemeCopy);
    yylval.attributeType = type;
    return ATTRIBUTE_TYPE;
}

token EntityTypePatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] EntityTypePatternAction: '%s'.", lexemeCopy);
    free(lexemeCopy);
    strncpy(yylval.varname, lexemeCopy + 8, NAMEDATALEN);
    return ENTITY_TYPE;
}

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

token VarnamePatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] VarnamePatternAction: '%s' (length = %d).", lexemeCopy, length);
    strncpy(yylval.varname, lexemeCopy, NAMEDATALEN);
    free(lexemeCopy);
    return VARNAME;
}

token UnknownPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
    free(lexemeCopy);
    yylval.token = ERROR;
    // Al emitir este token, el compilador aborta la ejecución.
    return ERROR;
}

void IgnoredPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
    LogText(lexemeCopy, length);
    LogRaw("' (length = %d).\n", length);
    free(lexemeCopy);
    // Como no debe hacer nada con el patrón, solo se loguea en consola.
    // No se emite ningún token.
}
