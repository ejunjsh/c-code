#include <stdio.h>

void changeA(int *a){
    a[0]=10;
}

void changeB(int b[]){
    b[1]=20;
}

int main(){
    int a[5]={1,2,3,4,5};
    changeA(a);
    printf("%d %d %d %d %d \n",a[0],a[1],a[2],a[3],a[4]);

    int b[5]={6,7,8,9,10};
    changeB(b);
    printf("%d %d %d %d %d \n",b[0],b[1],b[2],b[3],b[4]);

    int *c={1,2,3,4,5};
    changeA(c);
    printf("%d\n",c[0]);
}

