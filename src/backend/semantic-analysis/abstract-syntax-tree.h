#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

typedef struct AttributeSequence AttributeSequence;
typedef struct StatementSequence StatementSequence;

typedef enum AttributeType { INT, FLOAT, DOUBLE, TEXT, CHAR, BOOL, DATE, TIME } AttributeType;

typedef struct Attribute {
    char name[64];
    AttributeType type;
} Attribute;

struct AttributeSequence {
    Attribute attribute;
    AttributeSequence* next;
};

typedef struct Entity {
    char name[64];
    AttributeSequence* attributes;
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
