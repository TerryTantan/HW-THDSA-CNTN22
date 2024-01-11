#pragma once
#include <fstream>
#include "multi-link-list.h"

std::ifstream openFile(const char path[]);
int getDigit(int number, int k, int i);
void radixSort(TLIST& tlist, int k);