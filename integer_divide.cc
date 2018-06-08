/**
 正整数的划分问题是将一个正整数n表示成一系列正整数之和：
 n=n1+n2+…+nk，其中n1≥n2≥…≥nk≥1，k≥1。
 请编写至少三种不同的求解算法，
 并对所编写算法的时间效率进行测试和比较。
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

long long recusion(int p, int q) {
    if (p <= 1 || q <= 1) {
        return 1;
    }
    else if (p < q) {
        return recusion(p, p);
    }
    else {
        return recusion(p - q, q) + recusion(p, q - 1);
    }
}

long long dyn(int num){
    int size;
    long long **a;
    size = num + 1;
    a = (long long **) malloc(sizeof(long long*) * size);
    
    for (int i = 0; i < size; ++ i) {
        a[i] = (long long *) malloc(sizeof(long long) * size);
        memset(a[i], 0, sizeof(long long) * size);
    }
    a[0][0] = 1;
    for (int i = 0; i < size; i ++){
        for (int j = 1; j < size; ++j) {
            if (j > i) a[i][j] = a[i][i];
            else a[i][j] = a[i - j][j] + a[i][j - 1];
        }
    }
    return a[num][num];

}


long long gen(int num) {
    long long *a = (long long*) malloc(sizeof(long long) * (num + 1));
    long long *b = (long long*) malloc(sizeof(long long) * (num + 1));
    for(int i = 0; i <= num; ++ i) {  
        a[i]=1;  
        b[i]=0;  
    }  
    for(int i = 2; i <= num; ++ i)//共有n-1个多项式   
    {  
        for(int j=0; j <= num; ++ j)//每次枚举前边多项式中的每一项   
        {  
            for(int k=0 ; k+j <= num; k += i)//不同的i表示系数的等差   
            {  
                b[k+j]+=a[j];  
            }  
        }  
        for(int j = 0; j<= num; j++)//将新得到的数据重新存入到a中   
        {  
            a[j]=b[j];  
            b[j]=0;  
        }  
    }  
    return a[num];
}



int main() {
    int i;
    long long res;
    while (scanf("%d", &i) != EOF && i > 0) {
        clock_t s_time, e_time;
        s_time = clock();
        res = recusion(i, i);
        e_time = clock();
        printf("RECUSION RESULT: %lld TIME: %gs\n", res, (e_time - s_time)/1.0/CLOCKS_PER_SEC);

        s_time = clock();
        res = dyn(i);
        e_time = clock();
        printf("DYNAMIC RESULT: %lld TIME: %gs\n", res, (e_time - s_time)/1.0/CLOCKS_PER_SEC);

        s_time = clock();
        res = gen(i);
        e_time = clock();
        printf("GEN RESULT: %lld TIME: %gs\n", res, (e_time - s_time)/1.0/CLOCKS_PER_SEC);
    }
    
}