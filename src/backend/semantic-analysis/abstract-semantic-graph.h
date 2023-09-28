#ifndef ABSTRACT_SEMANTIC_GRAPH_HEADER
#define ABSTRACT_SEMANTIC_GRAPH_HEADER

#define NAMEDATALEN 31

typedef enum ObjectType { ENTITY, RELATION } ObjectType;

typedef enum AttributeType { INT, FLOAT, DOUBLE, TEXT, CHAR, BOOL, DATE, TIME } AttributeType;
typedef enum AttributeModifier { NOTNULL, NULLABLE, KEY, MULTI } AttributeModifier;

typedef struct Link Link;
typedef enum LinkType { SYMBOL, REFERENCE } LinkType;
typedef enum LinkModifier { ONE, MANY } LinkModifier;

typedef struct Object Object;
typedef struct ObjectList ObjectList;

typedef struct AttributeList AttributeList;

typedef struct EntityList EntityList;
typedef struct RelationList RelationList;

struct Link {
    char name[NAMEDATALEN];
    LinkType type;
    LinkModifier modifier;
    union SymbolVariant {
        char symbol[NAMEDATALEN];
        const Object* reference;
    } variant;
};

typedef struct Attribute {
    char name[NAMEDATALEN];
    AttributeType type;
    AttributeModifier modifier;
} Attribute;

struct AttributeList {
    Attribute* attribute;
    AttributeList* next;
};

struct Object {
    ObjectType type;
    char name[NAMEDATALEN];
    AttributeList* attributeList;
    Link** linkedObjects;
};

struct ObjectList {
    Object* object;
    ObjectList* next;
};

typedef struct Program {
    ObjectList* objectList;
} Program;

#endif
