#include "utilidades_back.h"

void downSem(int semID, unsigned short idx) {
    struct sembuf op = {idx, -1, 0};
    if (semop(semID, &op, 1) == -1) {
        perror("semop down");
        exit(1);
    }
}

void upSem(int semID, unsigned short idx) {
    struct sembuf op = {idx, +1, 0};
    if (semop(semID, &op, 1) == -1) {
        perror("semop up");
        exit(1);
    }
}