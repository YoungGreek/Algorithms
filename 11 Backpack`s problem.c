#include <stdlib.h>
#include <stdio.h>


typedef struct Weight_Cost{
   int weight;
   int cost;
}WC;

int CCI(int *N, int *W){
    if(scanf("%d %d", N, W) != 2){
        return 0;
    }
    if(*N == 0 || *W == 0){
        return 0;
    }
    return 1;
}

int max(int a, int b){
    if(a > b){
        return a;
    }
    else{
        return b;
    }
}

int main(void) {
    int N;
    int W;

    if(CCI(&N, &W) == 0){
        return 0;
    }
    int table[N + 1][W + 1];
    WC *list = (WC*)(malloc(N * sizeof(WC)));

    int value_1;
    int value_2;
    for(int i = 0; i < N; i++){
        if(scanf("%d %d", &value_1, &value_2) != 2){
            free(list);
            return 0;
        }
        list[i].weight = value_1;
        list[i].cost = value_2;
    }

    for(int i = 0; i <= N; i++){
        for(int j = 0; j <= W; j++){
            if(i == 0 || j == 0){
                table[i][j] = 0;
            }
            else if(list[i - 1].weight <= j){
                table[i][j] = max(list[i - 1].cost + table[i - 1][j - list[i - 1].weight], table[i - 1][j]);
            }
            else{
                table[i][j] = table[i - 1][j];
            }
        }
    }

    printf("%d\n", table[N][W]);

    WC *ans = (WC*)(malloc(N * sizeof(WC)));
    int counter = 0;

    while(table[N][W] != 0){
        if(table[N - 1][W] == table[N][W]){
            N -= 1;
        }
        else{
            W -= list[N - 1].weight;
            ans[counter].weight = list[N - 1].weight;
            ans[counter].cost = list[N - 1].cost;
            N -= 1;
            counter += 1;
        }
    }
    for(int i = counter - 1; i >= 0; i--){
        printf("%d %d\n", ans[i].weight, ans[i].cost);
    }
    free(list);
    free(ans);
    return 0;
}
