/** 
 给定平面上n（nk≥3）个点的集合P，求P的一个最小子集Q，
 使得Q中的点能构成的一个包围P中所有点的多边形。
 请设计一种贪心算法求解此问题，并证明你所设计的贪心策略的正确性，分析算法的时间复杂度。
*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define epsilon 1e-10
#define PI 3.1415916

typedef struct ListNode{
    void* key;
    struct ListNode* prev;
    struct ListNode* next;
} ListNode;
ListNode* createListNode(void* d, int size);
void clrListNode(ListNode* x, void(*proc)(void*));

ListNode* createListNode(void* d, int size) {
    ListNode* x = (ListNode*) malloc(sizeof (ListNode));
	assert(x!=NULL);
	if(d&&size){
		x->key = (void*) malloc(size);
		assert(x->key!=NULL);
		memcpy(x->key, d, size);
		x->prev = x->next = NULL;
	}
    return x;
}

void clrListNode(ListNode* x, void(*proc)(void*)) {
    if (x != NULL) {
        if (proc != NULL)
            proc(x->key);
        free(x->key);
    }
    x->next = x->prev = NULL;
}

typedef struct {
	size_t eleSize;
	int (*comp)(void*, void*);
    ListNode* nil;
    int n;
}LinkedList;
LinkedList *createList(unsigned long size,int(*comp)(void*,void*));
void clrList(LinkedList* L, void(*proc)(void*));
void listPushFront(LinkedList* L, void* k);
void listPushBack(LinkedList* L, void* k);
void listDelete(LinkedList* L, ListNode* e);
LinkedList *createList(unsigned long size,int(*comp)(void*,void*)) {/*ŽŽœšÐÂÁŽ±í*/
    LinkedList *L = (LinkedList *) malloc(sizeof (LinkedList));
	assert(L!=NULL);
    L->nil=createListNode(NULL,0);/*ŽŽœšÉÚ±øœÚµã*/
	L->nil->prev=L->nil->next=L->nil;
    L->n = 0;
	L->eleSize=size;/*ÉèÖÃÔªËØÊýŸÝŽæŽ¢¿í¶È*/
	L->comp=comp;/*ÉèÖÃÔªËØÊýŸÝ±ÈœÏ¹æÔò*/
    return L;
}

void clrList(LinkedList* L, void(*proc)(void*)) {/*ÇåÀíÁŽ±í*/
	ListNode *x=L->nil->next;
	while (x!=L->nil) {
        listDelete(L, x);
        clrListNode(x, proc);
        free(x);
		x=L->nil->next;
    }
	free(L->nil);
	L->comp=NULL;
}

void listInsert(LinkedList* L, ListNode* a, void* k) {/*ÔÚLµÄaÖ®Ç°²åÈëÔªËØk*/
	int size=L->eleSize;
	ListNode* x = createListNode(k, size), *b=a->prev;
    L->n++;
	x->next=a;
	x->prev=b;
	a->prev=b->next=x;
}

void listPushFront(LinkedList* L, void* k){/*ÔÚ±íÊ×²åÈë*/
	listInsert(L,L->nil->next,k);
}



void listDelete(LinkedList* L, ListNode* x) {/*ÔÚÁŽ±íÖÐÉŸ³ýœÚµã*/
	ListNode *a=x->next,*b=x->prev;
	if (x == L->nil)
        return;
	b->next=a;
	a->prev=b;
    L->n--;
}

typedef struct{
    LinkedList *L;
    ListNode *top;
}Stack;
Stack* createStack(unsigned long size);
void clrStack(Stack *S, void(*proc)(void*));
int stackEmpty(Stack* S);
void push(Stack *S,void *k);
ListNode *pop(Stack *S);

Stack* createStack(unsigned long size){
    Stack *S=(Stack*)malloc(sizeof(Stack));
	assert(S!=NULL);
    S->L=createList(size,NULL);
	S->top=S->L->nil->next;
    return S;
}
void clrStack(Stack *S, void(*proc)(void*)){
	S->top=NULL;
	clrList(S->L,proc);
	free(S->L);
}
int stackEmpty(Stack* S){
	return S->top==S->L->nil;
}
void push(Stack *S,void *k){
    listPushFront(S->L,k);
	S->top=S->L->nil->next;
}
ListNode *pop(Stack *S){
    ListNode *x;
    assert(!stackEmpty(S));
	x=S->top;
	S->top=x->next;
    listDelete(S->L,x);
    return x;
}


typedef struct {
    double x, y;
}Point;

