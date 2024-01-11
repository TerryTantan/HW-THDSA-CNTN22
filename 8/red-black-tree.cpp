#include <iostream>
struct RBNode
{
    int key;
    bool color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
};

#define BLACK false
#define RED true

RBNode* NIL = new RBNode{0, BLACK, nullptr, nullptr, nullptr};

RBNode* createNode(int key)
{
    RBNode* node = new RBNode;
    node->parent = node->left = node->right = NIL;
    node->key = key;
    node->color = RED;
    return node;
}

bool isLeftFromParent(RBNode* node) //xem node có nằm ở cây con trái của cha nó hay không
{
    return (node->parent->left == node);
}

void rotateLeft(RBNode*& pRoot, RBNode* node) //xoay trái
{
    if (!pRoot || pRoot == NIL || node == NIL)
        return;
    
    RBNode* rightNode = node->right;
    //chỉnh sửa cạnh
    node->right = rightNode->left;
    rightNode->left->parent = node;
    rightNode->left = node;
    rightNode->parent = node->parent;
    if (isLeftFromParent(node)) //xem node có bên trái parent hay không
        node->parent->left = rightNode;
    else 
        node->parent->right = rightNode;
    node->parent = rightNode;
    if (pRoot == node) //nếu xoay tại gốc thì chỉnh lại gốc
        pRoot = rightNode;
}

void rotateRight(RBNode*& pRoot, RBNode* node) //xoay phải
{
    if (pRoot == NIL || node == NIL)
        return;

    RBNode* leftNode = node->left;
    //chỉnh sửa cạnh
    node->left = leftNode->right;
    leftNode->right->parent = node;
    leftNode->right = node;
    leftNode->parent = node->parent;
    if (isLeftFromParent(node)) //xem node có bên trái parent hay không
        node->parent->left = leftNode;
    else 
        node->parent->right = leftNode;
    node->parent = leftNode;
    if (pRoot == node) //nếu xoay tại gốc thì chỉnh lại gốc
        pRoot = leftNode;
}

void Insert(RBNode* &pRoot, int key)
{
    if (pRoot == NIL) //không tồn tại phần tử nào trong cây
    {
        pRoot = createNode(key);
        pRoot->color = BLACK;
        return;
    }

    RBNode* node = pRoot;
    RBNode* parent = pRoot->parent; //là nút NIL
    while (node != NIL) //tìm được vị trí add khi node trỏ đến NIL
    {
        parent = node;
        if (key < node->key)
            node = node->left;
        else if (key > node->key)
            node = node->right;
        else 
            return; //key đã tồn tại trong cây
    }

    //tạo và nối cạnh giữa cha và con mới
    node = createNode(key);
    if (key < parent->key)
        parent->left = node;
    else    
        parent->right = node;
    node->parent = parent;    

    //xét hai nút đỏ có trùng nhau hau không
    while (node->parent->color == RED) //node đang có màu đỏ
    {
        parent = node->parent;
        RBNode* grand = parent->parent; //nút ông nội
        RBNode* uncle; //nút chú là nút anh em của cha
        if (isLeftFromParent(parent))
            uncle = grand->right;
        else 
            uncle = grand->left;
        if (uncle->color == RED) //TH1: cha đỏ và chú đỏ -> chuyển cha và chú thành đen, ông nội màu đỏ và tiếp tục kiểm tra tại ông nội
        {
            uncle->color = parent->color = BLACK;
            grand->color = RED;
            node = grand;
        }
        else //nút chú màu đen 
             //TH2: đổi màu và xoay đơn
             //TH3: xoay đơn trở thành TH2
        {            
            //kiểm tra nút hiện tại so với ông nội ở vị trí nào: LL LR RL RR
            if (grand->left->left == node) //LL -> TH2
            {
                grand->color = RED;
                parent->color = BLACK;
                rotateRight(pRoot, grand);
            }
            else if (grand->left->right == node) //LR -> TH3
            {
                rotateLeft(pRoot, parent); //về TH2
                node = parent;
            }
            else if (grand->right->left == node) //RL -> TH3
            {
                rotateRight(pRoot, parent); //về TH2
                node = parent;
            }
            else //RR -> TH2
            {
                grand->color = RED;
                parent->color = BLACK;
                rotateLeft(pRoot, grand);
            }
        }
    }
    pRoot->color = BLACK;
}

RBNode* createTree(int a[], int n)
{
    RBNode* pRoot = NIL;
    for (int i = 0; i < n; i++)
        Insert(pRoot, a[i]);
    return pRoot;
}

void deleteTree(RBNode*& node)
{
    if (node == NIL)
        return;
    
    deleteTree(node->left);
    deleteTree(node->right);
    node = nullptr;
    delete node;
}

RBNode* getDelNode(RBNode* node)
{
    while (node->right != NIL)
        node = node->right;
    return node;
}

