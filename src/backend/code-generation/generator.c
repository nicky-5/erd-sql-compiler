#include "generator.h"

#include "../support/logger.h"

/**
 * Implementación de "generator.h".
 */

const char* getTypeName(AttributeType type) {
    switch (type) {
        case INT:
            return "integer";
        case FLOAT:
            return "float";
        case DOUBLE:
            return "double";
        case TEXT:
            return "text";
        case CHAR:
            return "char";
        case BOOL:
            return "boolean";
        case DATE:
            return "date";
        case TIME:
            return "time";
    }
    return NULL;
}

void LogRawPrimaryKeys(NameList* keys) {
    if (keys != NULL) {
        LogRaw("    PRIMARY KEY (");
        while (keys != NULL) {
            LogRaw("%s", keys->name);
            if (keys->next != NULL) {
                LogRaw(", ");
            }

            keys = keys->next;
        }
        LogRaw(")\n");
    }
}

void LogRawAttributes(AttributeList* List, NameList* keys) {
    while (List != NULL) {
        Attribute* attribute = List->attribute;
        LogRaw("    \"%s\" %s", attribute->name, getTypeName(attribute->type));

        if (attribute->modifier != NULLABLE) {
            LogRaw(" NOT NULL");
        }

        if (List->next != NULL || keys != NULL) {
            LogRaw(",");
        }

        LogRaw("\n");

        List = List->next;
    }
    LogRawPrimaryKeys(keys);
}

void Generator(Program* program) {
    StatementList* List = program->statementList;
    while (List != NULL) {
        Statement* statement = List->statement;
        switch (statement->type) {
            case ENTITY:
                Entity* entity = statement->variant.entity;
                LogRaw("CREATE TABLE \"%s\" (\n", entity->name);
                LogRawAttributes(entity->attributes, entity->keys);
                LogRaw(");\n");
                break;
        }
        List = List->next;
    }
}
