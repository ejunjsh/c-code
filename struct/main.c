#include <stdio.h>

typedef struct test{
    int a;
    short b;
} t ;

void change(t *_t){
    _t->a=100;
}

void change1(t _t){
    _t.b=1000;
}

int main(){
    t c;
    c.a=1;
    c.b=2;

    change(&c);
    change1(c);

    printf("%d %d\n",c.a,c.b);
}