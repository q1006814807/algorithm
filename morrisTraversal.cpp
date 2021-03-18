#include<iostream>
#include<vector>
using namespace std;


class TreeNode {
public:

    int key;
    TreeNode* left, * right;
    TreeNode() : key(0), left(nullptr), right(nullptr) {}
    TreeNode(int val) : key(val), left(nullptr), right(nullptr) {}

};

// Morris遍历，空间复杂度为1的树遍历算法
class MorrisTraversal {
private:

    static TreeNode* reverse(TreeNode* head) {
        if (!head) return nullptr;

        TreeNode sentry;
        auto ptr = &sentry;

        while (head) {
            auto next = head->right;

            auto sentryNext = ptr->right;
            ptr->right = head;
            ptr->right->right = sentryNext;

            head = next;
        }

        return sentry.right;
    }

    static void postorderVisit(TreeNode* node) {
        reverse(node);

        auto cur = node;
        while (cur) {
            // ===========打印操作============

            // ===========打印操作============
            cur = cur->right;
        }
        
        reverse(node);
    }

public:

    static void morrisStd(TreeNode* root) {
        if (!root) return;

        auto cur = root;
        auto leftMostRight = root;

        while (cur) {
            leftMostRight = cur->left;

            if (leftMostRight) {
                // 有左子树，检查一下左子树最右结点的情况
                while (leftMostRight->right && leftMostRight->right != cur) {
                    leftMostRight = leftMostRight->right;
                }        

                if (leftMostRight->right) {
                    // 表示从cur的左子树最右结点跳回了cur结点，即第二次到达cur

                    // 已经回来，就把之前修改的指针还原
                    leftMostRight->right = nullptr;
                    cur = cur->right;
                }
                else {
                    // 第一次到达cur
                    // 修改左子树最右结的右指针指向自己，留一条回来的路
                    leftMostRight->right = cur;
                    cur = cur->left;
                }

            }
            else {
                // 无左子树，直接向右移动
                cur = cur->right;
            }
        }


    }

    static void donePreorder(TreeNode* root) {
        if (!root) return;

        auto cur = root;
        auto leftMostRight = root;

        while (cur) {
            leftMostRight = cur->left;

            if (leftMostRight) {
                // 有左子树，检查一下左子树最右结点的情况
                while (leftMostRight->right && leftMostRight->right != cur) {
                    leftMostRight = leftMostRight->right;
                }        

                if (leftMostRight->right) {
                    // 表示从cur的左子树最右结点跳回了cur结点，即第二次到达cur

                    // 已经回来，就把之前修改的指针还原
                    leftMostRight->right = nullptr;
                    cur = cur->right;
                }
                else {
                    // 第一次到达cur
                    // ===========打印操作============

                    // ===========打印操作============

                    // 修改左子树最右结的右指针指向自己，留一条回来的路
                    leftMostRight->right = cur;
                    cur = cur->left;
                }

            }
            else {
                // ===========打印操作============

                // ===========打印操作============

                // 无左子树，直接向右移动
                cur = cur->right;
            }
        }


    }

    static void doneInorder(TreeNode* root) {
        if (!root) return;

        auto cur = root;
        auto leftMostRight = root;

        while (cur) {
            leftMostRight = cur->left;

            if (leftMostRight) {
                // 有左子树，检查一下左子树最右结点的情况
                while (leftMostRight->right && leftMostRight->right != cur) {
                    leftMostRight = leftMostRight->right;
                }        

                if (leftMostRight->right) {
                    // 表示从cur的左子树最右结点跳回了cur结点，即第二次到达cur
                    // ===========打印操作============
                    cout << cur->key << ", ";
                    // ===========打印操作============

                    // 已经回来，就把之前修改的指针还原
                    leftMostRight->right = nullptr;
                    cur = cur->right;
                }
                else {
                    // 第一次到达cur


                    // 修改左子树最右结的右指针指向自己，留一条回来的路
                    leftMostRight->right = cur;
                    cur = cur->left;
                }

            }
            else {
                // ===========打印操作============
                cout << cur->key << ", ";
                // ===========打印操作============

                // 无左子树，直接向右移动
                cur = cur->right;
            }
        }

    }

    static void donePostorder(TreeNode* root) {
        if (!root) return;

        auto cur = root;
        auto leftMostRight = root;

        while (cur) {
            leftMostRight = cur->left;

            if (leftMostRight) {
                // 有左子树，检查一下左子树最右结点的情况
                while (leftMostRight->right && leftMostRight->right != cur) {
                    leftMostRight = leftMostRight->right;
                }        

                if (leftMostRight->right) {
                    // 表示从cur的左子树最右结点跳回了cur结点，即第二次到达cur
                    // 已经回来，就把之前修改的指针还原
                    leftMostRight->right = nullptr;
                    postorderVisit(cur);

                    cur = cur->right;
                }
                else {
                    // 第一次到达cur
                    // 修改左子树最右结的右指针指向自己，留一条回来的路
                    leftMostRight->right = cur;
                    cur = cur->left;
                }

            }
            else {
                // 无左子树，直接向右移动
                cur = cur->right;
            }
        }

        // 单独处理右上角列
        postorderVisit(root);


    }



    static int doneMinHeight(TreeNode* head) {
        if (!head) return 0;

        int minHeight = INT32_MAX;
        int curHeight = 0;

        auto cur = head;
        auto leftMostRight = head;

        while (cur) {
            leftMostRight = cur->left;
            int leftHeight = 1;

            if (leftMostRight) {
                // 有左子树，检查一下左子树最右结点的情况
                while (leftMostRight->right && leftMostRight->right != cur) {
                    leftMostRight = leftMostRight->right;
                    ++leftHeight;
                }        

                if (leftMostRight->right) {
                    // 表示从cur的左子树最右结点跳回了cur结点，即第二次到达cur
                    if (leftMostRight->left == nullptr) {
                        minHeight = min(minHeight, curHeight);
                    }

                    // 已经回来，就把之前修改的指针还原
                    leftMostRight->right = nullptr;
                    curHeight -= leftHeight;
                    cur = cur->right;
                }
                else {
                    // 第一次到达cur
                    ++curHeight;
                    // 修改左子树最右结的右指针指向自己，留一条回来的路
                    leftMostRight->right = cur;
                    cur = cur->left;
                }

            }
            else {
                // 无左子树，直接向右移动
                ++curHeight;
                cur = cur->right;
            }
        }

        // 最右处理
        cur = head;
        curHeight = 1;
        while (cur->right) {
            cur = cur->right;
            ++curHeight;
        }

        if (cur->left == nullptr) {
            minHeight = min(minHeight, curHeight);
        }

        return minHeight;
    }

};


int main() {


    MorrisTraversal mt;

    TreeNode n5(5);
    TreeNode n1(1);
    TreeNode n4(4);
    TreeNode n3(3);
    TreeNode n6(6);
    n5.left = &n1;
    n5.right = &n4;
    n4.left = &n3;
    n4.right = &n6;
    

    mt.doneInorder(&n5);

    
    system("pause");


}