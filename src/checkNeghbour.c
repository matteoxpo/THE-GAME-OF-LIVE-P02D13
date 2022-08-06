#include <stdio.h>

#define N 80
#define M 25

int checkNeighbour(int **mas, int iCheck, int jCheck);

/*int main() {
    int mas[5][5] = {{1, 0, 1, 0, 0}, {0, 1, 1, 0, 1}, {1, 0, 0, 1, 1}, {1, 1, 0, 0, 0}, {0, 1, 1, 1, 1}};
    int count;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            count = check(mas, i, j);
            printf("i=%d j=%d count=%d\n", i, j, count);
        }
    }
    return 0;
}*/

int checkNeighbour(int **mas, int iCheck, int jCheck) {
    int check = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j ==0) {
                continue;
            } else if (mas[(iCheck + i % N + N) % N][(jCheck + j % M + M) % M] == 1) {
                check++;
            }
        }
    }
    return check;
}
