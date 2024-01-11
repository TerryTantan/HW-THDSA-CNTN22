#include <iostream>
#include <fstream>
#include <string>
#include "listSystem.h"
#include "topo-sort.h"
struct LEADER;

int main()
{   
    LLIST list = {nullptr, nullptr};

    std::ifstream f = openFile("input.txt");
    std::string str; 
    f >> str; //lấy xâu ký tự

    //tạo hệ thống leader và trailer từ dữ liệu đề bài và trả vệ số số leader
    int listSize = getListSystem(list, str); 
    if (listSize == -1) //có lỗi
    {
        delListSystem(list);
        return -1;
    }

    //sắp xếp topo
    LLIST sortList = {nullptr, nullptr}; //danh sách các leader đã được sắp xếp topo
    int sortListSize = 0; //số phần tử đã vào trong danh sách sắp xếp topo
    topoSort(list, sortList, listSize, sortListSize);
    printLeader(sortList);

    delListSystem(list);
    f.close();
    return 0;
}

std::ifstream openFile(const char path[])
{
    std::ifstream f;
    f.open(path);
    if (!f)
    {
        std::cerr << "Cannot open file!\n";
        exit(-1);
    }
    return f;
}

bool getPair(std::string& str, int& x, int& y) //hàm lấy một cặp x y -> thành công trả về true, không thành công trả về false
{
    if (str.empty())
        return false;
    str.erase(str.begin()); //xóa dấu "("
    int cmPos = str.find(",", 0); //vị trí dấu "," đầu tiên
    std::string xStr = str.substr(0, cmPos); //lấy số đầu tiên
    str.erase(0, cmPos + 1); //xóa số đầu tiên và dấu ","

    int clBrPos = str.find(")", 0); //vị trí dấu ")" đầu tiên
    std::string yStr = str.substr(0, clBrPos); //lấy số thứ hai
    str.erase(0, clBrPos + 1); //xóa số thứ hai và dấu ")"

    x = std::stoi(xStr);
    y = std::stoi(yStr);
    return true;
}


bool topoSort(LLIST& list, LLIST& sortList, int listSize, int& sortListSize) //nếu xuất hiện lỗi thì trả về false
{
    if (!list.head) //không tồn tại phần tử nào thì không thực hiện sort
        return true;

    //tìm các leader có bậc vào bằng 0
    LEADER* leader = list.head;
    do
    {
        LEADER* checkLeader = leader;
        leader = leader->next;

        if (!checkLeader->degIn) //kiểm tra bậc vào bằng 0
        {            
            if (!sortList.head) //chưa tồn tại phần tử nào
                sortList.head = sortList.tail = checkLeader; 
            //addTail danh sách
            sortList.tail->next = checkLeader;
            checkLeader->next = nullptr;
            sortList.tail = checkLeader;
            sortListSize++;
        }
    }
    while (leader != list.tail);

    //sau khi tìm nhưng không tồn tại leader nào bằng 0 -> đồ thị đã cho không phải DAG
    if (!sortList.head)
        return false;
    
    //bắt đầu topo sort
    leader = sortList.head;
    while (leader)
    {
        for (TRAILER* trailer = leader->thead; trailer; trailer = trailer->next) //duyệt tất cả các trailer
        {
            //xóa cạnh leader->(leader->id)
            LEADER* delEdgeLeader = trailer->id; //leader cần xóa một cạnh
            delEdgeLeader->degIn--; //giảm một bậc vào
            if (delEdgeLeader->degIn == 0) //sau khi xóa, nếu (leader->id) bằng 0 thì cho vào danh sách đã sắp xếp bằng cách addTail
            {
                sortList.tail->next = delEdgeLeader;
                delEdgeLeader->next = nullptr;
                sortList.tail = delEdgeLeader;
                sortListSize++;
            } 
        }
        leader = leader->next;
    }
    if (sortListSize < listSize) //đồ thị không phải DAG
        return false;
    return true;
}
