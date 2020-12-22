#include <stdio.h>
#include "./include/Print_Intermediate_code.h"
#include "./includeIntermediate_code.h"

void print_Intermediate_code()
{
  for(int i = 0; i < nextquad; i++)
  {
      printf("%i\t", i+1);
      Quadruplet* quad = tabQuad[i];

      switch (quad->op)
      {
          case ASSIGNMENT :
              printf("%s := %s\n",  (char *) quad->res,
                                  (char *) quad->arg1);
              break;
          case ADD :
              printf("%s := %s + %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case MINUS :
              printf("%s := %s - %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case MULTIPLIES :
              printf("%s := %s * %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case DIVIDES :
              printf("%s := %s / %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case POWER :
              printf("%s := %s ^ %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case LOWER :
              printf("%s := %s < %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case LOWER_OR_EQUAL :
              printf("%s := %s <= %s\n",  (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case SUPERIOR :
              printf("%s := %s > %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case SUPERIOR_OR_EQUAL :
              printf("%s := %s >= %s\n",  (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case EQUALS :
              printf("%s := %s = %s\n",   (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case DIFFERENT :
              printf("%s := %s <> %s\n",  (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case AND :

              printf("%s := %s AND %s\n", (char *) quad->res,
                                          (char *) quad->arg1,
                                          (char *) quad->arg2);
              break;
          case OR :

              printf("%s := %s OR %s\n", (char *) quad->res,
                                      (char *) quad->arg1,
                                      (char *) quad->arg2);
              break;
          case XOR :

              printf("%s := %s XOR %s\n", (char *) quad->res,
                                      (char *) quad->arg1,
                                      (char *) quad->arg2);
              break;
          case NOT :
              printf("%s := NOT %s\n",  (char *) quad->res,
                                      (char *) quad->arg1);
              break;
          case NEGATE :
              printf("%s := %s\n",  (char *) quad->res,
                                  (char *) quad->arg1);
              break;

          case IF :
              if(((Symbol *) quad->res) == NULL)
                  printf("IF %s GOTO ?\n", (char *) quad->arg1);
              else
                  printf("IF %s GOTO %i\n", (char *) quad->arg1,
                                          *((int*) quad->res));
              break;
          case GOTO :
              if(quad->arg1 == NULL)
                  printf("GOTO ?");
              else
                  printf("GOTO %i\n", *((int*) quad->res) );
              break;

          case READ :
              printf("READ %s\n", (char *) quad->arg1);
              break;
          case WRITE :
              printf("WRITE %s\n", (char *) quad->arg1);
              break;
      }
  }
}
