/**
 Ackermann函数:
 ack(m, n) = n + 1                      ; m = 0 时
             ack(m - 1, 1)              ; m > 0, n = 0 时
             ack(m - 1, ack(m, n - 1))  ; m > 0, n > 0;

(1)请采用备忘录方法设计一个求解该函数的递归算法。
(2)请用动态规划方法设计一个非递归求解算法，该算法由两个嵌套循环组成，只能使用O(m)内的空间。
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int A[20000][20000] = {0};

int recusion(int m, int n) {
    if (A[m][n]) {
        return A[m][n];
    }
    if (m == 0) {
        A[0][n] = n + 1;
        return A[0][n];
    }
    else if (n == 0) {
        A[m][0] = recusion(m - 1, 1);
        return A[m][0];
    }
    A[m][n] = recusion(m - 1, recusion(m, n - 1));
     return A[m][n];
}

int recusiona(int **a, int m, int n) {
    if (a[m][n]) {
        return a[m][n];
    }
    if (m == 0) {
        return a[m][n] = n + 1;
    }
    else if (n == 0) {
        return a[m][n] = recusiona(a, m - 1, 1);
    }
    return a[m][n] = recusiona(a, m - 1, recusiona(a, m, n - 1));
}


int recusion1(int m, int n) {
    if (m == 0) {
        return n + 1;
    }
    else if (n == 0) {
        return recusion1(m - 1, 1);
    }
    return recusion1(m - 1, recusion1(m, n - 1));
}

int dyn(int& m,int& n)
{
    int i, j;
    int *vl = (int *)malloc(sizeof(int) * (m+1));
    int *ndx = (int *)malloc(sizeof(int) * (m+1));
    for(i = 1; i <= m; i++)
    {
        ndx[i] = -1;
        vl[i] = -1;
    }
    ndx[0] = 0;
    vl[0] = 1;
    while(ndx[m] < n)
    {
        vl[0]++;
        ndx[0]++;
        for(j=1;j<=m;j++)
        {
            if(1==ndx[j-1])
            {
                vl[j]=vl[j-1];
                ndx[j]=0;
            }
            if(vl[j]!=ndx[j-1])
                break;
            ndx[j]++;
            vl[j]=vl[j-1];
        }
    }
 
    return vl[m];
}


int main() {
    int m, n, res;    
    clock_t s_time, e_time;

    scanf("%d%d", &m, &n);    
    
    s_time = clock();
    res = recusion(m, n);
    e_time = clock();
    printf("RECUSION ACKMANN: %d TIME: %gs\n", res, double(e_time - s_time)/CLOCKS_PER_SEC);
    
    s_time = clock();
    res = dyn(m, n);
    e_time = clock();
    printf("DYNAMIC ACKMANN: %d TIME: %gs\n", res, double(e_time - s_time)/CLOCKS_PER_SEC);    
    
}