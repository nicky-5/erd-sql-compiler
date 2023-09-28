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
    for (size_t i = 0; i < 3; i++) {
        Link* link = relation->linkedObjects[i];

        if (link == NULL) {
            break;
        }

        if (link->type == SYMBOL) {
            LogDebug("[Linker] Linking symbol '%s' with reference to 'entity::%s'", link->name, link->variant.symbol);
            const Object* reference = getReference(program->objectList, link->variant.symbol);
            if (reference == NULL) {
                return 1;
            }

            link->type = REFERENCE;
            link->variant.reference = reference;
        }
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
            LogDebug("[Linker] Found relation '%s'", object->name);
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
    LogDebug("[Bison] ObjectListGrammarAction(name='%s', type=%d)", object->name, object->type);
    ObjectList* node = malloc(sizeof(ObjectList));
    node->object = object;
    node->next = next;
    return node;
}

AttributeList* AttributeListGrammarAction(Attribute* attribute, AttributeList* next) {
    LogDebug("[Bison] AttributeListGrammarAction(name='%s', type=%d)", attribute->name, attribute->type);
    AttributeList* node = malloc(sizeof(AttributeList));
    node->attribute = attribute;
    node->next = next;
    return node;
}

Object* EntityGrammarAction(const char name[NAMEDATALEN], AttributeList* attributes) {
    LogDebug("[Bison] EntityGrammarAction(name='%s')", name);
    Object* entity = malloc(sizeof(Object));
    strncpy(entity->name, name, NAMEDATALEN);
    entity->type = ENTITY;
    entity->attributeList = attributes;
    return entity;
}

Object* RelationGrammarAction(const char name[NAMEDATALEN], Link** links, AttributeList* attributes) {
    LogDebug("[Bison] RelationGrammarAction(name='%s', [0]=%x, [1]=%x, [2]=%x)", name, links[0], links[1], links[2]);
    Object* relation = malloc(sizeof(Object));
    strncpy(relation->name, name, NAMEDATALEN);
    relation->type = RELATION;
    relation->attributeList = attributes;
    relation->linkedObjects = links;
    return relation;
}

Attribute* AttributeGrammarAction(const char name[NAMEDATALEN], AttributeType type, AttributeModifier modifier) {
    LogDebug("[Bison] AttributeGrammarAction(name='%s', type=%d, modifier=%d)", name, type, modifier);

    Attribute* attribute = malloc(sizeof(Attribute));
    strncpy(attribute->name, name, NAMEDATALEN);
    attribute->type = type;
    attribute->modifier = modifier;
    return attribute;
}

Link* LinkGrammarAction(const char name[NAMEDATALEN], const char symbol[NAMEDATALEN], LinkModifier modifier) {
    LogDebug("[Bison] LinkGrammarAction(name='%s', symbol='%s', modifier=%d)", name, symbol, modifier);
    Link* link = malloc(sizeof(Link));
    link->type = SYMBOL;
    link->modifier = modifier;
    strncpy(link->variant.symbol, symbol, NAMEDATALEN);
    strncpy(link->name, name, NAMEDATALEN);
    return link;
}

Link** LinkArrayGrammarAction(Link* linkA, Link* linkB, Link* linkC) {
    LogDebug("[Bison] LinkArrayGrammarAction([0]=%x, [1]=%x, [2]=%x)", linkA, linkB, linkC);
    Link** array = malloc(sizeof(Link* [3]));
    array[0] = linkA;
    array[1] = linkB;
    array[2] = linkC;
    return array;
}
