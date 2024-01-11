#include <iostream>
#include <fstream>
#include <math.h>
#include "multi-link-list.h"
#include "radix-sort.h"

int main()
{
    TLIST tlist = {nullptr};
    std::ifstream f = openFile("input.txt");
    int k, n;
    f >> k >> n;
    for (int i = 0; i < n; i++) //tạo danh sách liên kết gồm các trailer
    {
        int number; 
        f >> number;
        addHeadTrailer(tlist, number);
    }
    radixSort(tlist, k);

    for (TRAILER* trailer = tlist.head; trailer; trailer=trailer->next)
        std::cout << trailer->number << " ";
    delTrailer(tlist);
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

int getDigit(int number, int k, int i) //chia các chữ số thành các phần gồm k chữ số
                                       //lấy đoạn thứ i (i từ 0) từ phải qua trái
{
    return (number / (int)pow(10, k * i)) % (int)pow(10, k);
}

void radixSort(TLIST& tlist, int k)
{
    if (!tlist.head)
        return;

    int max = tlist.head->number;
    for (TRAILER* trailer = tlist.head; trailer; trailer = trailer->next) 
        if (max < trailer->number)
            max = trailer->number; //tìm số lớn nhất trong trailer list
    int times = ceil(ceil(log10(max)) / k); //tính toán số lần thực hiện vòng for 
                                            //sử dụng số lớn nhất để tính toán
                                            //công thức: số số chữ số chia số khoảng chia yêu cần (làm tròn lên)
    
    for (int i = 0; i < times; i++)
    {
        LLIST llist = {nullptr};
        TRAILER* trailer = tlist.head;
        do
        {
            int digit = getDigit(trailer->number, k, i);
            LEADER* leader = getLeader(llist, digit); //trả về leader giữ digit 
                                                      //nếu chưa tồn tại, tạo mới rồi trả về
            
            //addTail trailer để giữ tính stable của radix sort
            TRAILER* curTrailer = trailer; //lưu lại trailer hiện tại
            trailer = trailer->next;

            ////nếu leader đang xét chưa có trailer -> thêm vào và cập nhật trailerHead và trailerTail
            if (!leader->trailerHead)
            {
                leader->trailerHead = leader->trailerTail = curTrailer;
                curTrailer->next = nullptr;
                continue;
            }
            ////nếu leader đã có trailer
            curTrailer->next = nullptr;
            leader->trailerTail = (leader->trailerTail)->next = curTrailer;
        } while (trailer);

        //sau khi hoàn thành đưa các trailer vào đúng leader có digit theo thứ tự tăng dần
        //-> nối các trailer lại với nhau
        tlist.head = (llist.head)->trailerHead;
        for (LEADER* leader = llist.head; leader->next; leader = leader->next)
            (leader->trailerTail)->next = (leader->next)->trailerHead;
        delLeader(llist); //sau khi nối thì xóa hệ thống leader
    }
}
