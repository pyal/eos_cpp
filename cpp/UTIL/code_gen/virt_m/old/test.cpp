//#include "zubr_use.h"
//extern "C" int test(int,char**);

//#define BUFFER_SIZE 10
//#include <string.h>
//#include <stdio.h>
//
//void func(const char *s)
//{
//     char buffer[BUFFER_SIZE];
//     if (s) strncpy(buffer, s, BUFFER_SIZE - 1)[BUFFER_SIZE - 1] = '\x00';
//     else buffer[0]='\x00';
//     printf("%s\n", buffer);
//}

#include <string>
#include <iostream>
using namespace ::std;
void main() {
    unsigned char c;
    int i = 0;
    int c1 = 0, c2 = 255;
    for(c = c1; c <= c2; c++) {
        if(c == 0)
            cout << "\n";
        cout << i++;
    }
}

//
//void main(int argc,char **argv)
//{
////	test(argc,argv);
//  func("AAAAaaaaaaaaaaaaaaaaaa");
//  func(NULL);
//}