#include "bison-actions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"

/**
 * Implementación de "bison-actions.h".
 */

/**
 * Esta función se ejecuta cada vez que se emite un error de sintaxis.
 */
void yyerror(const char* string) {
    LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
    for (int i = 0; i < yyleng; ++i) {
        switch (yytext[i]) {
            case '\n':
                LogErrorRaw("\\n");
            default:
                LogErrorRaw("%c", yytext[i]);
        }
    }
    LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
 * Esta acción se corresponde con el no-terminal que representa el símbolo
 * inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
 * indica que efectivamente el programa de entrada se pudo generar con esta
 * gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
 */
Program ProgramGrammarAction(StatementSequence* statementSequence) {
    LogDebug("[Bison] ProgramGrammarAction");
    /*
     * "state" es una variable global que almacena el estado del compilador,
     * cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
     * es utilizada en la función "main".
     */
    state.succeed = true;
    /*
     * Por otro lado, "result" contiene el resultado de aplicar el análisis
     * sintáctico mediante Bison, y almacenar el nood raíz del AST construido
     * en esta variable. Para el ejemplo de la calculadora, no hay AST porque
     * la expresión se computa on-the-fly, y es la razón por la cual esta
     * variable es un simple entero, en lugar de un nodo.
     */
    Program program;
    program.statementSequence = statementSequence;
    state.program = program;
    return program;
}

StatementSequence* StatementSequenceGrammarAction(Statement statement, StatementSequence* next) {
    LogDebug("[Bison] StatementSequenceGrammarAction");
    StatementSequence* sequence = malloc(sizeof(StatementSequence));
    sequence->statement = statement;
    sequence->next = next;
    return sequence;
}

AttributeSequence* AttributeSequenceGrammarAction(Attribute attribute, AttributeSequence* next) {
    LogDebug("[Bison] AttributeSequenceGrammarAction");
    AttributeSequence* sequence = malloc(sizeof(AttributeSequence));
    sequence->attribute = attribute;
    sequence->next = next;
    return sequence;
}

Statement EntityStatementGrammarAction(Entity entity) {
    LogDebug("[Bison] EntityStatementGrammarAction");
    Statement statement;
    statement.type = ENTITY;
    statement.variant.entity = entity;
    return statement;
}

Entity EntityGrammarAction(const char name[64], AttributeSequence* attributes) {
    LogDebug("[Bison] EntityGrammarAction(%s)", name);
    Entity entity;
    strncpy(entity.name, name, 64);
    entity.attributes = attributes;
    return entity;
}

Attribute AttributeGrammarAction(const char name[64], AttributeType type, AttributeModifier modifier) {
    LogDebug("[Bison] AttributeGrammarAction(%s)", name);
    Attribute attribute;
    strncpy(attribute.name, name, 64);
    attribute.type = type;
    attribute.modifier = modifier;
    return attribute;
}
