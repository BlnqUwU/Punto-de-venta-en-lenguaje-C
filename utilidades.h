#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <ctype.h>
#include <ncurses.h>

typedef enum mens { NO_MEMORY, OKE, INDEXOUTOFBOUND, EMPTY } mensaje;
typedef enum bo   { FALSO, TRU } booleano;

void downSem(int semID, unsigned short idx);
void upSem(int semID, unsigned short idx);
void ImprimirCentrado(int fila, const char *texto);
void hash(char *input, char *output);

#endif