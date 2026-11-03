vector<mint> a(n + 1, 0);
vector<mint> powv(n + 1);
ll b = 30;
powv[0] = 1;
for (int i = 1; i <= n; i++) {
	powv[i] = powv[i - 1] * b;
}
mint res = 0;
for (int i = 1; i <= n; i++) {
	res = (res * b + (s[i - 1] - 'a') + 1);
	a[i] = res;
}
auto get = [&](vector<mint>& x, int l, int r) -> mint {
	return x[r] - x[l] * powv[r - l];
};

struct Aho {
    using MP = unordered_map<char, int>;
    vector<MP> to;
    vector<int> cnt, fail;
    Aho() : to(1), cnt(1) {}
    
    int add(const string& s) {
        int v = 0;
        for (char c : s) {
            if (!to[v].count(c)) {
                to[v][c] = to.size();
                to.push_back(MP());
                cnt.push_back(0);
            }
            v = to[v][c];
        }
        cnt[v]++;
        return v;
    }
    
    void init() {
        fail = vector<int>(to.size(), -1);
        queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto [c, u] : to[v]) {
                fail[u] = (*this)(fail[v], c);
                cnt[u] += cnt[fail[u]];
                q.push(u);
            }
        }
    }
    
    int operator()(int v, char c) const {
        while (v != -1) {
            auto it = to[v].find(c);
            if (it != to[v].end()) return it->second;
            v = fail[v];
        }
        return 0;
    }
    
    int operator[](int v) const { return cnt[v]; }
};

// === 使い方メモ ===
// Aho aho;
// 1. パターンの登録: aho.add("pattern");
// 2. 構造の構築:    aho.init();
// 3. テキストの走査 (例: 含まれるパターンの総出現回数をカウント):
//    long long ans = 0;
//    int curr = 0; // ルートノードから開始
//    for (char c : T) {
//        curr = aho(curr, c); // 次の状態へ遷移
//        ans += aho[curr];    // その地点でマッチしている個数を加算
//    }
