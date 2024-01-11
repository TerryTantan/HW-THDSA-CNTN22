#include "command.h"
#include <iostream>
#include <chrono>
#include <string.h>

std::ifstream openFile(const char path[])
{
    std::ifstream f;
    f.open(path);
    if (!f)
    {
        std::cerr << "File cannot be opened.\n";  
        exit(-1);
    }
    return f;
}

int getAlg(const char alg[])
{
    if (stricmp(alg, "BF") == 0)
        return BF;
    
    if (stricmp(alg, "RK") == 0)
        return RK;

    if (stricmp(alg, "KMP") == 0)
        return KMP;

    if (stricmp(alg, "BM") == 0)
        return BM;
    
    return -1;
}

void command(int argc, char* argv[])
{
    if (argc <= 3)
    {
        std::cerr << "Wrong syntax.\n";
        exit(0);
    }

    std::ifstream f = openFile(argv[1]); //mở file text
    std::string pattern(argv[2]); //lấy xâu pattern
    int algNum = getAlg(argv[3]); //lấy thuật toán sử dụng
    if (algNum == -1)
    {
        std::cerr << "No algorithm available!\n";
        exit(0);
    }

    auto start = std::chrono::high_resolution_clock::now();
    int res = stringSearch(f, pattern, algNum);
    auto end = std::chrono::high_resolution_clock::now();   
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << res << " - " << 1.0 * dur.count()/1000000; //bắt đầu so khớp chuỗi
    f.close();
}


