typedef enum {

    ASSIGNMENT        = 0,
    ADD               = 1,
    MINUS             = 2,
    MULTIPLIES        = 3,
    DIVIDES           = 4,
    POWER             = 5,
    LOWER             = 6,
    LOWER_OR_EQUAL    = 7,
    SUPERIOR          = 8,
    SUPERIOR_OR_EQUAL = 9,
    EQUALS            = 10,
    DIFFERENT         = 11,
} Operation;

typedef struct{
    OP      op;
    void*   arg1;
    void*   arg2;
    void*   res;
} Quadruplet;


int nextquad;
Quadruplet* tabQuad[10000];
