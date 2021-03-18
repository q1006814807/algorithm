#include<iostream>
#include<vector>
#include<stack>
using namespace std;


// 递归式 o(n2)
class ExpressionCalculator {
private:

    static const int Sign_Plus     = INT32_MIN + 1;
    static const int Sign_Minus    = INT32_MIN + 2;
    static const int Sign_Multiply = INT32_MIN + 3;
    static const int Sign_Divide   = INT32_MIN + 4;

    int getSign(char c) {
        switch (c)
        {
        case '+': return Sign_Plus;
        case '-': return Sign_Minus;
        case '*': return Sign_Multiply;
        case '/': return Sign_Divide;
        }
        return 0;
    }

    int myStoi(string& s, int L, int R) {
        int num = 0;
        if (L > R) return num;
        int sign = 1;
        if (s[L] == '-') sign = -1;

        while (L <= R) {
            if (isdigit(s[L])) {
                num *= 10;
                num += s[L] - '0';
            }
            ++L;
        }
        return sign * num;
    }

    // 删除空格符
    void trim(string& s) {
        int left = 0;
        int right = 0;
        while (right < s.size()) {
            while (right < s.size() && s[right] == ' ') {
                ++right;
            }
            if (right < s.size()) {
                s[left] = s[right];
                ++left;
                ++right;
            }
        }
        s.resize(left);
    }

    // 返回外围无效括号对数量
    int invalidBrackets(string& s, int L, int R) {
        int lptr = L;
        while (lptr < R && s[lptr] == '(') ++lptr;
        if (lptr == L) return 0;

        int rptr = R;
        while (rptr > L && s[rptr] == ')') --rptr;
        if (rptr == R) return 0;

        // ((((1) + (2) - (1 + 1))))
        // ((9 - 1)) - (1 + 2)
        int lBackupBrackets  = lptr - L;
        int rBackupBrackets = R - rptr;
        int curLBrackets = 0;

        for (; lptr <= rptr; ++lptr) {
            char c = s[lptr];
            if      (c == '(') ++curLBrackets;
            else if (c == ')') {
                if (curLBrackets > 0) --curLBrackets;
                else {
                    --lBackupBrackets;
                    if (lBackupBrackets == 0) return 0;
                }
            }
        }

        rBackupBrackets -= curLBrackets;

        return min(lBackupBrackets, rBackupBrackets);
    }

    bool startWith(string& s, int L, int R, char c1, char c2) {
        if (L+1 > R) return false;
        return s[L] == c1 && s[L+1] == c2;
    }


public:

    int calculate(string& s) {
        trim(s);    
        return execute(s, 0, s.size()-1);
    }   

    int execute(string& s, int L, int R) {
        if (L > R) return 0;
        // 1. 去除外层括号
        int invalids = invalidBrackets(s, L, R);
        L += invalids;
        R -= invalids;

        // 2.找到中间+-连接符号

        vector<int>  nums;
        vector<char> sign;
      
        int brackets = 0;
        int mid_mulAndDiv = -1;
        int end = R + 1;

        for (int i = R; i >= L; --i) {
            char c = s[i];
            if      (c == ')') ++brackets;
            else if (c == '(') --brackets;
            else if (brackets == 0) {
                if      ((c == '*' || c == '/') && mid_mulAndDiv == -1) mid_mulAndDiv = i;
                else if ((c == '+' || c == '-') && (i > L && (s[i-1] == ')' || isdigit(s[i-1])))) {
                    nums.push_back(execute(s, i+1, end-1));
                    sign.push_back(c);
                    end = i;
                }
            }
        }
        if (nums.empty()) {
            // 2*(1+2), 1/2*3, (1+2)*3, (1+2)*(3+4), -(1+2)*2;
            if (mid_mulAndDiv == -1) {
                if (startWith(s, L, R, '+', '(') || startWith(s, L, R, '-', '(')) {
                    return (s[L] == '+' ? 1 : -1) * execute(s, L+2, R-1);
                }
                else return myStoi(s, L, R);
            }

            if (s[mid_mulAndDiv] == '*') return execute(s, L, mid_mulAndDiv-1) * execute(s, mid_mulAndDiv+1, R);
            else return execute(s, L, mid_mulAndDiv-1) / execute(s, mid_mulAndDiv+1, R);
        }
        else nums.push_back(execute(s, L, end-1));

        // 1+3-2
        // nums : 2 3 1
        // sign : - +

        while (sign.size()) {
            int num1 = nums.back();
            nums.pop_back();
            int num2 = nums.back();
            nums.pop_back();

            if (sign.back() == '+') nums.push_back(num1 + num2);
            else nums.push_back(num1 - num2);

            sign.pop_back();
        }

        return nums.back();
    }   

};

