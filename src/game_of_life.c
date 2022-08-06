#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W 80
#define H 25
#define life "*"
#define dead " "

// void gameMenu();
void game();
int allocMemory(char ***matrix, int n, int m);
int freeMemory(char **matrix, int n, int m);
void generation(char **matrix, int n, int m);
void resetField(char **matrix, int n, int m);
void fieldUpdate(char ***matrix, int n, int m, char ***buff);
void fieldOutput(char **matrix, int n, int m);

int main() {
  srand(time(NULL));
  game();
  return 0;
}

void game() {
  char **matrix;
  char **buff;
  allocMemory(&matrix, H, W);
  allocMemory(&buff, H, W);
  generation(matrix, H, W);
  fieldUpdate(&matrix, H, W, &buff);
  printf("\n\n");
  while (1) {
    int x;
    fieldUpdate(&matrix, H, W, &buff);
    scanf("%d", &x);
    printf("\n-------------------------------------\n");
    // fpurge(stdout);
    // printf("\033[H\033[J");
    fieldOutput(matrix, H, W);
  }

  freeMemory(matrix, H, W);
  freeMemory(buff, H, W);
}

int allocMemory(char ***matrix, int n, int m) {
  (*matrix) = malloc(n * sizeof(char *));
  for (int i = 0; i < n; i++) (*matrix)[i] = malloc(m * sizeof(char));
}
int freeMemory(char **matrix, int n, int m) {
  for (int i = 0; i < n; i++) free(matrix[i]);
  free(matrix);
}
void generation(char **matrix, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int x = rand() % 2;
      matrix[i][j] = x + '0';
    }
  }
}
void fieldUpdate(char ***matrix, int n, int m, char ***buff) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if ((*matrix)[(n + i + 1) % n][(m + j) % m] == '1') {
        count++;
      }
      if ((*matrix)[(n + i - 1) % n][(m + j) % m] == '1') {
        count++;
      }
      if ((*matrix)[(n + i) % n][(m + j + 1) % m] == '1') {
        count++;
      }
      if ((*matrix)[(n + i) % n][(m + j - 1) % m] == '1') {
        count++;
      }
      if ((*matrix)[(n + i + 1) % n][(m + j + 1) % m] == '1') {
        count++;
      }
      if ((*matrix)[(n + i - 1) % n][(m + j - 1) % m] == '1') {
        count++;
      }
      if ((*matrix)[(n + i + 1) % n][(m + j - 1) % m] == '1') {
        count++;
      }
      if ((*matrix)[(n + i - 1) % n][(m + j + 1) % m] == '1') {
        count++;
      }
      if (count == 2 || count == 3)
        (*buff)[i][j] = '1';
      else
        (*buff)[i][j] = '0';
    }
    count = 0;
  }
  char **temp = *matrix;
  *matrix = *buff;
  *buff = temp;
}

void resetField(char **matrix, int n, int m) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) matrix[i][j] = '0';
}

void fieldOutput(char **matrix, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == '1') {
        printf(life);
        if (j != m - 1) printf(" ");
      } else {
        printf(dead);
        if (j != m - 1) printf(" ");
      }
    }
    if (i != n - 1) printf("\n");
  }
}