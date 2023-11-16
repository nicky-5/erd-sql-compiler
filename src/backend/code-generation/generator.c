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

void GenerateKeyConstraints(const char* entityName, AttributeList* node) {
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

void GenerateUniqueConstraint(AttributeList* nodeA, AttributeList* nodeB, AttributeList* nodeC, const char* prefixA, const char* prefixB, const char* prefixC) {
    boolean first = true;
    LogRaw("    UNIQUE (");
    while (nodeA != NULL) {
        if (nodeA->attribute->modifier == KEY) {
            if (!first) {
                LogRaw(", ");
            }
            LogRaw("\"%s$%s\"", prefixA, nodeA->attribute->name);
            first = false;
        }

        nodeA = nodeA->next;
    }
    while (nodeB != NULL) {
        if (nodeB->attribute->modifier == KEY) {
            LogRaw(", \"%s$%s\"", prefixB, nodeB->attribute->name);
            first = false;
        }

        nodeB = nodeB->next;
    }
    while (nodeC != NULL) {
        if (nodeC->attribute->modifier == KEY) {
            LogRaw(", \"%s$%s\"", prefixC, nodeC->attribute->name);
            first = false;
        }

        nodeC = nodeC->next;
    }
    LogRaw("),\n");
}

void GenerateCardinalityConstraints(Link** links, int linksCount) {
    AttributeList* attributes[3];
    LinkModifier cardinality[3];
    for (size_t i = 0; i < linksCount; i++) {
        attributes[i] = links[i]->variant.reference->attributeList;
        cardinality[i] = links[i]->modifier;
    }

    // Unary relation
    if (linksCount == 1 && cardinality[0] == ONE) {
        GenerateUniqueConstraint(attributes[0], NULL, NULL, links[0]->name, NULL, NULL);
    }

    // Binary relation
    if (linksCount == 2) {
        // 1:1 | 1:N
        if (cardinality[0] == ONE) {
            GenerateUniqueConstraint(attributes[0], NULL, NULL, links[0]->name, NULL, NULL);
        }
        if (cardinality[1] == ONE) {
            GenerateUniqueConstraint(attributes[1], NULL, NULL, links[1]->name, NULL, NULL);
        }
        
        // N:M
        if (cardinality[0] == MANY && cardinality[1] == MANY) {
            GenerateUniqueConstraint(attributes[0], attributes[1], NULL, links[0]->name, links[1]->name, NULL);
        }
    }

    // Ternary relation
    if (linksCount == 3) {
        // N:M:P
        if (cardinality[0] == MANY && cardinality[1] == MANY && cardinality[2] == MANY) {
            GenerateUniqueConstraint(attributes[0], attributes[1], attributes[2], links[0]->name, links[1]->name, links[2]->name);
        }

        // 1:N:M
        if (cardinality[0] == MANY && cardinality[1] == MANY && cardinality[2] == ONE) {
            GenerateUniqueConstraint(attributes[0], attributes[1], NULL, links[0]->name, links[1]->name, NULL);
        }
        if (cardinality[0] == MANY && cardinality[1] == ONE && cardinality[2] == MANY) {
            GenerateUniqueConstraint(attributes[0], attributes[2], NULL, links[0]->name, links[2]->name, NULL);
        }
        if (cardinality[0] == ONE && cardinality[1] == MANY && cardinality[2] == MANY) {
            GenerateUniqueConstraint(attributes[1], attributes[2], NULL, links[1]->name, links[2]->name, NULL);
        }

        // 1:1:N
        if (cardinality[0] == ONE && cardinality[1] == ONE && cardinality[2] == MANY) {
            GenerateUniqueConstraint(attributes[0], attributes[2], NULL, links[0]->name, links[2]->name, NULL);
            GenerateUniqueConstraint(attributes[1], attributes[2], NULL, links[1]->name, links[2]->name, NULL);
        }
        if (cardinality[0] == ONE && cardinality[1] == MANY && cardinality[2] == ONE) {
            GenerateUniqueConstraint(attributes[0], attributes[1], NULL, links[0]->name, links[1]->name, NULL);
            GenerateUniqueConstraint(attributes[2], attributes[1], NULL, links[2]->name, links[1]->name, NULL);
        }
        if (cardinality[0] == MANY && cardinality[1] == ONE && cardinality[2] == ONE) {
            GenerateUniqueConstraint(attributes[1], attributes[0], NULL, links[1]->name, links[0]->name, NULL);
            GenerateUniqueConstraint(attributes[2], attributes[0], NULL, links[2]->name, links[0]->name, NULL);
        }

        // 1:1:1
        if (cardinality[0] == ONE && cardinality[1] == ONE && cardinality[2] == ONE) {
            GenerateUniqueConstraint(attributes[0], attributes[1], NULL, links[0]->name, links[1]->name, NULL);
            GenerateUniqueConstraint(attributes[1], attributes[2], NULL, links[1]->name, links[2]->name, NULL);
            GenerateUniqueConstraint(attributes[0], attributes[2], NULL, links[0]->name, links[2]->name, NULL);
        }
    }
}

void GenerateForeignConstraints(AttributeList* list, const char variableName[NAMEDATALEN], const char entityName[NAMEDATALEN]) {
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
    GenerateKeyConstraints(entity->name, entity->attributeList);
    LogRaw(");\n\n");

    AttributeList* attrNode = entity->attributeList;
    while (attrNode != NULL) {
        Attribute* attribute = attrNode->attribute;
        if (attribute->modifier == MULTI) {
            LogRaw("CREATE TABLE \"%s$%s\" (\n", entity->name, attribute->name);
            GenerateKeyAttributes(entity->attributeList, entity->name);
            LogRaw("    \"%s\" %s NOT NULL,\n", attribute->name, getTypeName(attribute->type));
            GenerateForeignConstraints(entity->attributeList, entity->name, entity->name);
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

    GenerateCardinalityConstraints(relation->linkedObjects, relation->linksCount);

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
            GenerateForeignConstraints(reference->attributeList, linkedObjects[i]->name, reference->name);
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
