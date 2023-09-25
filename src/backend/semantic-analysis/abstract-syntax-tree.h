#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
 * Se realiza este tipo de definiciones cuando el tipo de dato debe
 * auto-referenciarse, como es el caso de una "Expression", que está compuesta
 * de otras 2 expresiones.
 */
typedef struct StatementSequence StatementSequence;

/**
 * Para cada no-terminal se define una nueva estructura que representa su tipo
 * de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
 */
typedef struct Entity {
    char name[64];
} Entity;

/**
 * En caso de que un no-terminal ("Factor" en este caso), posea más de una
 * regla de producción asociada, se crea además de la estructura un enumerado
 * que identitifque cada reducción posible (cada regla de producción). Luego,
 * se agrega una instancia de este enumerado dentro del nodo del no-terminal.
 *
 * De este modo, al recorrer el AST, es posible determinar qué nodos hijos
 * posee según el valor de este enumerado.
 *
 * typedef enum { EXPRESSION, CONSTANT } FactorType;
 * typedef struct {
 *     FactorType type;
 *     Expression* expression;
 *} Factor;
 */

typedef enum StatementType { ENTITY } StatementType;
typedef struct Statement {
    StatementType type;
    union Data {
        Entity entity;
    } data;
} Statement;

struct StatementSequence {
    Statement statement;
    StatementSequence* next;
};

typedef struct Program {
    StatementSequence* statementSequence;
} Program;

#endif
