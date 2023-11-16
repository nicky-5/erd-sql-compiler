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

void GenerateCompoundAttributes(const char* name, AttributeList* node) {
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

void GenerateAttributes(AttributeList* node) {
    boolean first = true;
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->type == COMPOUND) {
            if (!first) {
                LogRaw(",\n");
            }
            GenerateCompoundAttributes(attribute->name, attribute->nested);
            first = false;
        } else if (attribute->modifier != MULTI) {
            if (!first) {
                LogRaw(",\n");
            }
            first = false;

            LogRaw("    \"%s\" %s", attribute->name, getTypeName(attribute->type));

            if (attribute->modifier != NULLABLE) {
                LogRaw(" NOT NULL");
            }
        }

        node = node->next;
    }
}

void GenerateEntityConstraints(const char* entityName, AttributeList* node) {
    AttributeList* start = node;

    boolean first = true;
    LogRaw(",\n    PRIMARY KEY (");
    while (node != NULL) {
        if (node->attribute->modifier == KEY) {
            if (!first) {
                LogRaw(", ");
            }
            LogRaw("\"%s\"", node->attribute->name);
            first = false;
        }

        node = node->next;
    }
    LogRaw(")\n");
}

void GenerateKeyAttributes(AttributeList* node, const char* variableName) {
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->modifier == KEY) {
            LogRaw("    \"%s$%s\" %s NOT NULL,\n", variableName, attribute->name, getTypeName(attribute->type));
        }

        node = node->next;
    }
}

void GenerateRelationConstraints(AttributeList* list, const char variableName[NAMEDATALEN], const char entityName[NAMEDATALEN]) {
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

void GenerateEntity(Object* entity) {
    LogRaw("CREATE TABLE \"%s\" (\n", entity->name);
    GenerateAttributes(entity->attributeList);
    GenerateEntityConstraints(entity->name, entity->attributeList);
    LogRaw(");\n\n");

    AttributeList* attrNode = entity->attributeList;
    while (attrNode != NULL) {
        Attribute* attribute = attrNode->attribute;
        if (attribute->modifier == MULTI) {
            LogRaw("CREATE TABLE \"%s$%s\" (\n", entity->name, attribute->name);
            GenerateKeyAttributes(entity->attributeList, entity->name);
            LogRaw("    \"%s\" %s NOT NULL,\n", attribute->name, getTypeName(attribute->type));
            GenerateRelationConstraints(entity->attributeList, entity->name, entity->name);
            LogRaw("\n);\n\n");
        }
        attrNode = attrNode->next;
    }
}

void GenerateRelation(Object* relation) {
    LogRaw("CREATE TABLE \"%s\" (\n", relation->name);
    Link** linkedObjects = relation->linkedObjects;

    for (size_t i = 0; i < 3; i++) {
        if (linkedObjects[i] == NULL) {
            break;
        }
        
        if (linkedObjects[i]->type == REFERENCE) {
            const Object* reference = linkedObjects[i]->variant.reference;
            GenerateKeyAttributes(reference->attributeList, linkedObjects[i]->name);
        }
    }

    GenerateAttributes(relation->attributeList);
    if (relation->attributeList != NULL) {
        LogRaw(",\n");
    }

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
            GenerateRelationConstraints(reference->attributeList, linkedObjects[i]->name, reference->name);
        }
    }
    if (!first) {
        LogRaw("\n");
    }

    LogRaw(");\n\n");
}

void Generator(Program* program) {
    ObjectList* node = program->objectList;
    while (node != NULL) {
        Object* object = node->object;
        if (object->type == ENTITY) {
            GenerateEntity(object);
        }
        node = node->next;
    }

    node = program->objectList;
    while (node != NULL) {
        Object* object = node->object;
        if (object->type == RELATION) {
            GenerateRelation(object);
        }
        node = node->next;
    }
}
