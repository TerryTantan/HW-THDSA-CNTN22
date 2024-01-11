#pragma once
#include <string>
#include <fstream>
#include "listSystem.h"

std::ifstream openFile(const char path[]);
bool getPair(std::string& str, int& x, int& y);
bool topoSort(LLIST& list, LLIST& sortList, int listSize, int& sortListSize);