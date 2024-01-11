#pragma once
struct LEADER;
struct TRAILER
{
    LEADER* id; //công việc có thể làm sau khi công việc của leader hoàn thành
    TRAILER* next;
};

struct LEADER 
{
    int key; //công việc hiện tại 
    int degIn; //số bậc vào - số các công việc ở trước công việc hiệnt tại
    LEADER* next;      
    TRAILER* thead; //danh sách các công việc ở đằng sau công việc hiện tại
};

struct LLIST //danh sách các leader
{
    LEADER* head;
    LEADER* tail;
};

TRAILER* createTrailer();
LEADER* createLeader(int key);
LEADER* addLeader(LLIST& list, int key, int& listSize);
void delTrailer(LEADER*& leader);
void delListSystem(LLIST& list);
int getListSystem(LLIST& list, std::string str);
void printLeader(LLIST list);