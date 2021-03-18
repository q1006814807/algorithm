#include<iostream>
#include<vector>
#include<stack>
using namespace std;

class TreeNode {
public:

    int key;
    TreeNode* left, * right;
    TreeNode() : key(0), left(nullptr), right(nullptr) {}
    TreeNode(int val) : key(val), left(nullptr), right(nullptr) {}

};

class IterativeTraversal {
public:

    static void preorder(TreeNode* root) {
        if (!root) return;

        stack<TreeNode*> stk;

        stk.push(root);
        while (stk.size()) {
            auto cur = stk.top();
            stk.pop();
            

            cout << cur->key << endl;

            // 先进右
            if (cur->right) stk.push(cur->right);
            // 再进左
            if (cur->left)  stk.push(cur->left);

        }
    }

    static void inorder(TreeNode* root) {
        if (!root) return;

        stack<TreeNode*> stk;
        stk.push(root);
        auto cur = root->left;

        while (stk.size() || cur) {

            if (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            else {
                cur = stk.top();
                stk.pop();

                cout << cur->key << endl;
                
                cur = cur->right;
            }
        }   

    }


    static void postorder1(TreeNode* root) {
        if (!root) return;

        stack<TreeNode*> stk;
        stack<int> data;

        stk.push(root);
        while (stk.size()) {
            auto cur = stk.top();
            stk.pop();
            data.push(cur->key);

            // 先进左
            if (cur->left)  stk.push(cur->left);
            // 再进右
            if (cur->right) stk.push(cur->right);
        }

        while (data.size()) {

            cout << data.top() << endl;

            data.pop();
        }

    }

    static void postorder2(TreeNode* root) {
        if (!root) return;

        stack<TreeNode*> stk;
        stk.push(root);

        TreeNode* cur = nullptr;
        TreeNode* pre = root;

        while (stk.size()) {
            cur = stk.top();

            if (cur->left && cur->left != pre && cur->right != pre) {
                stk.push(cur->left);
            }
            else if (cur->right && cur->right != pre) {
                stk.push(cur->right);
            }
            else {
                stk.pop();

                cout << cur->key << endl;

                pre = cur;
            }

        }

    }


};