template <class T>
struct weighted_dsu {
public:
	weighted_dsu() : _n(0) {}
	explicit weighted_dsu(int n) : _n(n), parent_or_size(n, -1), diff_weight(n, T(0)) {}

	// weight[b] - weight[a] = w となるように a と b をマージする
	int merge(int a, int b, T w) {
		assert(0 <= a && a < _n);
		assert(0 <= b && b < _n);
		w += weight(a);
		w -= weight(b);
		int x = leader(a), y = leader(b);
		if (x == y) return x;
		if (-parent_or_size[x] < -parent_or_size[y]) {
			std::swap(x, y);
			w = -w;
		}
		parent_or_size[x] += parent_or_size[y];
		parent_or_size[y] = x;
		diff_weight[y] = w;
		return x;
	}

	// a と b が同じ連結成分に属するかを判定
	bool same(int a, int b) {
		assert(0 <= a && a < _n);
		assert(0 <= b && b < _n);
		return leader(a) == leader(b);
	}

	// 属する連結成分のリーダー（根）を返す
	int leader(int a) {
		assert(0 <= a && a < _n);
		if (parent_or_size[a] < 0) return a;
		int r = leader(parent_or_size[a]);
		diff_weight[a] += diff_weight[parent_or_size[a]]; // 経路圧縮時に重みを更新
		return parent_or_size[a] = r;
	}

	// a が属する連結成分のサイズを返す
	int size(int a) {
		assert(0 <= a && a < _n);
		return -parent_or_size[leader(a)];
	}

	// weight[b] - weight[a] を返す（事前に same(a, b) が true であることを確認すること）
	T diff(int a, int b) {
		assert(0 <= a && a < _n);
		assert(0 <= b && b < _n);
		return weight(b) - weight(a);
	}

	// 連結成分ごとに頂点リストをまとめた二次元配列を返す
	std::vector<std::vector<int>> groups() {
		std::vector<int> leader_buf(_n), group_size(_n);
		for (int i = 0; i < _n; i++) {
			leader_buf[i] = leader(i);
			group_size[leader_buf[i]]++;
		}
		std::vector<std::vector<int>> result(_n);
		for (int i = 0; i < _n; i++) {
			result[i].reserve(group_size[i]);
		}
		for (int i = 0; i < _n; i++) {
			result[leader_buf[i]].push_back(i);
		}
		result.erase(
			std::remove_if(result.begin(), result.end(),
				[&](const std::vector<int>& v) { return v.empty(); }),
			result.end());
		return result;
	}

private:
	int _n;
	std::vector<int> parent_or_size;
	std::vector<T> diff_weight;

	// 根からの重み（ポテンシャル）を計算
	T weight(int a) {
		leader(a); // 経路圧縮を確実に実行
		return diff_weight[a];
	}
};

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template<typename T>
using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;
// 使い方:
// ordered_set<T> st;
// 基本setと同じだがemplaceは不可
// st.order_of_key(x);    : x 未満の要素の個数を O(log N) で取得 (lower_bound のインデックスに相当)
// st.find_by_order(k);   : k 番目 (0-indexed) の要素のイテレータを O(log N) で取得
