
#include "utilidades.h"

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

void ImprimirCentrado(int fila, const char *texto) {
    int col = (COLS - strlen(texto)) / 2;
    if (col < 0) col = 0;
    mvprintw(fila, col, "%s", texto);
}

void hash(char *input, char *output) {
    unsigned long h = 5381;
    int c;
    while ((c = *input++))
        h = ((h << 5) + h) + c;
    sprintf(output, "%lu", h);
}