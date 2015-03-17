#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
    int files_count = 10000;
    
    if (argc == 2) {
        files_count = atoi(argv[1]);
    }
    
    for (int i = 0; i < files_count; ++i) {
        char buffer[100];
        sprintf(buffer, "%d", i);
        ofstream fout(buffer);
        for (int j = 0; j < 1000; ++j) {
            fout << 1;
        }       
        fout << endl;
    }
}
