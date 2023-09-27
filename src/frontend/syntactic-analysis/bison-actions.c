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

const Object* getReference(ObjectList* list, const char symbol[NAMEDATALEN]) {
    ObjectList* node = list;
    while (node != NULL) {
        if (strncmp(node->object->name, symbol, NAMEDATALEN) == 0) {
            return node->object;
        }

        node = node->next;
    }
    return NULL;
}

const Attribute* getLocalReference(AttributeList* list, const char name[NAMEDATALEN]) {
    AttributeList* node = list;
    while (node != NULL) {
        if (strncmp(node->attribute->name, name, NAMEDATALEN) == 0) {
            return node->attribute;
        }

        node = node->next;
    }
    return NULL;
}

int checkLocalRedeclaration(AttributeList* list) {
    AttributeList* node = list;
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (getLocalReference(node->next, attribute->name) != NULL) {
            LogError("[Linker] Duplicate symbol '%s'", attribute->name);
            return 1;
        }

        node = node->next;
    }
    return 0;
}

int linkRelation(Program* program, Object* relation) {
    AttributeList* node = relation->attributeList;
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->type == SYMBOL) {
            const Object* reference = getReference(program->objectList, attribute->data.symbol);
            if (reference != NULL && reference->type == ENTITY) {
                attribute->type = REFERENCE;
                attribute->data.reference = reference;
            } else {
                LogError("[Linker] Missing definition '%s'", attribute->data.symbol);
                return 1;
            }
        }

        node = node->next;
    }
    return 0;
}

int Linker(Program* program) {
    ObjectList* node = program->objectList;
    while (node != NULL) {
        Object* object = node->object;

        if (getReference(node->next, object->name) != NULL) {
            LogError("[Linker] Duplicate symbol '%s'", object->name);
            return 1;
        }

        if (checkLocalRedeclaration(object->attributeList) != 0) {
            return 2;
        }

        if (object->type == RELATION) {
            if (linkRelation(program, object) != 0) {
                return 3;
            }
        }

        node = node->next;
    }

    return 0;
}

/**
 * Esta acción se corresponde con el no-terminal que representa el símbolo
 * inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
 * indica que efectivamente el programa de entrada se pudo generar con esta
 * gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
 */
Program* ProgramGrammarAction(ObjectList* objectList) {
    LogDebug("[Bison] ProgramGrammarAction");
    Program* program = malloc(sizeof(Program));
    program->objectList = objectList;

    state.succeed = Linker(program) == 0;

    state.program = program;
    return program;
}

ObjectList* ObjectListGrammarAction(Object* object, ObjectList* next) {
    LogDebug("[Bison] ObjectListGrammarAction");
    ObjectList* node = malloc(sizeof(ObjectList));
    node->object = object;
    node->next = next;
    return node;
}

AttributeList* AttributeListGrammarAction(Attribute* attribute, AttributeList* next) {
    LogDebug("[Bison] AttributeListGrammarAction");
    AttributeList* node = malloc(sizeof(AttributeList));
    node->attribute = attribute;
    node->next = next;
    return node;
}

Object* EntityObjectGrammarAction(Entity* entity) {
    LogDebug("[Bison] EntityObjectGrammarAction");
    Object* object = malloc(sizeof(Object));
    object->type = ENTITY;
    object->attributeList = entity->attributeList;
    strncpy(object->name, entity->name, NAMEDATALEN);
    free(entity);
    return object;
}

Object* RelationObjectGrammarAction(Relation* relation) {
    LogDebug("[Bison] RelationObjectGrammarAction");
    Object* object = malloc(sizeof(Object));
    object->type = RELATION;
    object->attributeList = relation->attributeList;
    strncpy(object->name, relation->name, NAMEDATALEN);
    free(relation);
    return object;
}

Entity* EntityGrammarAction(const char name[NAMEDATALEN], AttributeList* attributes) {
    LogDebug("[Bison] EntityGrammarAction(%s)", name);
    Entity* entity = malloc(sizeof(Entity));
    strncpy(entity->name, name, NAMEDATALEN);
    entity->attributeList = attributes;
    return entity;
}

Relation* RelationGrammarAction(const char name[NAMEDATALEN], AttributeList* attributes) {
    LogDebug("[Bison] RelationGrammarAction(%s)", name);
    Relation* relation = malloc(sizeof(Relation));
    strncpy(relation->name, name, NAMEDATALEN);
    relation->attributeList = attributes;
    return relation;
}

Attribute* AttributeGrammarAction(const char name[NAMEDATALEN], AttributeType type, AttributeModifier modifier) {
    LogDebug("[Bison] AttributeGrammarAction(%s)", name);

    Attribute* attribute = malloc(sizeof(Attribute));
    strncpy(attribute->name, name, NAMEDATALEN);
    attribute->type = type;
    attribute->data.modifier = modifier;
    return attribute;
}

Attribute* SymbolAttributeGrammarAction(const char name[NAMEDATALEN], const char symbol[NAMEDATALEN]) {
    LogDebug("[Bison] SymbolAttributeGrammarAction(%s)", name);
    Attribute* attribute = malloc(sizeof(Attribute));
    attribute->type = SYMBOL;
    strncpy(attribute->name, name, NAMEDATALEN);
    strncpy(attribute->data.symbol, symbol, NAMEDATALEN);
    return attribute;
}
