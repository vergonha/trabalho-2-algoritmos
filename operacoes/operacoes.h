
#include "../include/TDE.h"
#include "inserir.c"
#include "remover.c"
#include <stdlib.h>
#include <stdio.h>
#ifndef OPERACOESH
#define OPERACOESH

int inserir_usuario(Usuario *u);
int inserir_produto(Produto *p);

int remover_usuario(char *target);
int remover_produto(char *target);

#endif