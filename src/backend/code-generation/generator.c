#include "generator.h"

#include "../support/logger.h"
#include "../support/shared.h"

const char* getTypeName(AttributeType type) {
    switch (type) {
        case INT:
            return "integer";
        case NUMERIC:
            return "numeric";
        case FLOAT:
            return "float";
        case DOUBLE:
            return "double";
        case UUID:
            return "uuid";
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
        case TIMESTAMP:
            return "timestamp";
        case TIMESTAMPTZ:
            return "timestamptz";
    }
    return NULL;
}

void LogRawKeyConstraints(AttributeList* node) {
    boolean first = true;
    while (node != NULL) {
        if (node->attribute->modifier == KEY) {
            if (first) {
                LogRaw(",\n    PRIMARY KEY (");
                first = false;
            } else {
                LogRaw(", ");
            }
            LogRaw("\"%s\"", node->attribute->name);
        }

        node = node->next;
    }
    if (!first) {
        LogRaw(")\n");
    }
}

void LogRawCompoundAttributes(const char* name, AttributeList* node) {
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->type == COMPOUND) {
            
        } else {
            LogRaw("    \"%s$%s\" %s", name, attribute->name, getTypeName(attribute->type));

            if (attribute->modifier != NULLABLE) {
                LogRaw(" NOT NULL");
            }

            if (node->next != NULL) {
                LogRaw(",\n");
            }
        }

        node = node->next;
    }
}

void LogRawAttributes(AttributeList* node) {
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->type == COMPOUND) {
            LogRawCompoundAttributes(attribute->name, attribute->nested);
        } else {
            LogRaw("    \"%s\" %s", attribute->name, getTypeName(attribute->type));

            if (attribute->modifier != NULLABLE) {
                LogRaw(" NOT NULL");
            }

            if (node->next != NULL) {
                LogRaw(",\n");
            }
        }

        node = node->next;
    }
}

void LogRawForeignKeys(AttributeList* node, const char* variableName) {
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->modifier == KEY) {
            LogRaw("    \"%s$%s\" %s NOT NULL,\n", variableName, attribute->name, getTypeName(attribute->type));
        }

        node = node->next;
    }
}

void LogRawForeignKeyConstraints(AttributeList* list, const char variableName[NAMEDATALEN], const char entityName[NAMEDATALEN]) {
    boolean first = true;
    AttributeList* node = list;
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->modifier == KEY) {
            if (first) {
                LogRaw("    FOREIGN KEY(\"%s$%s\"", variableName, attribute->name);
                first = false;
            } else {
                LogRaw(", \"%s$%s\"", variableName, attribute->name);
            }
        }
        node = node->next;
    }

    if (!first) {
        LogRaw(") REFERENCES \"%s\"(", entityName);
    }

    first = true;
    node = list;
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->modifier == KEY) {
            if (first) {
                LogRaw("\"%s\"", attribute->name);
                first = false;
            } else {
                LogRaw(", \"%s\"", attribute->name);
            }
        }
        node = node->next;
    }

    if (!first) {
        LogRaw(")");
    }
}

void LogRawLinkAttributes(Link** linkedObjects) {
    for (size_t i = 0; i < 3; i++) {
        if (linkedObjects[i] == NULL) {
            break;
        }
        
        if (linkedObjects[i]->type == REFERENCE) {
            const Object* reference = linkedObjects[i]->variant.reference;
            LogRawForeignKeys(reference->attributeList, linkedObjects[i]->name);
        }
    }
}

void LogRawLinkConstraints(Link** linkedObjects) {
    boolean first = true;
    for (size_t i = 0; i < 3; i++) {
        if (linkedObjects[i] == NULL) {
            break;
        }
        if (linkedObjects[i]->type == REFERENCE) {
            if (first) {
                first = false;
            } else {
                LogRaw(",\n");
            }

            const Object* reference = linkedObjects[i]->variant.reference;
            LogRawForeignKeyConstraints(reference->attributeList, linkedObjects[i]->name, reference->name);
        }
    }
    if (!first) {
        LogRaw("\n");
    }
}

void Generator(Program* program) {
    ObjectList* node = program->objectList;
    while (node != NULL) {
        Object* object = node->object;
        if (object->type == ENTITY) {
            LogRaw("CREATE TABLE \"%s\" (\n", object->name);
            LogRawAttributes(object->attributeList);
            LogRawKeyConstraints(object->attributeList);
            LogRaw(");\n\n");
        }
        node = node->next;
    }

    node = program->objectList;
    while (node != NULL) {
        Object* object = node->object;
        if (object->type == RELATION) {
            LogRaw("CREATE TABLE \"%s\" (\n", object->name);
            LogRawLinkAttributes(object->linkedObjects);
            LogRawLinkConstraints(object->linkedObjects);
            LogRaw(");\n\n");
        }
        node = node->next;
    }
}
