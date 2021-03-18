#include<iostream>
#include<vector>
#include<math.h>
using namespace std;


// BKDR 最简单直接的字符串哈希匹配算法
class StringHashMatch {
private:

    // static code block=========================

    static const int BASE = 1331;

    static unsigned long long hash(const string& str, int L, int R) {
        int len = str.size();
        if (str.size() == 0) return 0;

        unsigned long long hashCode = 0;

        for (int i = L; i <= R; ++i) {
            hashCode *= BASE;
            hashCode += str[i];
        }

        return hashCode;
    }

    // o(p)
    static bool checkCollision(const string& source, const string& pattern, int src_i) {
        for (int ptn_i = 0; ptn_i < pattern.size(); ++ptn_i, ++src_i) {
            if (source[src_i] != pattern[ptn_i]) return true;
        }
        return false;
    }

    // =========================static code block

    vector<unsigned long long> preHash;
    vector<unsigned long long> preFact;
    int size;
    

    // member====================================
    void initialize(const string& str) {
        int len = str.size();
        this->size = len;
        if (len == 0) return;

        preHash.reserve(len);
        preFact.reserve(len);

        unsigned long long hashCode = 0;
        unsigned long long fact = 1;

        for (int i = 0; i < len; ++i) {
            hashCode *= BASE;
            hashCode += str[i];

            preHash.push_back(hashCode);
            preFact.push_back(fact);
            
            fact *= BASE;
        }
    }

    // ====================================member

public:

    StringHashMatch(const string& str) {
        initialize(str);
    }

    // [L, R]
    unsigned long long queryHash(int L, int R) const {
        if (L == 0) return preHash[R];
        // " 1 2 3 4 " 
        //     " 3 4 " 
        //   0 1 2 3
        //       L R
        // [0, L] :   12
        // [0, R] : 1234
        return preHash[R] - preHash[L-1] * preFact[R-L+1];
    }
    unsigned long long hashCode() const {
        return this->preHash[size-1];
    }

    int find(const string& pattern) {
        
        int pattern_size = pattern.size();
        auto pattern_hashCode = hash(pattern, 0, pattern_size-1);

        if (pattern_size > size) return -1;
        if (pattern_size == 0) return 0;

        for (int i = 0; i < size-pattern_size+1; ++i) {
            if (queryHash(i, i+pattern_size-1) == pattern_hashCode) return i;
        }       

        return -1;
    }


    // 使用该方法，你需要提供原始source字符串, 并且牺牲一点至少o(pattern)的时间复杂度，具体看冲突次数
    int find_exactly(const string& source, const string& pattern) {
        int pattern_size = pattern.size();
        auto pattern_hashCode = hash(pattern, 0, pattern_size-1);

        if (pattern_size > size) return -1;
        if (pattern_size == 0) return 0;

        for (int i = 0; i < size-pattern_size+1; ++i) {
            if (queryHash(i, i+pattern_size-1) == pattern_hashCode) {

                bool isCollision = checkCollision(source, pattern, i);
                if (!isCollision) return i;
            }
        }       

        return -1;
    }
};

// 轻量匹配
class StringHashMatchGo {
private:

    static const int BASE = 1331;
    
    static unsigned long long hash(const string& source, int L, int R) {
        int len = source.size();
        if (source.size() == 0) return 0;

        unsigned long long hashCode = 0;

        for (int i = L; i <= R; ++i) {
            hashCode *= BASE;
            hashCode += source[i];
        }

        return hashCode;
    }

    // o(p)
    static bool checkCollision(const string& source, const string& pattern, int src_i) {
        for (int ptn_i = 0; ptn_i < pattern.size(); ++ptn_i, ++src_i) {
            if (source[src_i] != pattern[ptn_i]) return true;
        }
        return false;
    }

    // o(p)
    static unsigned long long getFact(int size) {
        unsigned long long fact = 1;

        for (int i = 0; i < size-1; ++i) {
            fact *= BASE;
        }

        return fact;
    }


    int ans;

public:

    StringHashMatchGo() {}

    StringHashMatchGo(const string& source, const string& pattern) {
        go(source, pattern);
    }

    void go(const string& source, const string& pattern) {
        ans = -1;
        int len_src = source.size();
        int len_ptn = pattern.size();

        if (len_ptn == 0) {
            ans = 0;
            return;
        }

        if (len_src < len_ptn) {
            return;
        }

        auto hashCode_ptn = hash(pattern, 0, len_ptn-1);
        auto hashCode_src = hash(source,  0, len_ptn-1);
        if (hashCode_ptn == hashCode_src) {
            bool isCollision = checkCollision(source, pattern, 0);
            if (!isCollision) {
                ans = 0;
                return;
            }
        }

        unsigned long long fact = getFact(len_ptn);

        for (int i = len_ptn; i < len_src; ++i) {
            hashCode_src -= fact * source[i-len_ptn];
            hashCode_src *= BASE;
            hashCode_src += source[i];

            if (hashCode_ptn == hashCode_src) {
                bool isCollision = checkCollision(source, pattern, i-len_ptn+1);
                if (!isCollision) {
                    ans = i-len_ptn+1;
                    return;
                }
            }
        }
    }

    int result() { return ans; }

};



int main () {

    string str1 = "hello";
    string str2 = "ll";

    StringHashMatchGo shm(str1, str2);

    
   
    system("pause");
}
