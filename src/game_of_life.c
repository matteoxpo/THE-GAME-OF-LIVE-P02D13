#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 80
#define M 25
#define LIFE "*"
#define DEAD " "

// void gameMenu();
void game();
int allocMemory(char ***matrix);
void freeMemory(char **matrix);
void generation(char **matrix);
void resetField(char **matrix);
void fieldUpdate(char ***matrix, char ***buff);
int countAliveNeigh(char **matrix, int i, int j);
void fieldOutput(char **matrix);
int countAliveCells(char **matrix, int i, int j);

int main() {
  srand(time(NULL));
  game();
  return 0;
}

void game() {
  char **matrix;
  char **buff;
  allocMemory(&matrix);
  allocMemory(&buff);
  generation(matrix);
  fieldUpdate(&matrix, &buff);
  printf("\n\n");
  while (1) {
    int x;
    fieldUpdate(&matrix, &buff);
    scanf("%d", &x);
    printf("\n-------------------------------------\n");
    fieldOutput(matrix);
  }

  freeMemory(matrix);
  freeMemory(buff);
}

int allocMemory(char ***matrix) {
  int check = 1;
  (*matrix) = malloc(N * sizeof(char *));
  if (*matrix != NULL) {
    for (int i = 0; i < N; i++) {
      (*matrix)[i] = malloc(M * sizeof(char));
      if ((*matrix)[i] == NULL) {
        check = 0;
        break;
      }
    }
  } else {
    check = 0;
  }
  return check;
}
void freeMemory(char **matrix) {
  for (int i = 0; i < N; i++) free(matrix[i]);
  free(matrix);
}
void generation(char **matrix) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      int x = rand() % 2;
      matrix[i][j] = x + '0';
    }
  }
}
void fieldUpdate(char ***matrix, char ***buff) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      int count = countAliveCells(*matrix, i, j);
      if (count == 2 || count == 3)
        (*buff)[i][j] = '1';
      else
        (*buff)[i][j] = '0';
    }
  }
  char **temp = *matrix;
  *matrix = *buff;
  *buff = temp;
}

int countAliveCells(char **matrix, int i, int j) {
  int count = 0;
  for (int istep = -1; istep <= 1; istep++) {
    for (int jstep = -1; jstep <= 1; jstep++) {
      if (istep == jstep == 0) continue;
      if (matrix[(N + i + istep) % N][(M + j + jstep) % M] == '1') count++;
    }
  }
  return count;
}

int countAliveNeigh(char **matrix, int i, int j) {
  int count = 0;
  if (matrix[(N + i + 1) % N][(M + j) % M] == '1') {
    count++;
  }
  if (matrix[(N + i - 1) % N][(M + j) % M] == '1') {
    count++;
  }
  if (matrix[(N + i) % N][(M + j + 1) % M] == '1') {
    count++;
  }
  if (matrix[(N + i) % N][(M + j - 1) % M] == '1') {
    count++;
  }
  if (matrix[(N + i + 1) % N][(M + j + 1) % M] == '1') {
    count++;
  }
  if (matrix[(N + i - 1) % N][(M + j - 1) % M] == '1') {
    count++;
  }
  if (matrix[(N + i + 1) % N][(M + j - 1) % M] == '1') {
    count++;
  }
  if (matrix[(N + i - 1) % N][(M + j + 1) % M] == '1') {
    count++;
  }
  return count;
}

void resetField(char **matrix) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++) matrix[i][j] = '0';
}

void fieldOutput(char **matrix) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (matrix[i][j] == '1') {
        printf(LIFE);
        if (j != M - 1) printf(" ");
      } else {
        printf(DEAD);
        if (j != M - 1) printf(" ");
      }
    }
    if (i != N - 1) printf("\n");
  }
}