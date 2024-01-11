#include <iostream>
#include "circularLL.h"

void removeAll(List* &L) //hàm xóa các phần tử trong danh sách liên kết vòng
{
    if (!L) //danh sách liên kết vòng không tồn tại
        return;

    if (!L->p_head) //danh sách liên kết vòng không chứa phần tử nào
        return;

    NODE* node = L->p_head; //node cần được xóa
    do
    {
        NODE* next = node->p_next;
        delete node;
        node = next;
    } while (node != L->p_head);
}

NODE* createNode(int data)
{
    NODE* node = new NODE;
    if (!node) //lỗi
        return nullptr;
    node->key = data;
    node->p_next = nullptr;
    return node;
}

NODE* getLastNode(List* &L)
{
    NODE* lastNode = L->p_head;
    for (; lastNode->p_next != L->p_head; lastNode = lastNode->p_next); //tìm node cuối cùng trong danh sách liên kết vòng
    return lastNode;
}

bool addHead(List* &L, int data)
{
    NODE* newNode = createNode(data); //tạo node mới
    if (!newNode) //lỗi
        return false;

    if (!L) //danh sách liên kết vòng chưa tồn tại
        return false; 

    if (!L->p_head) //danh sách liên kết vòng không chứa phần tử nào
    {
        L->p_head = newNode;
        newNode->p_next = newNode;
        return true; 
    }

    NODE* lastNode = getLastNode(L);
    lastNode->p_next = newNode; //node cuối cùng nối đến node mới
    newNode->p_next = L->p_head; //node mới liên kết đến head hiện tại
    L->p_head = newNode; //cập nhật head của danh sách liên kết
    return true;
}

bool addTail(List* &L, int data)
{
    NODE* newNode = createNode(data); //tạo node mới
    if (!newNode) //lỗi
        return false;

    if (!L) //danh sách liên kết vòng chưa tồn tại
        return false; 

    if (!L->p_head) //danh sách liên kết vòng không chứa phần tử nào
    {
        L->p_head = newNode;
        newNode->p_next = newNode;
        return true; 
    }

    NODE* lastNode = getLastNode(L);
    lastNode->p_next = newNode; //node cuối cùng nối đến node mới
    newNode->p_next = L->p_head; //node mới liên kết đến head hiện tại
    return true;
}

void printList(List* L)
{
    if (!L)
    {
        std::cout << "Circular link list is not available!\n";
        return;
    }

    if (!L->p_head)
    {
        std::cout << "There is no element in circular link list.\n";
        return;
    }

    std::cout << "Elements in circular link list: ";
    NODE* node = L->p_head;
    for (; node->p_next != L->p_head; node = node->p_next)
        std::cout << node->key << " ";
    std::cout << node->key << "\n"; //in ra phần tử cuối cùng trong danh sách liên kết vòng
}

void removeHead(List* &L)
{
    if (!L || !L->p_head) 
        return;

    bool isOneNode = false; //biến kiểm tra danh sách có nhiều hơn một node hay không
    if (L->p_head == L->p_head->p_next)
        isOneNode = true;

    //đảm bảo đúng với danh sách liên kết có một node
    NODE* lastNode = getLastNode(L);
    NODE* delNode = L->p_head; //node cần xóa
    lastNode->p_next = L->p_head->p_next; 
    L->p_head = L->p_head->p_next; //cập nhật head mới của danh sách liên kết
    
    delete delNode;
    if (isOneNode)
        L->p_head = nullptr; //cập nhật lại head
}

void removeTail(List* &L)
{
    if (!L || !L->p_head) 
        return;

    bool isOneNode = false; //biến kiểm tra danh sách có nhiều hơn một node hay không
    if (L->p_head == L->p_head->p_next)
        isOneNode = true;

    //đảm bảo đúng với danh sách liên kết có một node
    NODE* prevLastNode = L->p_head; //nút kế cuối
    for (; prevLastNode->p_next->p_next != L->p_head; prevLastNode = prevLastNode->p_next); //tìm node cuối cùng trong danh sách liên kết vòng 
    NODE* delNode = prevLastNode->p_next; //nút cần xóa là nút cuối cùng
    prevLastNode->p_next = L->p_head;

    delete delNode;
    if (isOneNode)
        L->p_head = nullptr; //cập nhật lại head
}

NODE* findNodeVal(List* &L, int val)
{
    if (!L || !L->p_head)
        return nullptr;

    NODE* node = L->p_head;
    do 
    {
        node = node->p_next;
        if (node->key == val)
            return node;
    } while (node != L->p_head);
    return nullptr;
}

bool addAfter(List* &L, int data, int val) //thêm vào node có giá trị data vào sau node đầu tiên có giá trị val
{
    if (!L || !L->p_head)
        return false; //thêm không thành công
    
    NODE* nodeVal = findNodeVal(L, val);
    if (!nodeVal) //không tồn tại node có giá trị val
        return false;
    
    //đã tìm được node có giá trị val
    NODE* newNode = createNode(data);
    newNode->p_next = nodeVal->p_next;
    nodeVal->p_next = newNode;
    return true;
}

void removeAfter(List* &L, int val)
{
    if (!L || !L->p_head)
        return;
    
    NODE* nodeVal = findNodeVal(L, val);
    if (!nodeVal) //không tồn tại node có giá trị val
        return;

    //đã tìm được node có giá trị val
    //tồn tại 3 trường hợp
    // - Tầm thường.
    // - Danh sách có một node -> gán head bằng nullptr
    // - Nút xóa là nút head -> gán head bằng nút kế tiếp
    bool isOneNode = false; //biến kiểm tra danh sách có nhiều hơn một node hay không
    if (L->p_head == L->p_head->p_next)
        isOneNode = true;

    NODE* delNode = nodeVal->p_next;
    nodeVal->p_next = nodeVal->p_next->p_next;

    if (L->p_head == delNode) //nút xóa là nút head
        L->p_head = L->p_head->p_next;

    if (isOneNode) //danh sách có một node
        L->p_head = nullptr;
    delete delNode;
}