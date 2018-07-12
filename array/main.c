#include <stdio.h>

void changeA(int *a){
    a[0]=10;
}

void changeB(int b[]){
    b[0]=10;
}

int main(){
    int a[5]={1,2,3,4,5};
    changeA(a);
    printf("%d\n",a[0]);

    int b[5]={1,2,3,4,5};
    changeB(b);
    printf("%d\n",b[0]);

    int *c={1,2,3,4,5};
    changeA(c);
    printf("%d\n",c[0]);
}

