
#include<iostream>
#include<vector>
using namespace std;

class Manacher {
private:
    vector<int> radius;
    void initManacher(string& s) {
        if (s.empty()) return;

        // 1.expand
        string expanded; 
        expanded.reserve(s.size() * 2 + 3);

        expanded += "!#";
        for (char c : s) {
            expanded += c;
            expanded += '#';
        }
        expanded += '@';

        // 2.iterate
        int len = expanded.size();
        radius.resize(len, 1);

        int maxI = 0;

        for (int i = 1; i < len-1; ++ i) {
            // reuse radius
            int maxEnd = maxI + radius[maxI];
            if (i < maxEnd) {
                radius[i] = min(maxEnd-i, radius[2*maxI - i]);
            }
            
            // expand again
            int R = i + radius[i];
            int L = i - radius[i];
            while (expanded[L] == expanded[R]) {
                radius[i] += 1;
                --L, ++R;
            }

            // update maxI
            if (i + radius[i] > maxEnd) maxI = i;
        }
    }
    void initManacher(string& s1, string& s2) {
        if (s1.empty() && s2.empty()) return;

        // 1.expand
        string expanded; 
        expanded.reserve((s1.size() + s2.size()) * 2 + 3);

        expanded += "!#";
        for (char c : s1) {
            expanded += c;
            expanded += '#';
        }
        for (char c : s2) {
            expanded += c;
            expanded += '#';
        }
        expanded += '@';

        // 2.iterate
        int len = expanded.size();
        radius.resize(len, 1);

        int maxI = 0;

        for (int i = 1; i < len-1; ++ i) {
            // reuse radius
            int maxEnd = maxI + radius[maxI];
            if (i < maxEnd) {
                radius[i] = min(maxEnd-i, radius[2*maxI - i]);
            }
            
            // expand again
            int R = i + radius[i];
            int L = i - radius[i];
            while (expanded[L] == expanded[R]) {
                radius[i] += 1;
                --L, ++R;
            }

            // update maxI
            if (i + radius[i] > maxEnd) maxI = i;
        }
    }

public:
    Manacher(string& s) {
        initManacher(s);
    }
    Manacher(string& s1, string& s2) {
        initManacher(s1, s2);
    }
    
    bool isPalindrome(int L, int R) {
        if (L > R) return false;
        int mid = L + R + 2;
        return radius[mid] + mid - 1 >= 2 * R + 2;
    }
};



