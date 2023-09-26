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
        LogDebug("[Generator]     \"%s\" %s,", attribute.name, getTypeName(attribute.type));
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
                LogDebug("[Generator] CREATE TABLE \"%s\" (", entity.name);
                LogDebugAttributes(entity.attributes);
                LogDebug("[Generator] );");
                break;
        }
        sequence = sequence->next;
    }
}
