#pragma once
#include <fstream>
#include "algorithm.h"

std::ifstream openFile(const char path[]);
int getAlg(const char alg[]);
void command(int argc, char* argv[]);
int stringSearch(std::ifstream& f, std::string pattern, int alg);