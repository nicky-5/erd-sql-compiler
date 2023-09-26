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

void LogRawPrimaryKeys(PrimaryKeyNode* primaryKeys) {
    if (primaryKeys != NULL) {
        LogRaw("    PRIMARY KEY (");
        while (primaryKeys != NULL) {
            LogRaw("%s", primaryKeys->name);
            if (primaryKeys->next != NULL) {
                LogRaw(", ");
            }

            primaryKeys = primaryKeys->next;
        }
        LogRaw(")\n");
    }
}

void LogRawAttributes(AttributeSequence* sequence, PrimaryKeyNode* primaryKeys) {
    while (sequence != NULL) {
        Attribute attribute = sequence->attribute;
        LogRaw("    \"%s\" %s", attribute.name, getTypeName(attribute.type));

        if (attribute.modifier != NULLABLE) {
            LogRaw(" NOT NULL");
        }

        if (sequence->next != NULL || primaryKeys != NULL) {
            LogRaw(",");
        }

        LogRaw("\n");

        sequence = sequence->next;
    }
    LogRawPrimaryKeys(primaryKeys);
}

void Generator(Program* program) {
    StatementSequence* sequence = program->statementSequence;
    while (sequence != NULL) {
        Statement statement = sequence->statement;
        switch (statement.type) {
            case ENTITY:
                Entity entity = statement.variant.entity;
                LogRaw("CREATE TABLE \"%s\" (\n", entity.name);
                LogRawAttributes(entity.attributes, entity.primaryKeys);
                LogRaw(");\n");
                break;
        }
        sequence = sequence->next;
    }
}
