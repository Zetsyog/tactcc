#include "./include/generation.h"
#include "./include/grammar.h"
#include "./include/Intermediate_code.h"
#include <stdarg.h>

int gencode(OP op , ...)
{
    va_list args;
    va_start(args,op);
    int index = nextquad;
    size_t size = 0;

    if(op == GOTO)
    {
        tabQuad[index] = (Quadruplet*) malloc(sizeof(Quadruplet));
        tabQuad[index]->arg1 = NULL;
        tabQuad[index]->arg2 = NULL;
        tabQuad[index]->res = va_arg(args, int*);
    }else if(op == READ || WRITE){
            tabQuad[index] = (Quadruplet*) malloc(sizeof(Quadruplet));
            tabQuad[index]->arg1 = va_arg(args, char*);
            tabQuad[index]->arg2 = NULL;
            tabQuad[index]->res  = NULL;
    }else if(op == ASSIGNMENT){

                tabQuad[index] = (Quadruplet*) malloc(sizeof(Quadruplet));
                tabQuad[index]->arg1 = va_arg(args, char*);
                tabQuad[index]->arg2 = NULL;
                tabQuad[index]->res  = va_arg(args,int*);
    } else {
                    tabQuad[index] = (Quadruplet*) malloc(sizeof(Quadruplet));
                    tabQuad[index]->arg1 = va_arg(args, void *);
                    tabQuadruplet[index]->arg2 = NULL;
                    if (op >= 15)
                        size = 2;
                    else
                        size = 3;

                    if (size >=3)
                        tabQuad[index]->arg2 = va_arg(args, void *);

                    tabQuad[index]->res  = NULL;

                    if (size >=2)
                        tabQuad[index]->res = va_arg(args, void *);
    }

    tabQuad[index]->op = op;
    va_end(args);
    nextquad++;
    return index;

}
