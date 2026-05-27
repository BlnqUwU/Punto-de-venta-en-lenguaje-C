#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <ncurses.h>
#include <string.h>

#include <unistd.h>

#include <ctype.h>

#include <time.h>

void downSem(int semID, unsigned short idx);
void upSem(int semID, unsigned short idx);

typedef  enum mens { NO_MEMORY , OKE , INDEXOUTOFBOUND, EMPTY } mensaje;
typedef  enum bo { FALSO , TRU } booleano;

void ImprimirCentrado(int fila, const char *texto);
void hash(char *input, char *output);
#endif