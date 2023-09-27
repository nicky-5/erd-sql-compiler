#ifndef ABSTRACT_SEMANTIC_GRAPH_HEADER
#define ABSTRACT_SEMANTIC_GRAPH_HEADER

#define NAMEDATALEN 31

typedef enum StatementType { ENTITY, RELATION } StatementType;

typedef enum AttributeType { INT, FLOAT, DOUBLE, TEXT, CHAR, BOOL, DATE, TIME } AttributeType;
typedef enum AttributeModifier { NOTNULL, NULLABLE, PK } AttributeModifier;

typedef struct StatementList StatementList;

typedef struct AttributeList AttributeList;
typedef struct RelationEntityList RelationEntityList;

typedef struct EntityRefList EntityRefList;
typedef struct RelationRefList RelationRefList;

typedef struct NameList NameList;

struct NameList {
    char name[NAMEDATALEN];
    NameList* next;
};

typedef struct Relation {
    char name[NAMEDATALEN];
    RelationEntityList* relationEntityList;
} Relation;

typedef struct RelationRefList {
    const Relation* relationRef;
    RelationRefList* next;
} RelationRefList;

typedef struct Entity {
    char name[NAMEDATALEN];
    AttributeList* attributes;
    NameList* keys;
} Entity;

typedef struct EntityRefList {
    const Entity* entityRef;
    EntityRefList* next;
} EntityRefList;

typedef struct RelationEntity {
    char name[NAMEDATALEN];
    const Entity* entityRef;
} RelationEntity;

struct RelationEntityList {
    RelationEntity* relationEntity;
    RelationEntityList* next;
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

typedef struct Statement {
    StatementType type;
    union StatementVariant {
        Entity* entity;
        Relation* relation;
    } variant;
} Statement;

struct StatementList {
    Statement* statement;
    StatementList* next;
};

typedef struct Program {
    StatementList* statementList;
} Program;

#endif
