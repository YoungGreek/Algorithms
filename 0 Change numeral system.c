#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct t{
    int h;
    int m;
    int s;
}t;

void print(int n){
    for(int i = 2; i <= n; i++){
        if((pow(2, i) - 1) > n){
            break;
        }
        int count = 0;
        for(int j = 2; j <= (int)sqrt(i); j++){
            if(i % j == 0){
                count += 1;
            }
            if(count > 0){
                break;
            }
        }
        if(count == 0){
            printf("%d\n", (int)pow(2, i) - 1);
        }
    }
}

int move(t *time, int shift){
    int all_time = time->h * 60 * 60 + time->m * 60 + time->s;
    int last_time = 23 * 60 * 60 + 59 * 60 + 59;
    if(all_time + shift > last_time){
        all_time = shift - 1;
    }
    else{
        all_time = all_time + shift;
    }
    return all_time;
}

int main() {
    int n;
    scanf("%d", &n);
    print(n);

    t *time = malloc(sizeof(t));
    time->h = 12;
    time->m = 43;
    time->s = 30;
    int shift;
    scanf("%d", &shift);
    int all_time = move(time, shift);
    time->s = all_time % 60;
    time->m = all_time / 60 % 60;
    time->h = all_time / 3600;
    printf("%d %d %d ", time->h, time->m, time->s);
    return 0;
}
