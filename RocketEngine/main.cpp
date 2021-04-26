#include <iostream>
#include <memory>

using namespace std;

int main(int argc, char** argv)
{
    int* ptr = new int[10];
    delete[] ptr;
    
    cout << "hello world!" << endl;
    return 0;
}
