#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/semantic-analysis/abstract-semantic-graph.h"
#include "../../backend/support/shared.h"

// Program
Program* ProgramGrammarAction(ObjectList* objects);

// Lists
ObjectList* ObjectListGrammarAction(Object* object, ObjectList* next);
AttributeList* AttributeListGrammarAction(Attribute* attribute, AttributeList* next);

// Object variants
Object* EntityGrammarAction(const char name[NAMEDATALEN], AttributeList* attributes);
Object* RelationGrammarAction(const char name[NAMEDATALEN], Link** links, AttributeList* relationAttributeList);

// Attributes
Attribute* AttributeGrammarAction(const char name[NAMEDATALEN], AttributeType type, AttributeModifier modifier);

// Links
Link* LinkGrammarAction(const char name[NAMEDATALEN], const char symbol[NAMEDATALEN], LinkModifier modifier);
Link** LinkArrayGrammarAction(Link* linkA, Link* linkB, Link* linkC);

#endif