// 递归-中缀转后缀RPN通用解法 o(n2)
class ExpressionCalculator2 {
private:
    enum SIGN { 
        PLUS  = INT32_MIN + 3,
        MINUS,
        MULTIPLY,
        DIVIDE 
    };

    int getSign(char c) {
        switch (c)
        {
        case '+': return SIGN::PLUS;
        case '-': return SIGN::MINUS;
        case '*': return SIGN::MULTIPLY;
        case '/': return SIGN::DIVIDE;
        }
        return 0;
    }

    int myStoi(string& s, int L, int R) {
        int num = 0;
        if (L > R) return num;
        int sign = 1;
        if (s[L] == '-') sign = -1;

        while (L <= R) {
            if (isdigit(s[L])) {
                num *= 10;
                num += s[L] - '0';
            }
            ++L;
        }
        return sign * num;
    }

    bool startWith(string& s, int L, int R, char c1, char c2) {
        if (L+1 > R) return false;
        return s[L] == c1 && s[L+1] == c2;
    }

public:

    int calculate(string& s) {
        trim(s);    

        vector<int> RPN;
        convertToRPN(s, 0, s.size()-1, RPN);

        // for (auto e : RPN) {
        //     cout << e << endl;
        // }

        return evalRPN(RPN);
    }   


    void trim(string& s) {
        int left = 0;
        int right = 0;
        while (right < s.size()) {
            while (right < s.size() && s[right] == ' ') {
                ++right;
            }
            if (right < s.size()) {
                s[left] = s[right];
                ++left;
                ++right;
            }
        }
        s.resize(left);
    }

    // (((1 + (2) - (1 + 1))))
    // (9 - 1) - (1 + 2)

    // 3 - (-1 - (-6/3 + 3)) * 2 + 1
    void convertToRPN(string& s, int L, int R, vector<int>& RPNStorage) {
        // o(n)
        // 去除外围多余的括号
        int invalids = invalidBrackets(s, L, R);
        L += invalids;
        R -= invalids;

        // o(n)
        // 找到中间符号
        auto&& [mid, type] = findMedianSign(s, L, R);
        
        if (mid == -1) {
            if (!startWith(s, L, R, '+', '(') && !startWith(s, L, R, '-', '(')) {
                // -a, +a, a
                RPNStorage.push_back(myStoi(s, L, R));
            }
            else {
                // -(a) = 0-a = 0 a -, +(a) = 0+a = 0 a +
                RPNStorage.push_back(0);
                convertToRPN(s, L+2, R-1, RPNStorage);
                RPNStorage.push_back(getSign(s[L]));
            }
            return;
        }
        
        convertToRPN(s, L, mid-1, RPNStorage);

        if (type) {
            // +-
            // 注意：+-包含前方符号
            convertToRPN(s, mid, R, RPNStorage);
            RPNStorage.push_back(SIGN::PLUS); // a-b = a+(-b)
        }
        else {
            // */
            // 注意：*/不包含前方符号
            convertToRPN(s, mid+1, R, RPNStorage);
            RPNStorage.push_back(getSign(s[mid])); // a*b = a b *
        }
    }
    
    // <mid, +-?>
    pair<int, bool> findMedianSign(string& s, int L, int R) {
        int bracket = 0;
        int mid1 = R;
        int mid2 = -1;

        for (; mid1 > L; --mid1) {
            char c = s[mid1];

            if      (c == ')') ++bracket;
            else if (c == '(') --bracket;
            else if (bracket == 0) {    
                if (c == '+' || c == '-') {
                    if (mid1 > L && (s[mid1-1] == ')' || isdigit(s[mid1-1])))
                        break;
                }
                else if ((c == '*' || c == '/') && mid2 == -1) {
                    mid2 = mid1;
                }
            }
        }
        if (mid1 <= L) return {mid2, false};
        return {mid1, true};
    }

    int invalidBrackets(string& s, int L, int R) {
        int lptr = L;
        while (lptr < R && s[lptr] == '(') ++lptr;
        if (lptr == L) return 0;

        int rptr = R;
        while (rptr > L && s[rptr] == ')') --rptr;
        if (rptr == R) return 0;

        // ((((1) + (2) - (1 + 1))))
        // ((9 - 1)) - (1 + 2)
        int lBackupBrackets  = lptr - L;
        int rBackupBrackets = R - rptr;
        int curLBrackets = 0;

        for (; lptr <= rptr; ++lptr) {
            char c = s[lptr];
            if      (c == '(') ++curLBrackets;
            else if (c == ')') {
                if (curLBrackets > 0) --curLBrackets;
                else {
                    --lBackupBrackets;
                    if (lBackupBrackets == 0) return 0;
                }
            }
        }

        rBackupBrackets -= curLBrackets;

        return min(lBackupBrackets, rBackupBrackets);
    }

