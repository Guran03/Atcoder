priority_queue<P, vector<P>, greater<P>> pq;
vector<ll> d(n, INF);
d[0] = 0;
pq.emplace(d[0], 0);
while (!pq.empty()) {
	auto [cur, v] = pq.top();
	pq.pop();
	if (d[v] != cur) continue;
	for (auto [nx, ct] : g[v]) {
		if (chmin(d[nx], d[v] + ct)) pq.emplace(d[nx], nx);
	}
}
