#ifndef UTILS_BACK_H
#define UTILS_BACK_H

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

// CAMBIO: enums definidos aqui solo para el servidor
// el front usa utilidades.h que ya los define
typedef enum mens { NO_MEMORY, OKE, INDEXOUTOFBOUND, EMPTY } mensaje;
typedef enum bo   { FALSO, TRU } booleano;

void downSem(int semID, unsigned short idx);
void upSem(int semID, unsigned short idx);

#endif