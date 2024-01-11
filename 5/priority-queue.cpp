#include <iostream>
#include <string>

struct ITEM
{
    std::string ID;
    int order;
    int priority; //độ ưu tiên càng nhỏ càng ở vị trí cao hơn
};

//DSLK đơn
struct NODE
{
    ITEM item;
    NODE* next;
};

struct LINKEDLIST //sử dụng danh sách liên kết đơn được sắp xếp tăng dần
{
    NODE* head;
};

bool isEmpty(LINKEDLIST list)
{
    return (list.head == nullptr);
}

NODE* createNewNode(std::string ID, int order, int priority)
{
    NODE* node = new NODE;
    node->item.ID = ID;
    node->item.order = order;
    node->item.priority = priority;
    node->next = nullptr;
    return node;
}

void Insert(LINKEDLIST& list, std::string ID, int order, int priority) //thêm vào theo thứ tự
{
    NODE* newNode = createNewNode(ID, order, priority); //node chuẩn bị thêm vào list
    if (isEmpty(list))
        list.head = newNode;
    else if (priority <= list.head->item.priority) //add Head
    {
        newNode->next = list.head;
        list.head = newNode;
    }
    else
    {
        NODE* node = list.head;
        for (; node->next; node = node->next)
            if (node->item.priority <= priority && priority <= node->next->item.priority) //tìm được vì trí thích hợp
                break;

        //đã tìm được vị trí thích hợp hoặc độ ưu tiên hiện tại là lớn nhất trong danh sách
        newNode->next = node->next;
        node->next = newNode;
    }
}

std::string Extract(LINKEDLIST list)
{
    if (isEmpty(list))
        return "";

    std::string ID = list.head->item.ID; //trích xuất dữ liệu

    //delete Head
    NODE* deleteNode = list.head;  
    list.head = list.head->next;
    delete deleteNode;

    return ID;
}

void Remove(LINKEDLIST& list, std::string ID)
{
    if (isEmpty(list))
        return;
    
    NODE* delNode; //item xóa 
    if (list.head->item.ID == ID) //remove Head
    {
        delNode = list.head;
        list.head = list.head->next;
    }
    else 
    {
        NODE* node = list.head;
        for (; node->next && node->next->item.ID != ID; node = node->next); //tìm item có ID đã cho
        if (!node->next) //không tồn ID đã cho
            return;
        //tồn tại 
        delNode = node->next;
        node->next = delNode->next;
    }
    delete delNode;
}

void deleteList(LINKEDLIST& list)
{
    while (list.head)
    {
        NODE* deleteNode = list.head;
        list.head = list.head->next;
        delete deleteNode;
    }
}

void printList(LINKEDLIST list)
{
    for (NODE* node = list.head; node; node = node->next)
        std::cout << node->item.ID << " ";
    std::cout << "\n";
}

void changePriority(LINKEDLIST& list, std::string ID, int newPriority)
{
    for (NODE* node = list.head; node; node = node->next)
        if (node->item.ID == ID)
        {
            int order = node->item.order;
            Remove(list, ID);
            Insert(list, ID, order, newPriority);
        }
}

//heap
#define HEAP_MAX_SIZE 10000
struct HEAP
{
    ITEM arr[HEAP_MAX_SIZE];
    int tail = 0; //vị trí phần tử chuẩn bị được thêm vào heap
};

bool isEmpty(HEAP heap)
{
    return (heap.tail == 0);
}

bool isFull(HEAP heap)
{
    return (heap.tail > HEAP_MAX_SIZE);
}

void Insert(HEAP& heap, std::string ID, int order, int priority)
{
    if (isFull(heap))
        return;
    //thêm vào cuối mảng 
    heap.arr[heap.tail] = {ID, order, priority};
    heap.tail++;

    //update từ lá lên đỉnh
    int i_parent; //trỏ đến nút cha
    int i = heap.tail - 1; //nút hiện tại đang xét
    while (i != 0) //khi nút đang xét chưa lên đến gốc
    {
        i_parent = (i - 1) / 2;
        if (heap.arr[i_parent].priority > heap.arr[i].priority)
            std::swap(heap.arr[i_parent], heap.arr[i]);
        else break;
        i = i_parent;
    }
}

std::string Extract(HEAP& heap)
{
    if (isEmpty(heap))
        return "";

    std::string ID = heap.arr[0].ID;
    std::swap(heap.arr[0], heap.arr[heap.tail - 1]);
    heap.tail--;
    
    int node = 0; //i là nút hiện tại đang xét
    int child = node * 2 + 1; //j là nút con trái của i
    while (child < heap.tail)
    {
        if (child < heap.tail - 1) //nếu i có hai con
            if (heap.arr[child + 1].priority < heap.arr[child].priority) //chọn con có độ ưu tiên nhỏ hơn
                child++;
        
        if (heap.arr[node].priority < heap.arr[child].priority)
            break;
        std::swap(heap.arr[node], heap.arr[child]);
        //cập nhật nút
        node = child;
        child = node * 2 + 1;
    }
    return ID;
}

void Remove(HEAP& heap, std::string ID)
{
    for (int i = 0; i < heap.tail; i++)
        if (heap.arr[i].ID == ID) //đả tìm được vị trí xóa 
        {
            std::swap(heap.arr[i], heap.arr[heap.tail - 1]); //swap với phần tử cuối cùng
            heap.tail--;
            
            int node = i;
            int child = i * 2 + 1;
            while (child < heap.tail)
            {
                if (child < heap.tail - 1)
                    if (heap.arr[child + 1].priority < heap.arr[child].priority)
                        child++;
                if (heap.arr[node].priority < heap.arr[child].priority)
                    return;
                std::swap(heap.arr[node], heap.arr[child]);
                node = child;
                child = node * 2 + 1;
            }
            return;
        }
}

void changePriority(HEAP& heap, std::string ID, int newPriority)
{
    for (int i = 0; i < heap.tail; i++)
        if (heap.arr[i].ID == ID)
        {
            int order = heap.arr[i].order; 
            Remove(heap, ID);
            Insert(heap, ID, order, newPriority);
            break;
        }
}