    int evalRPN(vector<int>& RPN) {

        stack<int> stk;
        int N = RPN.size();

        for (int i = 0; i < N; ++i) {
            auto cur = RPN[i];
            switch (cur)
            {
            case SIGN::PLUS : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num1 + num2);
                    break;
                }
            case SIGN::MINUS : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num2 - num1);
                    break;
                }
            case SIGN::MULTIPLY : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num1 * num2);
                    break;
                }
            case SIGN::DIVIDE : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num2 / num1);
                    break;
                }
            default: {
                    stk.push(cur);
                }
            }

        }

        return stk.top();
    }

};

// 迭代栈 RPN o(n)
class ExpressionCalculator3 {
private:
    enum SIGN { 
        PLUS  = INT32_MIN + 3,
        MINUS,
        MULTIPLY,
        DIVIDE 
    };

    int getSign(char c) {
        switch (c)
        {
        case '+': return SIGN::PLUS;
        case '-': return SIGN::MINUS;
        case '*': return SIGN::MULTIPLY;
        case '/': return SIGN::DIVIDE;
        }
        return 0;
    }

    string parse(int num) {
        switch (num)
        {
        case SIGN::PLUS:        return "+";
        case SIGN::MINUS:       return "-";
        case SIGN::MULTIPLY:    return "*";
        case SIGN::DIVIDE:      return "/";
        } 

        return to_string(num);
    }

    void trim(string& s) {
        int left = 0;
        int right = 0;
        while (right < s.size()) {
            while (right < s.size() && s[right] == ' ') {
                ++right;
            }
            if (right < s.size()) {
                s[left] = s[right];
                ++left;
                ++right;
            }
        }
        s.resize(left);
    }


public:

    int calculate(string& s) {
        trim(s);

        vector<int> RPN;
        convertToRPN(s, RPN);

        for (auto e : RPN) {
            cout << parse(e) << endl;
        }

        return evalRPN(RPN);
    }   

    // 3 - (-1 - (-6/3 + 3)) * 2 + 1
    void convertToRPN(string& s, vector<int>& RPNStorage) {
        int len = s.size();
        stack<int> sign;

        int begin = 0;
        int preSign = 1;

        while (begin < len) {
            char c = s[begin];
            switch (c)
            {
                case '*': 
                case '/': {
                    int curSign = getSign(c);
                    while (sign.size() && (sign.top() >= SIGN::MULTIPLY && sign.top() != '(')) {
                        RPNStorage.push_back(sign.top());
                        sign.pop();
                    }   
                    sign.push(curSign);
                    break;
                }

                case '(': {
                    sign.push('(');
                    break;
                }
                case ')': {
                    while (sign.size() && sign.top() != '(') {
                        RPNStorage.push_back(sign.top());
                        sign.pop();
                    }
                    sign.pop();

                    break;
                }

                case '+':
                case '-': {

                    int curSign = getSign(c);
                    // -1-(2+1)
                    // ^ ^
                    if (begin == 0 || (s[begin-1] != ')' && !isdigit(s[begin-1]))) {
                        RPNStorage.push_back(0);
                        sign.push(getSign(c));
                        // (-1) -> 0-1, 此时负号拥有最高优先级
                        break;
                    }      

                    while (sign.size() && (sign.top() >= SIGN::PLUS && sign.top() != '(')) {
                        RPNStorage.push_back(sign.top());
                        sign.pop();
                    }   

                    sign.push(curSign);
                    break;
                }

                default: {
                    
                    int num = 0;

                    while (begin < len && isdigit(s[begin])) {
                        num *= 10;
                        num += s[begin] - '0';
                        ++begin;
                    } 

                    RPNStorage.push_back(num);

                    continue;
                }
            }

            ++begin;
        }

        while (sign.size()) {
            RPNStorage.push_back(sign.top());
            sign.pop();
        }

    }

    int evalRPN(vector<int>& RPN) {

        stack<int> stk;
        int N = RPN.size();

        for (int i = 0; i < N; ++i) {
            auto cur = RPN[i];
            switch (cur)
            {
            case SIGN::PLUS : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num1 + num2);
                    break;
                }
            case SIGN::MINUS : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num2 - num1);
                    break;
                }
            case SIGN::MULTIPLY : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num1 * num2);
                    break;
                }
            case SIGN::DIVIDE : {
                    int num1 = stk.top(); stk.pop();
                    int num2 = stk.top(); stk.pop();
                    stk.push(num2 / num1);
                    break;
                }
            default: {
                    stk.push(cur);
                }
            }

        }

        return stk.top();
    }

};



int main() {
    ExpressionCalculator3 EC;
    string expr = "1+2*5/3+6/4*2";
    // string expr = "2*(3+4)-5/2*3";

    auto res = EC.calculate(expr);

    cout << "+++" << endl;
    cout << res << endl;

    system("pause");
}