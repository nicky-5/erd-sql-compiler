#ifndef ABSTRACT_SEMANTIC_GRAPH_HEADER
#define ABSTRACT_SEMANTIC_GRAPH_HEADER

#define NAMEDATALEN 31

typedef enum ObjectType { ENTITY, RELATION } ObjectType;
typedef enum AttributeType { INT, FLOAT, DOUBLE, TEXT, CHAR, BOOL, DATE, TIME, REFERENCE, SYMBOL } AttributeType;
typedef enum AttributeModifier { NOTNULL, NULLABLE, KEY } AttributeModifier;

typedef struct Object Object;
typedef struct ObjectList ObjectList;

typedef struct AttributeList AttributeList;

typedef struct EntityList EntityList;
typedef struct RelationList RelationList;

typedef struct SymbolList SymbolList;

typedef struct Relation {
    char name[NAMEDATALEN];
    AttributeList* attributeList;
} Relation;

typedef struct Entity {
    char name[NAMEDATALEN];
    AttributeList* attributeList;
} Entity;

typedef struct Attribute {
    char name[NAMEDATALEN];
    AttributeType type;
    union AttributeData {
        AttributeModifier modifier;
        const Object* reference;
        char symbol[NAMEDATALEN];
    } data;
} Attribute;

struct AttributeList {
    Attribute* attribute;
    AttributeList* next;
};

struct Object {
    ObjectType type;
    char name[NAMEDATALEN];
    AttributeList* attributeList;
};

struct ObjectList {
    Object* object;
    ObjectList* next;
};

typedef struct Program {
    ObjectList* objectList;
} Program;

#endif
