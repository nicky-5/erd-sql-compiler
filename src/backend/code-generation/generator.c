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

void LogDebugAttributes(AttributeSequence* sequence) {
    while (sequence != NULL) {
        Attribute attribute = sequence->attribute;
        LogRaw("    \"%s\" %s", attribute.name, getTypeName(attribute.type));

        if (attribute.modifier == NOTNULL) {
            LogRaw(" NOT NULL");
        }

        if (sequence->next != NULL) {
            LogRaw(",");
        }

        LogRaw("\n");

        sequence = sequence->next;
    }
}

void Generator(Program* program) {
    StatementSequence* sequence = program->statementSequence;
    while (sequence != NULL) {
        Statement statement = sequence->statement;
        switch (statement.type) {
            case ENTITY:
                Entity entity = statement.variant.entity;
                LogRaw("CREATE TABLE \"%s\" (\n", entity.name);
                LogDebugAttributes(entity.attributes);
                LogRaw(");\n");
                break;
        }
        sequence = sequence->next;
    }
}
