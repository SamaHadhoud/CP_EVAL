#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const int BUFSIZE = 1 << 20;
    int idx, size;
    char buf[BUFSIZE];
    FastScanner() : idx(0), size(0) {}
    inline char getch() {
        if (idx >= size) {
            size = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }
    template <typename T>
    bool readInt(T &out) {
        char c = getch();
        if (!c) return false;
        while (c <= ' ') {
            c = getch();
            if (!c) return false;
        }
        bool neg = false;
        if (c == '-') {
            neg = true;
            c = getch();
        }
        long long val = 0;
        while (c > ' ') {
            val = val * 10 + (c - '0');
            c = getch();
        }
        out = neg ? -val : val;
        return true;
    }
};

struct FastOutput {
    static const int BUFSIZE = 1 << 20;
    int idx;
    char buf[BUFSIZE];
    FastOutput() : idx(0) {}
    ~FastOutput() { flush(); }
    inline void pushChar(char c) {
        if (idx >= BUFSIZE) flush();
        buf[idx++] = c;
    }
    inline void writeUInt64(uint64_t x) {
        char s[32];
        int n = 0;
        if (x == 0) s[n++] = '0';
        else {
            while (x) {
                s[n++] = char('0' + (x % 10));
                x /= 10;
            }
        }
        while (n--) pushChar(s[n]);
        pushChar('\n');
    }
    inline void flush() {
        if (idx) {
            fwrite(buf, 1, idx, stdout);
            idx = 0;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FastScanner fs;
    FastOutput fo;

    int T;
    unsigned long long M;
    if (!fs.readInt(T)) return 0;
    if (!fs.readInt(M)) return 0;

    vector<int> Ns(T);
    int maxN = 0;
    for (int i = 0; i < T; ++i) {
        int n;
        fs.readInt(n);
        Ns[i] = n;
        if (n > maxN) maxN = n;
    }

    vector<uint32_t> fact(maxN + 1);
    vector<uint32_t> inv(maxN + 1);

    // Precompute factorials modulo M
    fact[0] = 1u;
    for (int i = 1; i <= maxN; ++i) {
        fact[i] = (uint32_t)(((unsigned long long)fact[i - 1] * (unsigned long long)i) % M);
    }

    // Precompute involutions modulo M: I(0)=1, I(1)=1, I(n)=I(n-1)+(n-1)*I(n-2)
    inv[0] = 1u;
    if (maxN >= 1) inv[1] = 1u;
    for (int i = 2; i <= maxN; ++i) {
        unsigned long long term = ((unsigned long long)(i - 1) * (unsigned long long)inv[i - 2]) % M;
        unsigned long long s = inv[i - 1] + term;
        if (s >= M) s -= M;
        inv[i] = (uint32_t)s;
    }

    for (int i = 0; i < T; ++i) {
        int n = Ns[i];
        uint64_t a = fact[n];
        uint64_t b = inv[n];
        uint64_t ans = (a >= b) ? (a - b) : (a + M - b);
        fo.writeUInt64(ans);
    }
    fo.flush();
    return 0;
}