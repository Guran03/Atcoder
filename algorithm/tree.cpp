template<class T = long long>
struct CartesianTree {
    int n, root;
    vector<int> l, r;
    CartesianTree() {}
    CartesianTree(const vector<T>& a, bool _max = true) {
        n = a.size();
        l = r = vector<int>(n, -1);
        vector<int> st;
        rep(i, n) {
            int p = -1;
            while (st.size() && !((a[st.back()] < a[i]) ^ _max)) {
                int j = st.back(); st.pop_back();
                r[j] = p; p = j;
            }
            l[i] = p;
            st.push_back(i);
        }
        rep(i, st.size() - 1) r[st[i]] = st[i + 1];
        root = st[0];
    }
};
// 親が子よりも大きい木
// CartesianTree<int> t(A);
// 親が子よりも「小さい」木
// CartesianTree<int> t(A, false);
// t.root:木の根となる要素のindex
// t.l[v]:index=vの要素の左の子のindex 子がないときは-1
// t.r[v]:index=vの要素の右の子のindex 子がないときは-1

template<typename T = int> // T: type of cost
struct lca {
    int n, root, l;
    vector<vector<int>> to;
    vector<vector<T>> co;
    vector<int> dep;
    vector<T> costs;
    vector<vector<int>> par;
    lca(int n) :n(n), to(n), co(n), dep(n), costs(n) {
        l = 0;
        while ((1 << l) < n) ++l;
        par = vector<vector<int>>(n + 1, vector<int>(l, n));
    }
    void addEdge(int a, int b, T c = 0) {
        to[a].push_back(b); co[a].push_back(c);
        to[b].push_back(a); co[b].push_back(c);
    }
    void dfs(int v, int d = 0, T c = 0, int p = -1) {
        if (p != -1) par[v][0] = p;
        dep[v] = d;
        costs[v] = c;
        for (int i = 0; i < (int)to[v].size(); ++i) {
            int u = to[v][i];
            if (u == p) continue;
            dfs(u, d + 1, c + co[v][i], v);
        }
    }

    void init(int _root = 0) {
        root = _root;
        dfs(root);
        for (int i = 0; i < l - 1; ++i) {
            for (int v = 0; v < n; ++v) {
                par[v][i + 1] = par[par[v][i]][i];
            }
        }
    }
    // LCA
    int up(int v, int k) {
        for (int i = l - 1; i >= 0; --i) {
            int len = 1 << i;
            if (k >= len) k -= len, v = par[v][i];
        }
        return v;
    }
    int operator()(int a, int b) {
        if (dep[a] > dep[b]) swap(a, b);
        b = up(b, dep[b] - dep[a]);
        if (a == b) return a;
        for (int i = l - 1; i >= 0; --i) {
            int na = par[a][i], nb = par[b][i];
            if (na != nb) a = na, b = nb;
        }
        return par[a][0];
    }
    int length(int a, int b) {
        int c = (*this)(a, b);
        return dep[a] + dep[b] - dep[c] * 2;
    }
    T dist(int a, int b) {
        int c = (*this)(a, b);
        return costs[a] + costs[b] - costs[c] * 2;
    }
};
/*
lca<> tree(n);
tree.addEdge(u, v, c);
tree.init(r);

// LCA
int lca_node = tree(a, b);
// 2点間の辺の数
int len = tree.length(a, b);
// 2点間の重み合計
long long d = tree.dist(a, b);
// k個上の祖先
int ancestor = tree.up(v, k);
*/

struct Rerooting {
    struct DP {
        // TODO: 必要な状態を定義（例: int val;）
        DP() {
            // TODO: 初期値を設定（葉のデフォルト値）
        }
        
        // 兄弟ノード（部分木同士）の情報をマージする演算子
        DP operator+(const DP& a) const {
            DP res;
            // TODO: マージ処理（例: res.val = max(this->val, a.val);）
            return res;
        }
        
        // 親ノードへ状態を渡すための変換関数（辺を1本通る処理）
        DP addRoot() const {
            DP res;
            // TODO: 根の加算や変化（例: res.val = this->val + 1;）
            return res;
        }
    };
    
    int n;
    std::vector<std::vector<int>> to;
    std::vector<std::vector<DP>> dp;
    std::vector<DP> ans;
    
    Rerooting(int n = 0) : n(n), to(n), dp(n), ans(n) {}
    
    void addEdge(int a, int b) {
        to[a].push_back(b);
        to[b].push_back(a);
    }
    
    void init() {
        dfs(0);
        bfs(0);
    }
    
    DP dfs(int v, int p = -1) {
        DP dpSum;
        dp[v] = std::vector<DP>(to[v].size());
        for (int i = 0; i < (int)to[v].size(); i++) {
            int u = to[v][i];
            if (u == p) continue;
            dp[v][i] = dfs(u, v);
            dpSum = dpSum + dp[v][i];
        }
        return dpSum.addRoot();
    }
    
    void bfs(int v, const DP& dpP = DP(), int p = -1) {
        int deg = to[v].size();
        for (int i = 0; i < deg; i++) {
            if (to[v][i] == p) dp[v][i] = dpP;
        }
        
        // 左右からの累積和（累積マージ）
        std::vector<DP> dpSumL(deg + 1);
        for (int i = 0; i < deg; i++) {
            dpSumL[i + 1] = dpSumL[i] + dp[v][i];
        }
        
        std::vector<DP> dpSumR(deg + 1);
        for (int i = deg - 1; i >= 0; i--) {
            dpSumR[i] = dpSumR[i + 1] + dp[v][i];
        }
        
        // 頂点 v を根としたときの答えを確定
        ans[v] = dpSumL[deg].addRoot();
        
        // 子ノードへ親・兄弟の情報を配りながら遷移
        for (int i = 0; i < deg; i++) {
            int u = to[v][i];
            if (u == p) continue;
            DP d = dpSumL[i] + dpSumR[i + 1];
            bfs(u, d.addRoot(), v);
        }
    }
};
// 使い方:
// Rerooting g(N);
// g.addEdge(u, v);
// g.init();
// 答えは g.ans[i] に格納される

vector<int> tree_centers(const vector<vector<int>>& g) {
    int n = g.size();
    vector<int> deg(n), leaves;
    for (int v = 0; v < n; v++) {
        deg[v] = g[v].size();
        if (deg[v] <= 1) leaves.push_back(v);
    }
    int remaining = n;
    while (remaining > 2) {
        remaining -= leaves.size();
        vector<int> next;
        // 葉を同時に1層削り、新しく葉になる頂点を集める。
        for (int v : leaves) {
            for (int u : g[v]) {
                if (--deg[u] == 1) next.push_back(u);
            }
        }
        leaves = move(next);
    }
    return leaves;
}
/*
auto cent = tree_centers(g); // 中心となる1頂点または隣接する2頂点の番号を取得
*/

vector<int> tree_diameter(const vector<vector<int>>& g) {
    auto bfs = [&](int s) {
        vector<int> par(g.size(), -1);
        queue<int> q;
        q.push(s);
        int t = s;
        while (!q.empty()) {
            t = q.front();
            q.pop();
            for (int u : g[t]) {
                if (u == par[t]) continue;
                par[u] = t;
                q.push(u);
            }
        }
        // BFSで最後に取り出した頂点が、始点からの最遠点。
        return pair{t, move(par)};
    };
    int s = bfs(0).first;
    auto [t, par] = bfs(s);
    vector<int> path;
    for (int v = t; v != -1; v = par[v]) path.push_back(v);
    return path;
}
/*
auto path = tree_diameter(g); // 直径の経路をvector配列で取得
*/
