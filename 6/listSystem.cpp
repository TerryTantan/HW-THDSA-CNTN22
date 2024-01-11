#include <iostream>
#include "listSystem.h"
#include "topo-sort.h"
TRAILER* createTrailer()
{
    TRAILER* trailer = new TRAILER;
    if (!trailer)
    {
        std::cerr << "Cannot allocate memory!";
        return nullptr;
    }
    trailer->id = nullptr;
    trailer->next = nullptr;
    return trailer;
}

LEADER* createLeader(int key)
{
    LEADER* leader = new LEADER;
    if (!leader)
        return nullptr;
    leader->key = key;
    leader->degIn = 0;
    leader->next = nullptr;
    leader->thead = nullptr;
    return leader;
}

LEADER* addLeader(LLIST& list, int key, int& listSize)
{
    list.tail->key = key; //tail luôn trỏ đến leader lính canh
    LEADER* leader = list.head;
    while (leader->key != key) //tìm leader có key trong danh sách
        leader = leader->next;
     
    //nếu leader tìm thấy là lính canh -> tạo mới
    if (leader == list.tail)
    {
        LEADER* newLeader = createLeader(-1); //tạo lính canh mới
        leader->next = newLeader;
        leader->degIn = 0;
        list.tail = newLeader; //cập nhật tail
        listSize++;
    }
    return leader;
}

void delTrailer(LEADER*& leader)
{
    if (!leader->thead)
        return;
    TRAILER* trailer = leader->thead;
    do
    {
        TRAILER* delTrailer = trailer;
        trailer = trailer->next;
        delete trailer;
    } while (trailer);
}

void delListSystem(LLIST& list) //xóa hệ thống leader và trailer
{
    if (!list.head)
        return; 

    LEADER* leader = list.head;
    do
    {
        LEADER* delLeader = leader;
        leader = leader->next;
        delete delLeader;
    } while (leader);
}

int getListSystem(LLIST& list, std::string str)
{
    int x, y;
    int listSize = 0;
    list.head = list.tail = createLeader(-1); //tạo một lính canh;
    if (!list.head)
        return -1;
    while (getPair(str, x, y))
    {
        //thêm x và y vào hệ thống leader (nếu đã có thì không thêm)
        //và trả về con trỏ đến leader đó
        LEADER* xLeader = addLeader(list, x, listSize); 
        LEADER* yLeader = addLeader(list, y, listSize);
        if (!xLeader || !yLeader)
            return -1;
        
        TRAILER* trailer = createTrailer();
        if (!trailer)
            return -1;
        trailer->id = yLeader; //nối x trailer vào y leader
        //addHead leader
        trailer->next = xLeader->thead; 
        xLeader->thead = trailer;
        //tăng bậc vào của leader y
        yLeader->degIn++;
    }
    return listSize;
}

void printLeader(LLIST list)
{
    for (LEADER* leader = list.head; leader; leader = leader->next)
        std::cout << leader->key << " ";
}