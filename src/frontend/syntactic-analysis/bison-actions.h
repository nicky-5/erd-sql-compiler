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
Program* ProgramGrammarAction(ObjectList* objects);

// Lists
ObjectList* ObjectListGrammarAction(Object* object, ObjectList* next);
AttributeList* AttributeListGrammarAction(Attribute* attribute, AttributeList* next);

// Objects
Object* EntityObjectGrammarAction(Entity* entity);
Object* RelationObjectGrammarAction(Relation* relation);

// Object variants
Entity* EntityGrammarAction(const char name[NAMEDATALEN], AttributeList* attributes);
Relation* RelationGrammarAction(const char name[NAMEDATALEN], AttributeList* relationAttributeList);

// Attributes
Attribute* AttributeGrammarAction(const char name[NAMEDATALEN], AttributeType type, AttributeModifier modifier);
Attribute* SymbolAttributeGrammarAction(const char name[NAMEDATALEN], const char symbol[NAMEDATALEN]);

#endif
