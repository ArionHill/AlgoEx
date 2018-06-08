#include<stdio.h>

void print(int i)
{
  if(i > 10){
    print(i / 10);
  }
  printf("%d", i % 10);
}

int main()
{
  print(12345);
  printf("\n");
}
