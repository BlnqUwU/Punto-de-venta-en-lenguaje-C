#ifndef UTILS_H
#define UTILS_H

#include <sys/sem.h>

typedef  enum mens { NO_MEMORY , OKE , INDEXOUTOFBOUND, EMPTY } mensaje;
typedef  enum bo { FALSO , TRU } booleano;

void downSem(int semID, unsigned short idx);
void upSem(int semID, unsigned short idx);

#endif