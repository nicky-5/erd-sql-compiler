#include "shared.h"

#include <stdlib.h>
#include <string.h>

void addScopedKey(const char name[NAMEDATALEN]) {
    NameQueue* queue = &state.scopeKeys;
    NameList* node = malloc(sizeof(NameList));
    strncpy(node->name, name, NAMEDATALEN);
    node->next = NULL;

    if (queue->last == NULL) {
        queue->first = node;
        queue->last = node;
    } else {
        queue->last->next = node;
        queue->last = node;
    }
}

NameList* getScopedKeys() {
    NameQueue* queue = &state.scopeKeys;
    NameList* first = queue->first;
    queue->first = NULL;
    queue->last = NULL;
    return first;
}

void addScopedName(const char name[NAMEDATALEN]) {
    NameList* node = malloc(sizeof(NameList));
    strncpy(node->name, name, NAMEDATALEN);
    node->next = state.scopedNames;
    state.scopedNames = node;
}

boolean isNameInScope(const char name[NAMEDATALEN]) {
    NameList* node = state.scopedNames;
    while (node != NULL) {
        if (strncmp(node->name, name, NAMEDATALEN) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

void clearScopedNamesRec(NameList* node) {
    if (node != NULL) {
        clearScopedNamesRec(node->next);
        free(node);
    }
}

void clearScopedNames() {
    clearScopedNamesRec(state.scopedNames);
    state.scopedNames = NULL;
}

void addEntity(const Entity* entityRef) {
    EntityRefList* node = malloc(sizeof(EntityRefList));
    node->entityRef = entityRef;
    node->next = state.entityRefList;
    state.entityRefList = node;
}

const Entity* findEntity(const char name[NAMEDATALEN]) {
    EntityRefList* node = state.entityRefList;
    while (node != NULL) {
        if (strncmp(node->entityRef->name, name, NAMEDATALEN) == 0) {
            return node->entityRef;
        }
        node = node->next;
    }
    return NULL;
}

void addRelation(const Relation* relationRef) {
    RelationRefList* node = malloc(sizeof(RelationRefList));
    node->relationRef = relationRef;
    node->next = state.relationRefList;
    state.relationRefList = node;
}

const Relation* findRelation(const char name[NAMEDATALEN]) {
    RelationRefList* node = state.relationRefList;
    while (node != NULL) {
        if (strncmp(node->relationRef->name, name, NAMEDATALEN) == 0) {
            return node->relationRef;
        }
        node = node->next;
    }
    return NULL;
}
