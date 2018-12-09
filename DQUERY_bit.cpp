#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int,int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vi>;
using vvl = vector<vll>;
using vb = vector<bool>;

const int INF = 2e9+5;
const ll LLINF = (1LL << 60) + 5;

/*
    filippos
    SPOJ
    Task : DQUERY
    Given an array of N integers, answer Q queries
    	"Count distinct numbers in [l, r]"
    Offline solution using BIT (Fenwick tree)
    Core idea is based on solvign queries on prefixes
*/


// Code below is a bit old (~ September 2017)

constexpr int lsb(int n){
	return n&(-n);
}
//da usare 0 based
struct fenwick{
	vector<int> v;

	fenwick(int n){
		v.resize(n+1, 0);
	}
	fenwick(vector<int> &source){
		v.resize(source.size() + 1, 0);
		for(int i = 0; i < source.size(); i++){
			v[i+1] += source[i];
			if(i+1 + lsb(i+1) < v.size())
				v[i+1 + lsb(i+1)] += v[i+1];
		}
	}
	int pf(int i){
		i++;
		int ans = 0;
		for(; i; i-=lsb(i))
			ans += v[i];
		return ans;
	}
	int rsq(int l, int r){
		return pf(r) - pf(l-1);
	}
	void update(int i, int k){
		i++;
		for(; i < v.size(); i += lsb(i))
			v[i] += k;
	}
	void print(){
		for(int i = 1; i < v.size(); i++)
			cout << v[i] << ' ';
		cout << endl;
	}
};

struct query{
	int l, r, i, ans = 0;
};

int N;

bool beh(query &a, query &b){
	return a.r > b.r;
}

bool beh2(query &a, query &b){
	return a.i < b.i;
}

void solve(vector<int> &v, vector<query> &q){
	N = v.size();
	sort(q.begin(), q.end(), beh);
	fenwick fw(N);
	vector<int> prev(N, -1);
	unordered_map<int, int> last;
	for(int i = 0; i < N; i++){
		if(last.count(v[i]))
			prev[i] = last[v[i]];
		if(prev[i] != -1)
			fw.update(prev[i], -1);

		fw.update(i, 1);
		last[v[i]] = i;
	}
	int r = N-1;
	for(auto &Q : q){
		while(r > Q.r){
			fw.update(r, -1);
			last[v[r]] = prev[r];
			if(prev[r] != -1)
				fw.update(prev[r], 1);
			r--;
		}
		Q.ans = fw.rsq(Q.l, Q.r);
	}

	sort(q.begin(), q.end(), beh2);
}

int main(){
	cin.tie(0), cin.sync_with_stdio(0);
	cin >> N;
	vector<int> v(N);
	for(int &i : v)
		cin >> i;
	
	int Q;
	cin >> Q;
	vector<query> q(Q);
	for(int i = 0; i < Q; i++){
		cin >> q[i].l >> q[i].r;
		// 1-based to 0-based
		q[i].l--, q[i].r--;
		q[i].i = i;
	}
	solve(v, q);
	for(auto &x : q){
		cout << /*x.l << ' ' << x.r << " -> " <<*/ x.ans << '\n';
	}
}