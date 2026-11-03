vector<int> comp = v;
sort(comp.begin(), comp.end());
comp.erase(unique(comp.begin(), comp.end()), comp.end());
for(auto &x : v){
	x = lower_bound(comp.begin(), comp.end(), x) - comp.begin();
}

vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);
sort(idx.begin(), idx.end(),
	[&](int i, int j) { return a[i] < a[j]; });

vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);
sort(idx.begin(), idx.end(),
	[&](int i, int j) { return a[i] > a[j]; });
