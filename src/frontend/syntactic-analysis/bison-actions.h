#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include "../../backend/support/shared.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Program
Program ProgramGrammarAction(StatementSequence* statements);

// Sequences
StatementSequence* StatementSequenceGrammarAction(Statement statement, StatementSequence* next);
AttributeSequence* AttributeSequenceGrammarAction(Attribute attribute, AttributeSequence* next);

// Statements
Statement EntityStatementGrammarAction(Entity entity);

// Statement variants
Entity EntityGrammarAction(const char name[NAMEDATALEN], AttributeSequence* attributes);

// Attributes
Attribute AttributeGrammarAction(const char name[NAMEDATALEN], AttributeType type, AttributeModifier modifier);

#endif
