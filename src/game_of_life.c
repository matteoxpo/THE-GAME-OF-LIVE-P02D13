#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define LENGTH 80
#define LIFE "*"
#define DEAD " "
#define MAXSPEED 1.9
#define MINSPEED 0.1
#define ITERSPEED 0.1
#define STARTSPEED 100000

void gameMenu();
void printMenuOptions();
void printAboutGame();
void saveScan(int *command);
int corrrectCommand(int command);
void inputRules();
void game(int mode);
int allocMemory(char ***matrix);
void freeMemory(char **matrix);
void changeStream(int mode);
void generation(char **matrix);
void resetField(char **matrix);
int fieldUpdate(char ***matrix, char ***buff);
int countAliveNeigh(char **matrix, int i, int j);
void fieldOutput(char **matrix, WINDOW *win);
int countAliveCells(char **matrix, int i, int j);
int sameMatrix(char **matrix, char **buff);
void changeSpeed(char button, float *speed);

int main() {
  gameMenu();
  return 0;
}

void gameMenu() {
  int command = -1;
  printAboutGame();
  printMenuOptions();
  while (1) {
    saveScan(&command);
    if (corrrectCommand(command)) {
      game(command);
      break;
    } else {
      printf("Неизвестная команда.\n");
    }
  }
}
void printMenuOptions() {
  printf("1 - пользовательский ввод игры\n");
  printf("2 - запустить карту cow\n");
  printf("3 - запустить карту gunGospy\n");
  printf("4 - запустить карту gunSim\n");
  printf("5 - запустить карту agar\n");
  printf("6 - запустить карту shipNew\n");
}

void printAboutGame() {
  printf("Игра в жизнь\n");
  printf(
      "Место действия игры — размеченная на клетки плоскость, которая может "
      "быть безграничной, ограниченной, или замкнутой.\n");
  printf("В нашем случае поле %d на %d размером и сфеерически замкнута.\n", HEIGHT,
         LENGTH);
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
    printf("Ввод команды: ");
    check = scanf("%d", command);
    if (!check) {
      printf("Ошибка ввода, попробуем снова.\n");
    }
  }
}

int corrrectCommand(int command) {
  return (command >= 1 && command <= 6) ? 1 : 0;
}

void game(int mode) {
  char **matrix;
  char **buff;
  float speed = 1.0f;
  char button = '\0';
  allocMemory(&matrix);
  allocMemory(&buff);
  changeStream(mode);
  generation(matrix);
  stdin = freopen("/dev/tty", "r", stdin);
  initscr();
  noecho();
  WINDOW *win = newwin(HEIGHT, LENGTH, 0, 0);
  wrefresh(win);
  fieldOutput(matrix, win);
  wrefresh(win);
  while (fieldUpdate(&matrix, &buff) && !(button == 'q' || button == 'Q')) {
    fieldOutput(matrix, win);
    mvwprintw(win, HEIGHT - 1, 5, "Speed: x%.1f", 2.0 - speed);
    halfdelay(1);
    button = wgetch(win);
    changeSpeed(button, &speed);
    usleep(STARTSPEED * speed);
    wrefresh(win);
  }
  freeMemory(matrix);
  freeMemory(buff);
  endwin();
}

void changeStream(int mode) {
  switch (mode) {
    case 1:
      inputRules();
      break;
    case 2:
      stdin = freopen("./presets/cow.txt", "r", stdin);
      break;
    case 3:
      stdin = freopen("./presets/gunGospy.txt", "r", stdin);
      break;
    case 4:
      stdin = freopen("./presets/gunSim.txt", "r", stdin);
      break;
    case 5:
      stdin = freopen("./presets/agar.txt", "r", stdin);
      break;
    case 6:
      stdin = freopen("./presets/shipNew.txt", "r", stdin);
      break;
  }
}
void inputRules() {
  printf("Корректный ввод мертвой клетки: 0 или - \n");
  printf("Корректный ввод живой клетки: 1 или  o \n");
}
int allocMemory(char ***matrix) {
  int check = 1;
  (*matrix) = malloc(HEIGHT * sizeof(char *));
  if (*matrix != NULL) {
    for (int i = 0; i < HEIGHT; i++) {
      (*matrix)[i] = malloc(LENGTH * sizeof(char));
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
  for (int i = 0; i < HEIGHT; i++) free(matrix[i]);
  free(matrix);
}

void generation(char **matrix) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < LENGTH; j++) {
      char c;
      scanf("%c ", &c);
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
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < LENGTH; j++) {
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
        if (matrix[(HEIGHT + i + istep) % HEIGHT][(LENGTH + j + jstep) % LENGTH] == '1')
          count++;
      }
    }
  }
  return count;
}

void fieldOutput(char **matrix, WINDOW *win) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < LENGTH; j++) {
      if (matrix[i][j] == '1') {
        mvwprintw(win, i, j, LIFE);
        printw(LIFE);
      } else {
        mvwprintw(win, i, j, DEAD);
      }
    }
  }
}

void changeSpeed(char button, float *speed) {
  if (*speed > MINSPEED && (button == 'a' || button == 'A')) {
    *speed -= ITERSPEED;
  }
  if (*speed < MAXSPEED && (button == 'z' || button == 'Z')) {
    *speed += ITERSPEED;
  }
}
