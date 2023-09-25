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

token EntityPatternAction() {
    LogDebug("[Flex] EntityPatternAction: 'entity'.");
    yylval.token = ENTITY_KEYWORD;
    return ENTITY_KEYWORD;
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

token VarnamePatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] VarnamePatternAction: '%s' (length = %d).", lexemeCopy, length);
    strncpy(yylval.varname, lexemeCopy, 64);
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
