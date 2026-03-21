#include "diplom/Functions.h"
#include <quadmath.h>

int TestAdd(int a, int b) {
	return a + b;
}

__float128 GG0(__float128& ALPHA, __float128& GAMMA, __float128& P) {
    __float128 DM = P / ALPHA;
    __float128 TT = 1.0Q + GAMMA;
    __float128 TT2 = 0.5Q * TT;
    return sinq(TT * atanq(DM)) / powq((1 + DM * DM), TT2);
}

__float128 GG(int& L, __float128& ALPHA, __float128& GAMMA, __float128& P,
    __float128& GAM) {
    if (P == 0.0Q)
        return 0.0Q;

    __float128 GLA[8];

    GLA[0] = GG0(ALPHA, GAMMA, P); // GG0_orig

    if (L == 0)
        return GLA[0] * GAM; // gam and GG0_orig

    __float128 AP = ALPHA / P;
    __float128 TT = 1.0Q + GAMMA;
    __float128 GAMMA1 = GAMMA - 1.0Q;
    __float128 CN = TT * GG0(ALPHA, GAMMA1, P) / GAMMA - GLA[0]; // GG0_orig
    GLA[1] = CN * AP;
    __float128 frInv;
    for (int idx = 2; idx < L + 1; ++idx) { // DO I = 2 , L
        frInv = 1.0Q / (GAMMA - idx + 1);
        GLA[idx] = AP * (2 * idx - 1) * frInv * GLA[idx - 1] -
            (idx + GAMMA) * frInv * GLA[idx - 2];
    }
    return GLA[L] * GAM;
}

int Max(const int& a, const int& b) {
    return (a > b) ? a : b;
}

int Sum(int a, int b) {
    return a + b;
}