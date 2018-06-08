#include <cstdio>
#include <cmath>
#include <climits>
#include <stack>
#include <algorithm>
using namespace std;
struct Point
{
    int  x;
    int  y;
} P[50005], Q[50005];

int  xx;
int  yy;

bool cmp(struct Point a, struct Point b)
{
    if(atan2(a.y-yy, a.x-xx) != atan2(b.y-yy, b.x-xx))
        return (atan2(a.y-yy, a.x-xx)) < (atan2(b.y-yy, b.x-xx));
    return a.x < b.x;
}

int crss_prdct(int x1, int y1, int x2, int y2)
{
    return (x1*y2 - x2*y1);
}

int corner(struct Point a, struct Point b, struct Point c)
{
    return crss_prdct((b.x-a.x), (b.y-a.y), (c.x-a.x), (c.y-a.y));
}


int main()
{
    int n, i, j;
    printf("PLEASE INPUT THE SIZE OF THE SET:\n");
    while(~scanf("%d", &n))
    {
        int top = 1;
        yy = INT_MAX;
        printf("PLEASE INPUT THE DOTS IN THE SET:\n");
        for(i=0;i<n;i++)
        {
            scanf("%d%d",&P[i].x,&P[i].y);
            if(P[i].y < yy)
            {
                yy = P[i].y;
                xx = P[i].x;
                j = i;
            }
        }
        P[j] = P[0];
        sort(P+1, P+n, cmp);
        Q[0].x = xx;
        Q[0].y = yy;
        Q[1] = P[1];
        Q[2] = P[2];
        for(i = 3; i < n;)
        {
            if(top && (corner(Q[top-1], Q[top], P[i]) < 0))
                top --;
            else
                Q[++ top] = P[i ++];
        }
        printf("RESULT:\n");
        for(i=0; i<=top; i++)
            printf("       %d    %d\n", Q[i].x, Q[i].y);
        printf("\nPLEASE INPUT THE SIZE OF THE SET:\n");
    }
    return 0;
}