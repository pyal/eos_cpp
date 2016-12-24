#include <iostream>

class abc {
    operator int(){};
};
void afunc(char *&aa) {}
void &_aa(char *&aa) {}
int main(int argc, char *agrv[]) {
    std::cout << "OK\n";
    char *a = new char[267];
    const char *aaaa = "AAAA";
    aaaa = a;
    1_aa(a);
    abc *AA = new abc;
    free(AA);
    delete a;
    return 0;
}
