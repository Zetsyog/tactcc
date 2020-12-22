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


int* crelist(int addr)
{
    int* list = malloc(sizeof(int));
    for(int i = 0; i < sizeof(list); i++)
    {
        list[i]=addr;
    }
    return list;
}

int* concat(int * list1, int * list2)
{
    list1, list2 = malloc(sizeof(int));
    int* list_concat=malloc(sizeof(list1)+sizeof(list2));
    for(int i = 0; i < sizeof(list1);i++)
        list_concat[i] = list1 [i];
    for(int j=0; j < sizeof(list2) ; j++)
        list_concat[j+sizeof(list1)]=list2[j];

    return list_concat;
}
