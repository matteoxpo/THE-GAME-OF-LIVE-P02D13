#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 25
#define M 80
#define LIFE "*"
#define DEAD " "

void gameMenu();
void printMenuOptions();
void printAboutGame();
void saveScan(int *command);
void game(int mode);
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
  game(1);
  return 0;
}

void gameMenu() {
  int check = 0;
  int command;
  while (1) {
    printMenuOptions();
    saveScan(&command);
    switch (command) {
      case 1:
        break;
      case 2:
        break;
    }
  }
}
void printMenuOptions() {
  printf("1 - пользовательский ввод игры\n");
  printf("2 - запустить карту gunGospy\n");
  printf("3 - запустить карту gunSim\n");
  printf("4 - запустить карту agar\n");
}

void printAboutGame() {
  printf("Игра в жизнь\n");
  printf(
      "Место действия игры — размеченная на клетки плоскость, которая может "
      "быть безграничной, ограниченной, или замкнутой.\n");
  printf("В нашем случае поле %d на %d размером и сфеерически замкнута.\n", N,
         M);
  printf(
      "Каждая клетка на этой поверхности имеет восемь соседей, окружающих "
      "её, "
      "и может находиться в двух состояниях: быть «живой» (заполненной) или "
      "«мёртвой» (пустой).\n");
  printf("Игра прекращается, если\n");
  printf("* на поле не останется ни одной «живой» клетки\n");
  printf("* при очередном шаге ни одна из клеток не меняет своего состояния\n");
}
void saveScan(int *command) {
  int check = 0;
  while (!check) {
    printf("Ввод команды:");
    check = scanf("%d", command);
    if (!check) {
      printf("Ошибка ввода, попробуем снова.\n");
    }
  }
}

void game(int mode) {
  char **matrix;
  char **buff;
  allocMemory(&matrix);
  allocMemory(&buff);
  generation(matrix);
  fieldOutput(matrix);
  printf("\n");
  while (fieldUpdate(&matrix, &buff)) {
    // break;
    // printf("\n000000000000\n");
    fieldOutput(matrix);
    usleep(1000000);
    printf("\n");
    printf("\033[0d\033[2J");
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
      char c;
      scanf(" %c", &c);
      if (c == '-') c = '0';
      if (c == 'o') c = '1';
      matrix[i][j] = c;
    }
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
