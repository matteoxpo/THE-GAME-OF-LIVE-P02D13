#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 5
#define M 5
#define LIFE "*"
#define DEAD "-"

// void gameMenu();
void game();
int allocMemory(char ***matrix);
void freeMemory(char **matrix);
void generation(char **matrix);
void resetField(char **matrix);
int fieldUpdate(char ***matrix, char ***buff);
int countAliveNeigh(char **matrix, int i, int j);
void fieldOutput(char **matrix);
int countAliveCells(char **matrix, int i, int j);
int sameMatrix(char **matrix, char **buff);

int main() {
  game();
  return 0;
}

void game() {
  char **matrix;
  char **buff;
  allocMemory(&matrix);
  allocMemory(&buff);
  generation(matrix);
  printf("\n");
  while (fieldUpdate(&matrix, &buff)) {
    // int x;
    // scanf("%d", &x);
    printf("\n-------------------------------------\n");
    sleep(5);
    fieldOutput(matrix);
  }

  /*
    generation(matrix);
    fieldOutput(matrix);
    printf("\n---------------------------\n");
    fieldUpdate(&matrix, &buff);
    fieldOutput(matrix);
    printf("\n---------------------------\n");
    fieldUpdate(&matrix, &buff);
    fieldOutput(matrix);
    printf("\n---------------------------\n");
    */
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
      int x = i % 2;
      matrix[i][j] = x + '0';
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}

int fieldUpdate(char ***matrix, char ***buff) {
  int livCells = 0;
  int changeFlag = 0;
  int count;
  int check = 1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      count = countAliveCells(*matrix, i, j);
      livCells += count;
      if ((*matrix)[i][j] == '1') {
        if (count != 2 && count != 3) {
          (*buff)[i][j] = '0';
          changeFlag = 1;
        } else {
          (*buff)[i][j] = '1';
        }
      } else {
        if (count == 3) {
          (*buff)[i][j] = '1';
          changeFlag = 1;
        } else {
          (*buff)[i][j] = '0';
        }
      }
    }
  }
  char **temp = *matrix;
  *matrix = *buff;
  *buff = temp;
  if (livCells == 0 || changeFlag == 0) check = 0;

  return check;
}

int countAliveCells(char **matrix, int i, int j) {
  int count = 0;
  for (int istep = -1; istep <= 1; istep++) {
    for (int jstep = -1; jstep <= 1; jstep++) {
      if (istep == 0 && jstep == 0) {
      } else {
        if (matrix[(N + i + istep % N) % N][(M + j + jstep % M) % M] == '1')
          count++;
      }
    }
  }
  return count;
}

void fieldOutput(char **matrix) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (matrix[i][j] == '1') {
        printf(LIFE);
      } else {
        printf(DEAD);
      }
    }
    if (i != N - 1) printf("\n");
  }
}

int sameMatrix(char **matrix, char **buff) {
  int check = 1;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++)
      if (matrix[i][j] != buff[i][j]) check = 0;
  return check;
}