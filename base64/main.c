//
//  main.c
//  base64
//
//  Created by guofu on 2017/5/25.
//  Copyright © 2017年 guofu. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include "base64.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    char str1[] = "abcdefg";
    char str2[20] = {0}; //注意长度要给够
    int len = 0;
    base64_encode(str1,(int)strlen(str1),str2,&len);
    printf("%s, len = %d\n", str2, len);

    
    char str3[10] = {0};
    base64_decode(str2, (int)strlen(str2), str3, &len);
    
    printf("%s, len = %d\n", str3, len);
    
    
    return 0;
}