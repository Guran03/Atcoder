vector<bool> Eratosthenes(int N) {
	vector<bool> isprime(N + 1, true);
	isprime[0] = isprime[1] = false;
	for (int p = 2; p <= N; ++p) {
		if (!isprime[p]) continue;
		for (int q = p * 2; q <= N; q += p) {
			isprime[q] = false;
		}
	}
	return isprime;
}

ll power(ll a, ll b, ll m) {
	ll p = a, ans = 1;
	for (int i = 0; i < 30; i++) {
		int w = 1 << i;
		if (b & (1 << i)) ans = (ans * p) % m;
		p = (p * p) % m;
	}
	return ans;
}

ll division(ll a, ll b, ll m) {
	return (a * power(b, m - 2, m)) % m;
}

// AtCoder の modint を使う
// MAXはnCkのnのmax
const int MAX = 510000;
mint fac[MAX], finv[MAX], inv[MAX];

// テーブルを作る前処理
void COMinit() {
    const int MOD = mint::mod();
    fac[0] = fac[1] = 1;
    finv[0] = finv[1] = 1;
    inv[1] = 1;
    for (int i = 2; i < MAX; i++) {
        fac[i]  = fac[i - 1] * i;
        inv[i]  = MOD - inv[MOD % i] * (MOD / i);
        finv[i] = finv[i - 1] * inv[i];
    }
}

// 二項係数計算
mint COM(int n, int k) {
    if (n < k)       return 0;
    if (n < 0 || k < 0) return 0;
    return fac[n] * finv[k] * finv[n - k];
}

// main関数の中にCOMinit();を呼び出す
// 計算例
// COM(100000, 50000).val()

mint comb(long long n, long long k) {
	if (n < k || k < 0) return 0;
	if (k == 0 || n == k) return 1;
	k = std::min(k, n - k);
	mint num = 1;
	mint den = 1;
	for (long long i = 0; i < k; i++) {
		num *= (n - i);
		den *= (i + 1);
	}
	return num / den;
}

// 多倍長テンプレ
/* ---------------------- ここから ---------------------- */
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
// 任意長整数型
using Bint = mp::cpp_int;
// 仮数部が10進数で1024桁の浮動小数点数型(TLEしたら小さくする)
using Real = mp::number<mp::cpp_dec_float<1024>>;
// ついでに有理数型
using Rat = boost::rational<Bint>;
/* ---------------------- ここまで ---------------------- */
