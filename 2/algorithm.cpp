#include "algorithm.h"
#include <math.h>

int stringSearch(std::ifstream& f, std::string pattern, int algNum)
{
    ALG alg; //alg là con trỏ hàm trỏ đến thuật toán tương ứng
    switch (algNum)
    {
        case BF:
            alg = BruteForce;
            break;

        case RK:
            alg = RapinKarp;
            break;

        case KMP:
            alg = KnuthMorrisPath;
            break;

        case BM:
            alg = BoyerMoore;
            break;
    }

    return alg(f, pattern);
}

bool stringCompare(std::string& s1, int s1_start, std::string& s2, int s2_start, int numCmp) //mô phỏng std::compare trong string 
                                                                                             //vỉ có thể C++ đã tối ưu
{
    int i = s1_start;
    int len_s1 = s1.length();
    int j = s2_start;
    int len_s2 = s2.length();
    while (numCmp > 0 && i < len_s1 && j < len_s2)
    {
        if (s1[i] < s2[j])
            return -1;
        
        if (s1[i] > s2[j])
            return 1;
        
        numCmp--;
        i++;
        j++;
    }
    return 0;
}


int BruteForce(std::ifstream& f, std::string pattern)
{
    int res = 0;
    int patLen = pattern.length();
    while (!f.eof()) //File có nhiều dòng, so khớp chuỗi trên từng dòng 
                     //và trả về tổng các chuỗi khớp trên tất cả dòng
    {
        std::string text;
        getline(f, text);
        
        res += BruteForce(text, pattern);
    }
    return res;
}

int BruteForce(std::string text, std::string pattern)
{
    int res = 0;
    int texLen = text.length();
    int patLen = pattern.length();

    for (int i = 0; i < texLen - patLen + 1; i++)
        if (stringCompare(text, i, pattern, 0 , patLen) == 0) //so sánh pattern với text tại mọi vị trí của text 
            res++;
    return res;
}

int RapinKarp(std::ifstream& f, std::string pattern)
{
    int res = 0;

    //tính trước hash number cho xâu pattern
    int mod = pow(10, 9) + 7; //lấy số mod nguyên tố làm giảm đụng độ
    //lấy hash number cho xâu pattern
    int patLen = pattern.length();
    int patHash = 0;
    for (int i = 0; i < patLen; i++)
        patHash = (long long)(patHash + pattern[i]) * MAXCHARACTER % mod; //MAXCHARACTER lưu số ký tự tối đa có thể
                                                                              //xuất hiện trong cả pattern và text
    while (!f.eof()) //File có nhiều dòng, so khớp chuỗi trên từng dòng 
                     //và trả về tổng các chuỗi khớp trên tất cả dòng
    {
        std::string text;
        getline(f, text);

        res += RapinKarp(text, pattern, patLen, patHash);
    }
    return res;                                                     
}

int RapinKarp(std::string text, std::string pattern, int patLen, int patHash)
{
    int res = 0;
    int mod = pow(10, 9) + 7;                                        
    //lấy hash number cho từng vị trí xâu text
    int texLen = text.length();
    int texHash = 0;
    int window = 0; //lưu số chữ cái trong xâu đã được tính toán trong texHash 
                    //nếu window > patLen thì thực hiện bỏ một kí tự ở đầu trong text Hash
                    //gọi kí tự xóa là a, thực hiện hash - a * MAXCHARACTER ^ patLen

    int leadingBase = 1; //lưu MAXCHARACTER ^ patLen (không thể dùng pow vì có thể quá lớn)
    for (int i = 0; i <= patLen; i++)
        leadingBase = ((long long)leadingBase * MAXCHARACTER) % mod;  

    //thực hiện so khớp chuỗi
    for (int i = 0; i < texLen; i++)
    {
        texHash = ((long long)texHash + text[i]) * MAXCHARACTER % mod;
        window++;
        if (window > patLen)
        {
            texHash = texHash - (text[i - patLen] * (long long)leadingBase) % mod;
            if (texHash < 0)
                texHash = texHash + mod;
            window--;
        }

        if (texHash == patHash && window == patLen) //hai hash number bằng nhau vẫn phải kiểm tra lại vì có thể xảy ra đụng độ
        {
            if (stringCompare(text, i - patLen + 1, pattern, 0 , patLen) == 0)
                res++;
        }
    }        
    return res;
}

int KnuthMorrisPath(std::ifstream& f, std::string pattern)
{
    int res = 0;
    int patLen = pattern.length();
    int* patSuf = new int[patLen]; //tạo mảng lưu độ dài hậu tố dài nhất kết thúc tại vị trí i-1 (mà không phải cả xâu [0..i-1])
    
    patSuf[0] = 0;
    for (int i = 1; i < patLen; i++)
    {
        int sufLen = patSuf[i - 1]; //sufLen lưu độ dài xâu hậu tố đang xét đến
        while (sufLen != 0 && pattern[sufLen] != pattern[i]) //nếu không tồn tại hậu tố hoặc đã tìm được hậu tố thì thoát while
            sufLen = patSuf[sufLen - 1];

        if (sufLen == 0 && pattern[0] != pattern[i]) //nếu không tồn tại hậu tố thì lấy phần tử đầu so sánh
            patSuf[i] = 0;
        else
            patSuf[i] = sufLen + 1;
    }

    while (!f.eof()) //File có nhiều dòng, so khớp chuỗi trên từng dòng 
                     //và trả về tổng các chuỗi khớp trên tất cả dòng
    {
        std::string text;
        getline(f, text);

        res += KnuthMorrisPath(text, pattern, patSuf);
    }

    delete[] patSuf;
    return res;
}

