#pragma once

struct NODE
{
    int key;
    NODE* p_next;
};

struct List //struct chứa danh sách liên kết vòng
{
    NODE* p_head;
};

void removeAll(List* &L);
NODE* createNode(int data);
NODE* getLastNode(List* &L);
NODE* findNodeVal(List* &L, int val);

bool addHead(List* &L, int data);
bool addTail(List* &L, int data);
bool addAfter(List* &L, int data, int val);
void removeHead(List* &L);
void removeTail(List* &L);
void removeAfter(List* &L, int val);
void printList(List* L);