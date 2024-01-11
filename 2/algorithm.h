#pragma once
#include "command.h"
#include <string>
#include <fstream>
#include <iostream>

#define BF 0
#define RK 1
#define KMP 2
#define BM 3
#define MAXCHARACTER 256

typedef int (*ALG) (std::ifstream& f, std::string pattern);

int stringSearch(std::ifstream& f, std::string pattern, int algNum);
bool stringCompare(std::string& s1, int s1_start, std::string& s2, int s2_start, int numCmp);
int BruteForce(std::ifstream& f, std::string pattern);
int BruteForce(std::string text, std::string pattern);
int RapinKarp(std::ifstream& f, std::string pattern);
int RapinKarp(std::string text, std::string pattern, int patLen, int patHash);
int KnuthMorrisPath(std::ifstream& f, std::string pattern);
int KnuthMorrisPath(std::string text, std::string pattern, int suffix[]);
int BoyerMoore(std::ifstream& f, std::string pattern);
void buildBadShift(int badShift[], std::string pattern);
void buildGoodShift(int goodShift[], std::string pattern);
int BoyerMoore(std::string text, std::string pattern, int badShift[], int goodShift[]);