double dist(Point *a, Point *b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

Point sub(Point *a, Point *b) {
    Point c = {a->x - b->x, a->y - b->y};
    return c;
}



static int pyxcomp(Point *a, Point *b) {
    if (a->y < b->y)
        return 1;
    if (a->y == b->y && a->x < b->x)
        return 1;
    if (a->y == b->y && a->x == b->x)
        return 0;
    return -1;
}



/**
 * 计算向量的模.
 */
double pabs(Point *a) {
    Point o = {0.0, 0.0};
    return dist(a, &o);
}

/**
 * 向量归一化
 */
void normalize(Point *a) {
    double r = pabs(a);
    if (r >= epsilon)
        a->x /= r, a->y /= r;
}

/**
 * 计算向量a关于b的极角.
 */
double psudoPolarAngle(Point *a, Point *b) {
    Point p1 = sub(a, b);
    normalize(&p1);
    if (pabs(&p1) < epsilon)
        return 2*PI;
    if (p1.y >= 0.0 && p1.x >= 0.0)
        return p1.y;
    if (p1.y >= 0.0 && p1.x <= 0.0)
        return PI/2 - p1.x;
    if (p1.y < 0.0 && p1.x < 0.0)
        return PI - p1.y;
    return 3 * PI/2 + p1.x;
}

static Point *O;
static int angleComp(Point *a, Point *b) {
    double anglea = psudoPolarAngle(a, O), angleb = psudoPolarAngle(b, O);
    if (anglea == angleb)
        return 0;
    if (anglea < angleb)
        return -1;
    return 1;
}

int most(void *a, int n, int size, int(*comp)(void *,void *)){
	void *m=malloc(size);
	int i, k=0;
	memcpy(m,a,size);
	for(i=1;i<n;i++)
		if(comp(((char*)a+i*size),m)>0){
			memcpy(m, ((char*)a+i*size), size);
			k=i;
		}
	free(m);
	return k;
}

void swap(void* x, void* y, int size){
    void* temp=(void*)malloc(size);
    memcpy(temp, x, size);
    memcpy(x, y, size);
    memcpy(y, temp,size);
    free(temp);
}

double crossProduct(Point *a, Point *b) {
    return a->x * b->y - a->y * b->x;
}

int direction(Point *p0, Point *p1, Point *p2) {
    Point p = sub(p2, p0), q = sub(p1, p0);
    double d = crossProduct(&p, &q);
    if (d > 0.0) {
        return 1;
    }
    if (d < 0.0) {
        return -1;
    }
    return 0;

}

long partition(void *a,int size,int p,int r,int(*comp)(void *,void *)){
    int i,j;
    void *x=(void*)malloc(size);
    memcpy((char*)x,(char*)a+r*size,size);/*x←a[r]*/
    i=p-1;
    for(j=p;j<r;j++)
        if(comp((char*)a+j*size,(char*)x)<=0){/*a[j]≤x*/
            i++;
            swap((char*)a+i*size,(char*)a+j*size,size);/*a[i]↔a[j]*/
        }
	 free(x);
     swap((char*)a+(i+1)*size,(char*)a+r*size,size);/* a[i+1]↔a[r]*/
     return i+1;
}

int randomNumber(int p, int q){
    return p+(int)((double)(q-p)*rand()/(RAND_MAX));
}
unsigned long long RangedRandom( unsigned long long range_min, unsigned long long range_max){
	unsigned long long u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)+ range_min;
	return u;
}

long randmizedPartition(void *a,int size,long p,long r,int(*comp)(void *,void *)){
    int i=randomNumber(p,r);
    swap((char*)a+r*size,(char*)a+i*size,size);/*a[r]↔a[i]*/
    return partition(a,size,p,r,comp);
}

void quickSort(void* a,int size,int p,int r,int(*comp)(void *,void *)){
    if(p<r){
        int q=randmizedPartition(a,size,p,r,comp);
        quickSort(a,size,p,q-1,comp);
        quickSort(a,size,q+1,r,comp);
    }
}

Point * grahamScan(Point *p, int n, int *m) {
    Point *q;
    Stack *S = createStack(sizeof(Point));
    int i = most(p, n, sizeof(Point), pyxcomp);
    swap(p, p + i, sizeof(Point));
    O = p;
    quickSort(p + 1, sizeof(Point), 0, n - 1, angleComp);
    push(S, p); push(S, p + 1); push(S, p + 2);
    for (i = 3; i < n; ++ i) {
        ListNode *b = S->top, *a = b->next;
        while (direction(a->key, b->key, p + i) > 0) {
            pop(S);
            b = S->top;
            a = b->next;
        }
        push(S, p + i);
    }
    *m = S->L->n;
    q = (Point *)calloc(*m, sizeof(Point));
    for (i = *m - 1; i >= 0; -- i) {
        ListNode *x = pop(S);
        q[i] = *((Point *)(x->key));
        clrListNode(x, NULL);
        free(x);
    }
    clrStack(S, NULL);
    free(S);
    return q;
}