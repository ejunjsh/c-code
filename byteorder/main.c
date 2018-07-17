#include <stdio.h>

int main(){
    char a[10]="abcdefghi";

    printf("%d %d\n",a[0],a[9]);
    printf("%c %c\n",a[0],a[8]);

    int b=0x01020304;

    printf("%d %d\n",((char*)&b)[0],((char*)&b)[3]);
}