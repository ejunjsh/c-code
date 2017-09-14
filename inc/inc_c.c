#include<stdio.h>
#include<memory.h>
#include<sys/mman.h>

typedef int (* inc_func)(int a); 

int main() {
    char code[] = { 
        0x8d, 0x47, 0x01,    // lea 0x1(rdi), rax
        0xc3                 // ret
    }; 

    void * temp = mmap(NULL, sizeof(code), PROT_WRITE | PROT_EXEC,
            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0); 

    memcpy(temp, code, sizeof(code));
    inc_func p_inc = (inc_func)temp;
    printf("%d\n", p_inc(7));

    return 0;
}