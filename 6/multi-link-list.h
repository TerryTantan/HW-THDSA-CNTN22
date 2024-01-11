#pragma once
struct TRAILER //node quản lý các số dữ liệu thực sự
{
    int number;
    TRAILER* next;
};

struct TLIST //danh sách các trailer
{
    TRAILER* head;
};

struct LEADER //node quản lý các chữ số dùng để so sánh được sắp xếp thứ tự tăng dần của digit
{
    int digit;
    LEADER* next;
    TRAILER* trailerHead;
    TRAILER* trailerTail;
};

struct LLIST //danh sách các leader
{
    LEADER* head;
};

TRAILER* createTrailer(int number);
LEADER* createLeader(int digit);
void addHeadTrailer(TLIST& tlist, int number);
void delTrailer(TLIST& tlist);
void delLeader(LLIST& llist);
LEADER* getLeader(LLIST& llist, int digit);