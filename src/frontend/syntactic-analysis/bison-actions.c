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
Program* ProgramGrammarAction(StatementList* statementList) {
    LogDebug("[Bison] ProgramGrammarAction");
    Program* program = malloc(sizeof(Program));
    program->statementList = statementList;
    state.program = program;
    return program;
}

StatementList* StatementListGrammarAction(Statement* statement, StatementList* next) {
    LogDebug("[Bison] StatementListGrammarAction");
    StatementList* List = malloc(sizeof(StatementList));
    List->statement = statement;
    List->next = next;
    return List;
}

AttributeList* AttributeListGrammarAction(Attribute* attribute, AttributeList* next) {
    LogDebug("[Bison] AttributeListGrammarAction");
    AttributeList* List = malloc(sizeof(AttributeList));
    List->attribute = attribute;
    List->next = next;
    return List;
}

RelationEntityList* RelationEntityListGrammarAction(RelationEntity* relationEntity, RelationEntityList* next) {
    LogDebug("[Bison] RelationEntityListGrammarAction");
    RelationEntityList* node = malloc(sizeof(RelationEntityList));
    node->relationEntity = relationEntity;
    node->next = next;
    return node;
}

Statement* EntityStatementGrammarAction(Entity* entity) {
    if (entity == NULL) {
        return NULL;
    }

    LogDebug("[Bison] EntityStatementGrammarAction");
    Statement* statement = malloc(sizeof(Statement));
    statement->type = ENTITY;
    statement->variant.entity = entity;
    return statement;
}

Statement* RelationStatementGrammarAction(Relation* relation) {
    if (relation == NULL) {
        return NULL;
    }

    LogDebug("[Bison] RelationStatementGrammarAction");
    Statement* statement = malloc(sizeof(Statement));
    statement->type = RELATION;
    statement->variant.relation = relation;
    return statement;
}

Entity* EntityGrammarAction(const char name[NAMEDATALEN], AttributeList* attributes) {
    if (findEntity(name) != NULL || findRelation(name) != NULL) {
        state.succeed = false;
        LogError("Object redeclaration of name '%s'", name);
        return NULL;
    }

    LogDebug("[Bison] EntityGrammarAction(%s)", name);
    Entity* entity = malloc(sizeof(Entity));
    strncpy(entity->name, name, NAMEDATALEN);
    entity->attributes = attributes;
    entity->keys = getScopedKeys(&state.scopeKeys);
    addEntity(entity);
    clearScopedNames();
    return entity;
}

Relation* RelationGrammarAction(const char name[NAMEDATALEN], RelationEntityList* relationEntityList) {
    if (findEntity(name) != NULL || findRelation(name) != NULL) {
        state.succeed = false;
        LogError("Object redeclaration of name '%s'", name);
        return NULL;
    }

    LogDebug("[Bison] RelationGrammarAction(%s)", name);
    Relation* relation = malloc(sizeof(Relation));
    strncpy(relation->name, name, NAMEDATALEN);
    relation->relationEntityList = relationEntityList;
    addRelation(relation);
    clearScopedNames();
    return relation;
}

Attribute* AttributeGrammarAction(const char name[NAMEDATALEN], AttributeType type, AttributeModifier modifier) {
    if (isNameInScope(name)) {
        state.succeed = false;
        LogError("Object redeclaration in this scope of name '%s'", name);
        return NULL;
    }

    LogDebug("[Bison] AttributeGrammarAction(%s)", name);

    if (modifier == PK) {
        addScopedKey(name);
    }

    Attribute* attribute = malloc(sizeof(Attribute));
    strncpy(attribute->name, name, NAMEDATALEN);
    attribute->type = type;
    attribute->modifier = modifier;
    addScopedName(name);
    return attribute;
}

RelationEntity* RelationEntityGrammarAction(const char name[NAMEDATALEN], const char entityName[NAMEDATALEN]) {
    if (isNameInScope(name)) {
        state.succeed = false;
        LogError("Object redeclaration in this scope of name '%s'", name);
        return NULL;
    }

    const Entity* entityRef = findEntity(entityName);
    if (entityRef == NULL) {
        state.succeed = false;
        LogError("Reference to undefined entity '%s'", name);
        return NULL;
    }

    LogDebug("[Bison] RelationEntityGrammarAction(%s)", name);
    RelationEntity* relationEntity = malloc(sizeof(RelationEntity));
    strncpy(relationEntity->name, name, NAMEDATALEN);
    relationEntity->entityRef = entityRef;
    addScopedName(name);
    return relationEntity;
}