int KnuthMorrisPath(std::string text, std::string pattern, int patSuf[])
{
    int res = 0;

    int sufLen = 0; //biến lưu độ dài hậu tố dài nhất kết thúc tại i-1 trong text
    int texLen = text.length();
    int patLen = pattern.length();

    if (text[0] == pattern[0])
        sufLen = 1;
    for (int i = 1; i < texLen; i++)
    {
        while (sufLen != 0 && pattern[sufLen] != text[i])
            sufLen = patSuf[sufLen - 1];
        
        if (pattern[sufLen] == text[i]) 
            sufLen++;
        
        if (sufLen == patLen) //độ dài hậu tố bằng độ dài xâu pattern -> tìm thấy xâu khớp
            res++;
    } 
    return res;
}

int BoyerMoore(std::ifstream& f, std::string pattern)
{
    int res = 0;

    int patLen = pattern.length();
    int* badShift = new int[MAXCHARACTER];
    int* goodShift = new int[patLen];

    buildBadShift(badShift, pattern); //xây dựng bảng dịch chuyển khi gặp bad rule character
    buildGoodShift(goodShift, pattern); //xây dựng bảng dịch chuyển khi gặp good rule character

    while (!f.eof()) //File có nhiều dòng, so khớp chuỗi trên từng dòng 
                     //và trả về tổng các chuỗi khớp trên tất cả dòng
    {
        std::string text;
        getline(f, text);

        res += BoyerMoore(text, pattern, badShift, goodShift);
    }

    delete[] badShift;
    delete[] goodShift;
    return res;
}

void buildBadShift(int badShift[], std::string pattern)
{
    int patLen = pattern.length(); //xây dựng bảng badShift theo thuật toán Horspool
    for (int i = 0; i < MAXCHARACTER; i++)
        badShift[i] = patLen;
    for (int i = 0; i < patLen - 1; i++)
        badShift[pattern[i]] = patLen - i - 1;
}

void buildGoodShift(int goodShift[], std::string pattern)
{
    int patLen = pattern.length();
    for (int i = 0; i < patLen; i++)
        goodShift[i] = patLen; //khởi tạo giá trị
    goodShift[0] = 1; //không có dữ kiện -> dịch chuyển tầm thường

    //quy tắc 1: VD: cho a[bcd] với bcd là đoạn khớp
    //               tìm [bcd] khác trong pattern sao cho kí tự liền kề khác a
    //          -> dùng KMP theo chiều ngược lại
    int* patPre = new int[patLen];
    patPre[patLen - 1] = 0;
    for (int i = patLen - 2; i >= 0; i--)
    {
        int len = patPre[i + 1]; 

        while (len > 0 && pattern[patLen - len - 1] != pattern[i])
            len = patPre[patLen - len];

        if (len == 0 && pattern[patLen - 1] != pattern[i])
            patPre[i] = 0;
        else
            patPre[i] = len + 1;
    }

    for (int i = 0; i < patLen; i++) 
    {
        int len = patPre[i];
        
        while (len > 0) //tồn tại tiền tố 
        {
            if (i == 0 || pattern[i - 1] != pattern[patLen - len - 1]) //xét điều kiện về kí tự liền kề phải khác nhau 
                goodShift[len] = patLen - len - i;
            len = patPre[patLen - len];
        }
    }

    //quy tắc 2: VD: cho a[bcd] với bcd là đoạn khớp
    //               ta không tìm được đoạn [bcd] phù hợp
    //               tìm đoạn suffix trong đoạn khớp sao cho trùng prefix của pattern
    int len = patPre[0];
    for (int i = patLen - 1; i >= 0; i--)
    {
        if (len == 0) 
            break;
        if (len > i)
            len = patPre[patLen - len];
        goodShift[i] = std::min(goodShift[i], patLen - len);
    }
    delete[] patPre;
}

int BoyerMoore(std::string text, std::string pattern, int badShift[], int goodShift[])
{
    int res = 0;
    int texLen = text.length();
    int patLen = pattern.length();
    
    int i = patLen - 1; //trỏ vào text
    while (i < texLen)
    {
        int j = patLen - 1; //trỏ vào pattern
        
        while (j > -1 && text[i + j - (patLen - 1)] == pattern[j]) //kiểm tra từng ký tự
            j--;
        if (j == -1)
        {
            res++;
            i++;
        }
        else //phát hiện ký tự lỗi -> dịch chuyển i
            i += std::max(badShift[text[i + j - (patLen - 1)]], goodShift[patLen - j - 1]);
    }
    return res;
}