#include "generator.h"

#include "../support/logger.h"
#include "../support/shared.h"

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

void LogRawKeys(AttributeList* node) {
    boolean first = true;
    while (node != NULL) {
        if (node->attribute->data.modifier == KEY) {
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

void LogRawAttributes(AttributeList* node) {
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        LogRaw("    \"%s\" %s", attribute->name, getTypeName(attribute->type));

        if (attribute->data.modifier != NULLABLE) {
            LogRaw(" NOT NULL");
        }

        if (node->next != NULL) {
            LogRaw(",\n");
        }

        node = node->next;
    }
}

void LogRawRelationKeys(AttributeList* node, const char* name) {
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->data.modifier == KEY) {
            LogRaw("    \"%s$%s\" %s NOT NULL,\n", name, attribute->name, getTypeName(attribute->type));
        }

        node = node->next;
    }
}

void LogRawRelationAttributes(AttributeList* node) {
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->type == REFERENCE) {
            LogRawRelationKeys(attribute->data.reference->attributeList, attribute->name);
        } else {
            LogRaw("    \"%s\" %s", attribute->name, getTypeName(attribute->type));
        }

        node = node->next;
    }
}

void LogRawForeignKey(AttributeList* list, const char name[NAMEDATALEN], const char entityName[NAMEDATALEN]) {
    boolean first = true;
    AttributeList* node = list;
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->data.modifier == KEY) {
            if (first) {
                LogRaw("    FOREIGN KEY(\"%s$%s\"", name, attribute->name);
                first = false;
            } else {
                LogRaw(", \"%s$%s\"", name, attribute->name);
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
        if (attribute->data.modifier == KEY) {
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

void LogRawForeignKeyList(AttributeList* node) {
    boolean first = true;
    while (node != NULL) {
        Attribute* attribute = node->attribute;
        if (attribute->type == REFERENCE) {
            const Object* reference = attribute->data.reference;
            if (first) {
                first = false;
            } else {
                LogRaw(",\n");
            }

            LogRawForeignKey(reference->attributeList, attribute->name, reference->name);
        }

        node = node->next;
    }
    if (!first) {
        LogRaw("\n");
    }
}

void Generator(Program* program) {
    ObjectList* List = program->objectList;
    while (List != NULL) {
        Object* object = List->object;
        LogRaw("CREATE TABLE \"%s\" (\n", object->name);
        switch (object->type) {
            case ENTITY:
                LogRawAttributes(object->attributeList);
                LogRawKeys(object->attributeList);
                break;
            case RELATION:
                LogRawRelationAttributes(object->attributeList);
                LogRawForeignKeyList(object->attributeList);
                break;
        }
        LogRaw(");\n\n");
        List = List->next;
    }
}
