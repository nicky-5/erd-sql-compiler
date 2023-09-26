#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#define NAMEDATALEN 64

typedef struct AttributeSequence AttributeSequence;
typedef struct StatementSequence StatementSequence;

typedef enum AttributeType { INT, FLOAT, DOUBLE, TEXT, CHAR, BOOL, DATE, TIME } AttributeType;
typedef enum AttributeModifier { NOTNULL, NULLABLE, PK } AttributeModifier;

typedef struct Attribute {
    char name[NAMEDATALEN];
    AttributeType type;
    AttributeModifier modifier;
} Attribute;

struct AttributeSequence {
    Attribute attribute;
    AttributeSequence* next;
};

typedef struct PrimaryKeyNode PrimaryKeyNode;

struct PrimaryKeyNode {
    char name[NAMEDATALEN];
    PrimaryKeyNode* next;
};

typedef struct Entity {
    char name[NAMEDATALEN];
    AttributeSequence* attributes;
    PrimaryKeyNode* primaryKeys;
} Entity;

typedef enum StatementType { ENTITY } StatementType;
typedef struct Statement {
    StatementType type;
    union StatementVariant {
        Entity entity;
    } variant;
} Statement;

struct StatementSequence {
    Statement statement;
    StatementSequence* next;
};

typedef struct Program {
    StatementSequence* statementSequence;
} Program;

#endif
