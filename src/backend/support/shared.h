#ifndef SHARED_HEADER
#define SHARED_HEADER

#include <stdio.h>

#include "../semantic-analysis/abstract-semantic-graph.h"

// Descriptor del archivo de entrada que utiliza Bison.
extern FILE* yyin;

// Descriptor del archivo de salida que utiliza Bison.
extern FILE* yyout;

// La cantidad de caracteres en el lexema "yytext". Se debe leer en las
// acciones de Flex, no en Bison.
extern int yyleng;

// Variable global que contiene el número de la línea analizada.
extern int yylineno;

// Token actual en el tope de la pila del analizador Bison.
extern char* yytext;

// Función global de manejo de errores en Bison.
extern void yyerror(const char* string);

// Función global del analizador léxico Flex.
extern int yylex(void);

// Función global del analizador sintáctico Bison.
extern int yyparse(void);

// Emular tipo "boolean".
typedef enum { false = 0, true = 1 } boolean;

// El tipo de los tokens emitidos por Flex.
typedef int token;

typedef struct NameQueue {
    NameList* first;
    NameList* last;
} NameQueue;

// Estado global de toda la aplicación.
typedef struct {
    // Indica si la compilación tuvo problemas hasta el momento.
    boolean succeed;

    // El nodo raíz del AST (se usará cuando se implemente el backend).
    Program* program;

    // Agregar lo que sea necesario para el compilador.
    // Agregar una pila para manipular scopes.
    // Agregar una tabla de símbolos.
    // ...
    NameQueue scopeKeys;

    EntityRefList* entityRefList;
    RelationRefList* relationRefList;

    NameList* scopedNames;
} CompilerState;

void addScopedKey(const char name[NAMEDATALEN]);
NameList* getScopedKeys();

void addScopedName(const char name[NAMEDATALEN]);
boolean isNameInScope(const char name[NAMEDATALEN]);
void clearScopedNames();

void addEntity(const Entity* entityRef);
const Entity* findEntity(const char name[NAMEDATALEN]);

void addRelation(const Relation* relationRef);
const Relation* findRelation(const char name[NAMEDATALEN]);

// El estado se define e inicializa en el archivo "main.c".
extern CompilerState state;

#endif