void delFixUp(RBNode*& pRoot, RBNode* node) //node là nút đang giữ dấu hiệu đen
{
    while (node != pRoot) //nếu root dấu hiệu đen thì xóa dấu hiệu đen đi
    {
        if (node->color == RED) //nút đỏ đen -> đổi màu đỏ thành đen
        {
            node->color = BLACK;
            break;
        }
        //nút đen kép
        RBNode* parent = node->parent;
        RBNode* brother; //nút anh em với node
        if (isLeftFromParent(node))
            brother = parent->right;
        else 
            brother = parent->left;
        
        if (brother->color == BLACK && brother->left->color == BLACK && brother->right->color == BLACK) 
        //TH1: brother và hai con của brother đều màu đen -> brother thành đỏ và di chuyển dấu hiệu đen lên parent
        {
            brother->color = RED;
            node = parent;
        }
        else if (brother->color == RED) //TH2: brother đỏ -> swap màu brother và parent sau đó thực hiện xoay
        {
            parent->color = RED;
            brother->color = BLACK;
            if (isLeftFromParent(brother))
                rotateRight(pRoot, parent);
            else 
                rotateLeft(pRoot, parent);
        }
        else //TH3: tồn tại một con của brother màu đỏ 
             //TH3a: brother nhận màu parent, parent và nút con của brother màu đỏ thành đen sau đó xoay -> hoàn thành xóa
             //TH3b: swap màu brother và con của brother màu đỏ và trở thành TH3a
        {
            //kiểm tra nút con của brother màu đỏ so với cha ở vị trí nào: LL LR RL RR
            if (isLeftFromParent(node)) //R
            {
                if (brother->right->color == RED) // RR -> TH3a
                {
                    brother->color = parent->color;
                    parent->color = brother->right->color = BLACK;
                    rotateLeft(pRoot, parent);
                }
                else //RL -> TH3b
                {
                    //xử lý TH3b
                    std::swap(brother->color, brother->left->color);
                    rotateRight(pRoot, brother);
                    brother = parent->right; //cập nhật lại brother sau khi xoay

                    //xử lý TH3a
                    brother->color = parent->color;
                    parent->color = brother->right->color = BLACK;
                    rotateLeft(pRoot, parent);
                }
            }
            else //L
            {
                if (brother->right->color == RED) //LR -> TH3b
                {
                    std::swap(brother->color, brother->right->color);
                    rotateLeft(pRoot, brother);
                    brother = parent->left; //cập nhật lại brother sau khi xoay

                    //xử lý TH3a
                    brother->color = parent->color;
                    parent->color = brother->left->color = BLACK;
                    rotateRight(pRoot, parent);
                }
                else //LL -> TH3a
                {
                    brother->color = parent->color;
                    parent->color = brother->left->color = BLACK;
                    rotateRight(pRoot, parent);
                }
            }
            break;
        }
    }
}

void Remove(RBNode*& pRoot, int key)
{
    if (!pRoot || pRoot == NIL) 
        return;
    
    RBNode* node = pRoot;
    //tìm node có key đã cho trước
    while (node != NIL && node->key != key)
    {
        if (key < node->key)
            node = node->left;
        else 
            node = node->right;
    }

    if (node == NIL) //không có node có key cho trước
        return;
    //tìm thấy node có key cho trước
    RBNode* delNode = node; //nút xóa thật sự
    if (node->left != NIL && node->right != NIL) //node có hai con
    {
        delNode = getDelNode(node->left); //nút xóa thật sự là node phải cùng cây con trái
        node->key = delNode->key;
    }

    //thực hiện xóa cây
    RBNode* checkNode; //node dùng để thực hiện kiểm tra
    if (delNode->left == NIL)
    {
        if (isLeftFromParent(delNode))
            delNode->parent->left = delNode->right;
        else 
            delNode->parent->right = delNode->right;
        delNode->right->parent = delNode->parent;
        checkNode = delNode->right;
    }
    else 
    {
        if (isLeftFromParent(delNode))
            delNode->parent->left = delNode->left;
        else 
            delNode->parent->right = delNode->left;
        delNode->left->parent = delNode->parent;
        checkNode = delNode->left;
    }

    if (delNode->color == RED) //nút xóa là đỏ -> hoàn thành xóa
        delete delNode;
    else //nút xóa là đen -> xuất hiện dấu hiệu đen
    {
        if (delNode == pRoot)
        {
            pRoot = checkNode; //cập nhật lại root khi xóa root
            pRoot->color = BLACK; //root luôn có màu đen
        }
        delete delNode;
        delFixUp(pRoot, checkNode); //lúc này checkNode đang giữ dấu hiệu đen
    }
}

RBNode* lookUp(RBNode* pRoot, int key)
{
    RBNode* node = pRoot;
    while (node != NIL)
    {
        if (key < node->key)
            node = node->left;
        else if (key > node->key)
            node = node->right;
        else 
            break;
    }
    return node;
}

int Height(RBNode* node)
{
    if (node == NIL)
        return 0;
    
    return std::max(Height(node->left), Height(node->right)) + 1;
}

int BlackHeight(RBNode* node) // từ một node đến nút NIL  
                              // vì tất cả các đường đi đều có chiều cao đen bằng nhau -> chỉ cần đi đường trái nhất
{
    if (node == NIL)
        return 1;
    
    return BlackHeight(node->left) + (node->color == BLACK);
}
