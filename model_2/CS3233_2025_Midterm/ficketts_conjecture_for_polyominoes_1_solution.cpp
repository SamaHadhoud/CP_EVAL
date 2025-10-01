#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

static inline void addTent(vector<int64>& slope, int baseShift, int deltaL, int deltaR, int m) {
    if (m <= 0 || deltaL > deltaR) return;
    int L = deltaL + baseShift;
    int R = deltaR + baseShift;
    // slope is defined on indices [0..lenDr] inclusive (lenDr+1 slots)
    slope[L] += 1;
    slope[L + m] -= 1;
    slope[R - m + 1] -= 1;
    slope[R + 1] += 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int R, C;
        cin >> R >> C;
        vector<string> g(R);
        for (int i = 0; i < R; ++i) cin >> g[i];

        // Row intervals [Lr, Rr], Column intervals [Tc, Bc]
        vector<int> Lr(R, C), Rr(R, -1);
        vector<int> Tc(C, R), Bc(C, -1);
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                if (g[r][c] == '#') {
                    if (Lr[r] == C) Lr[r] = c;
                    Rr[r] = c;
                    if (Tc[c] == R) Tc[c] = r;
                    Bc[c] = r;
                }
            }
        }

        // Erosion by 3x3: eroded row intervals [eLr, eRr], eroded column intervals [eTc, eBc]
        vector<int> eLr(R, 1), eRr(R, 0); // default empty (length <= 0)
        vector<int> eTc(C, 1), eBc(C, 0); // default empty (length <= 0)

        for (int r = 1; r + 1 < R; ++r) {
            int Lmax = max(Lr[r - 1], max(Lr[r], Lr[r + 1]));
            int Rmin = min(Rr[r - 1], min(Rr[r], Rr[r + 1]));
            int l = Lmax + 1;
            int rr = Rmin - 1;
            eLr[r] = l;
            eRr[r] = rr;
        }
        for (int c = 1; c + 1 < C; ++c) {
            int Tmax = max(Tc[c - 1], max(Tc[c], Tc[c + 1]));
            int Bmin = min(Bc[c - 1], min(Bc[c], Bc[c + 1]));
            int t = Tmax + 1;
            int b = Bmin - 1;
            eTc[c] = t;
            eBc[c] = b;
        }

        // Base answer from 0°/180° is exactly 1
        int64 bestNum = 1, bestDen = 1;

        // Only 90° matters (270° is symmetric under sweeping the full shift range)
        int minDc = -(R - 1);
        int maxDc = (C - 1);
        int minDr = -(C - 1);
        int maxDr = (R - 1);
        int lenDr = maxDr - minDr + 1; // = R + C - 1
        int baseShift = -minDr;        // shift Δr to index: idx = Δr + baseShift

        vector<int64> slopeN(lenDr + 1), slopeNE(lenDr + 1), slopeNE1(lenDr + 1);

        for (int dc = minDc; dc <= maxDc; ++dc) {
            // Clear slope arrays
            fill(slopeN.begin(), slopeN.end(), 0);
            fill(slopeNE.begin(), slopeNE.end(), 0);
            fill(slopeNE1.begin(), slopeNE1.end(), 0);

            // For this Δc, valid pairs (j, k) with k = j + Δc
            int jStart = max(0, -dc);
            int jEnd = min(R - 1, C - 1 - dc);
            for (int j = jStart; j <= jEnd; ++j) {
                int k = j + dc;

                // Skip if this row is empty
                if (Lr[j] > Rr[j]) continue;

                int bLen = Rr[j] - Lr[j] + 1;
                int bL0 = (C - 1) - Rr[j];
                int bR0 = (C - 1) - Lr[j];

                // Base N: columns [Tc[k], Bc[k]] vs row j's shifted
                if (Tc[k] <= Bc[k]) {
                    int aL = Tc[k], aR = Bc[k];
                    int aLen = aR - aL + 1;
                    int m = min(aLen, bLen);
                    int deltaL = aL - bR0; // Tc + Lj - (C-1)
                    int deltaR = aR - bL0; // Bc + Rj - (C-1)
                    addTent(slopeN, baseShift, deltaL, deltaR, m);
                }

                // NE: eroded columns [eTc[k], eBc[k]] vs row j's shifted
                if (eTc[k] <= eBc[k]) {
                    int aL = eTc[k], aR = eBc[k];
                    int aLen = aR - aL + 1;
                    int m = min(aLen, bLen);
                    int deltaL = aL - bR0; // eTc + Lj - (C-1)
                    int deltaR = aR - bL0; // eBc + Rj - (C-1)
                    addTent(slopeNE, baseShift, deltaL, deltaR, m);
                }

                // N_E1: columns [Tc[k], Bc[k]] vs eroded row [eLr[j], eRr[j]] shifted
                if (Tc[k] <= Bc[k] && eLr[j] <= eRr[j]) {
                    int aL = Tc[k], aR = Bc[k];
                    int aLen = aR - aL + 1;
                    int beLen = eRr[j] - eLr[j] + 1;
                    int m = min(aLen, beLen);
                    int beL0 = (C - 1) - eRr[j];
                    int beR0 = (C - 1) - eLr[j];
                    int deltaL = aL - beR0; // Tc + eLj - (C-1)
                    int deltaR = aR - beL0; // Bc + eRj - (C-1)
                    addTent(slopeNE1, baseShift, deltaL, deltaR, m);
                }
            }

            // Sweep Δr and update answer
            int64 sN = 0, sNEv = 0, sNE1v = 0;
            int64 valN = 0, valNEv = 0, valNE1v = 0;
            for (int idx = 0; idx < lenDr; ++idx) {
                sN += slopeN[idx];
                sNEv += slopeNE[idx];
                sNE1v += slopeNE1[idx];

                valN += sN;
                valNEv += sNEv;
                valNE1v += sNE1v;

                int64 num = valN - valNEv;   // |(A\E) ∩ A1|
                int64 den = valN - valNE1v;  // |(A1\E1) ∩ A|
                if (den > 0) {
                    // Maximize num/den
                    if (num * bestDen > bestNum * den) {
                        bestNum = num;
                        bestDen = den;
                    }
                }
            }
        }

        int64 g = std::gcd(bestNum, bestDen);
        bestNum /= g;
        bestDen /= g;
        cout << bestNum << '/' << bestDen << '\n';
    }

    return 0;
}