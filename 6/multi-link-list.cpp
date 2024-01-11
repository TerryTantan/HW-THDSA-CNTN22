#include <iostream>
#include "multi-link-list.h"
TRAILER* createTrailer(int number)
{
    TRAILER* trailer = new TRAILER;
    if (!trailer)
    {
        std::cerr << "Cannot allocate memory\n";
        return nullptr;
    }
    trailer->next = nullptr;
    trailer->number = number;
    return trailer;
}

LEADER* createLeader(int digit)
{
    LEADER* leader = new LEADER;
    if (!leader)
    {
        std::cerr << "Cannot allocate memory\n";
        return nullptr;
    }
    leader->digit = digit;
    leader->next = nullptr;
    leader->trailerHead = nullptr;
    leader->trailerTail = nullptr;
    return leader;
}

void addHeadTrailer(TLIST& tlist, int number)
{
    TRAILER* newTrailer = createTrailer(number); 
    if (!newTrailer)
        return;

    if (tlist.head)
        newTrailer->next = tlist.head;
    tlist.head = newTrailer;
}

void delTrailer(TLIST& tlist)
{
    for (TRAILER* trailer = tlist.head; trailer; trailer = trailer->next)
        delete trailer;
    tlist.head = nullptr;
}

void delLeader(LLIST& llist)
{
    for (LEADER* leader = llist.head; leader; leader = leader->next)
        delete leader;
    llist.head = nullptr;
}

LEADER* getLeader(LLIST& llist, int digit)
{
    if (!llist.head) //leader list chưa có phần tử nào -> tạo phần tử đầu tiên
    {
        LEADER* newLeader = createLeader(digit);
        if (!newLeader)
            return nullptr;
        llist.head = newLeader;
        return llist.head;
    }

    //tìm leader với digit đã cho
    LEADER* fakeHead = createLeader(-1); //tạo một head giả trỏ đến head thật sự
    fakeHead->next = llist.head;
    LEADER* leader = fakeHead;
    while (leader->next && (leader->next)->digit < digit) //tìm phần tử cuối cùng 
                                                          //hoặc phần tử tiếp theo có digit lớn hơn hoặc bằng digit đã cho
            leader = leader->next;
    //nếu phần tử tiếp theo là nullptr hoặc có digit lớn hơn 
    //-> không tồn tại leader với digit đã cho
    //-> tạo leader mới
    if (!leader->next || (leader->next)->digit > digit)
    {
        LEADER* newLeader = createLeader(digit);
        newLeader->next = leader->next;
        if (leader == fakeHead)
            llist.head = newLeader;
        else
            leader->next = newLeader;
        return newLeader;
    }

    //trường hợp còn lại: phần tử tiếp theo có digit bằng với digit cho trước 
    //-> đã tồn tại leader với digit cho trước
    //-> trả về leader tiếp theo 
    return leader->next;
}
