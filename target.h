
#ifndef _CODE_H_
#define _CODE_H_

#include "globals.h"
#include "cgen.h"

#define size_registerS 10

#define registerZero 0 //Registrador com valor constante zero
#define registerTBR 1 //Início dos registradores do tipo T
#define registerSBR 11 //Início dos registradores do tipo S
// #define registerCONST1 21 //registrador para valores constantes
// #define registerCONST2 22 //registrador para valores constantes
// #define registerMDR 26 //Registrador para multiplicação/divisão (Comparação)
// #define registerMD1 27 //Registrador para multiplicação/divisão
// #define registerMD2 28 //Registrador para multiplicação/divisão
#define registerINOUT 29 //registrador para impressão
#define registerRV 30 //Registrador para guardar valor de retorno
#define registerAR 31 //Registrador para end. retorno

#define MAXPARAMS 1000

typedef struct FuncMem
{
    const char * name_func;
    int memloc;
    struct arguments * args;
    struct FuncMem * next;
} FuncMem;

typedef struct arguments
{
    char *name;
    int memloc;
    struct arguments *next;
} Arguments;

typedef struct labels
{
    int label;
    struct OperatingT * op;
    struct labels * next;
} Labels;

int reg_params[MAXPARAMS];

AssemblyCode codeGenerationAssembly(AddressQuad code, FILE * codefile, int is_os);

Operating create_element_Operating(OperatingType type, int op);

void print_assembly();

#endif
