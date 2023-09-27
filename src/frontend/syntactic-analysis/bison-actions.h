#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/semantic-analysis/abstract-semantic-graph.h"
#include "../../backend/support/shared.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Program
Program* ProgramGrammarAction(StatementList* statements);

// Lists
StatementList* StatementListGrammarAction(Statement* statement, StatementList* next);
AttributeList* AttributeListGrammarAction(Attribute* attribute, AttributeList* next);
RelationEntityList* RelationEntityListGrammarAction(RelationEntity* relationEntity, RelationEntityList* next);

// Statements
Statement* EntityStatementGrammarAction(Entity* entity);
Statement* RelationStatementGrammarAction(Relation* relation);

// Statement variants
Entity* EntityGrammarAction(const char name[NAMEDATALEN], AttributeList* attributes);
Relation* RelationGrammarAction(const char name[NAMEDATALEN], RelationEntityList* relationEntityList);

// Attributes
Attribute* AttributeGrammarAction(const char name[NAMEDATALEN], AttributeType type, AttributeModifier modifier);
RelationEntity* RelationEntityGrammarAction(const char name[NAMEDATALEN], const char entityName[NAMEDATALEN]);

#endif
