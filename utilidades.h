#ifndef UTILS_H
#define UTILS_H

#include <sys/sem.h>

void downSem(int semID, unsigned short idx);
void upSem(int semID, unsigned short idx);

#endif