
#pragma pack(1)
struct test{
    int c;
    char a;
    int e;
    short b;
};
#pragma pack()

struct test1{
    int c;
    char a;
    int e;
    short b;
};

struct test2{
    int c;
    int e;
    short b;
    char a;
};

struct test3{
    char a;
    short b;
};

struct test4{
    char a;
    char b;
};



int main(){
    printf("%d\n",sizeof(struct test));
    printf("%d\n",sizeof(struct test1));
    printf("%d\n",sizeof(struct test2));
    printf("%d\n",sizeof(struct test3));
    printf("%d\n",sizeof(struct test4));
}