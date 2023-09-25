#include "generator.h"

#include "../support/logger.h"

/**
 * Implementación de "generator.h".
 */

void Generator(Program* program) {
    StatementSequence* sequence = program->statementSequence;
    while (sequence != NULL) {
        Statement statement = sequence->statement;
        switch (statement.type) {
            case ENTITY:
                Entity entity = statement.data.entity;
                LogDebug("[Generator] CREATE TABLE %s ( );", entity.name);
                break;
        }
        sequence = sequence->next;
    }
}
