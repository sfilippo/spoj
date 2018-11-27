#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vvi = vector<vi>;

const int INF = 2e9+5;
const ll LLINF = 1LL << 60;

/*
    filippos
    SPOJ
    Task : BRKSTRING
    O(N³) solution, coded only as reference before
    implementing the solution with Knuth's optimization
*/

ll dp(int l, int r, vi &sum, vector<vector<ll>> &memo){
	// base case
	if(l == r) return 0;
	// dp check
	ll &state = memo[l][r];
	if(state != -1) return state;
	// try all splittings
	state = LLINF;
	for(int m = l; m < r; m++){
		//splitting as [l, m][m+1, r]
		ll tmp = sum[r] - (l ? sum[l-1] : 0) + dp(l, m, sum, memo) + dp(m+1, r, sum, memo);
		state = min(state, tmp);
	}
	return state;
}

int main(){
    cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);
    // I don't like how this looks like
    int n, m;
    while(cin >> n >> m){
    	// we have another testcase
    	vi cuts(m);
    	for(int &i : cuts)
    		cin >> i;
    	// we build sizes
    	vi v(m+1);
    	for(int i = 0; i <= m; i++){
    		v[i] = (i == m ? n : cuts[i]) - (i ? cuts[i-1] : 0);
    	}
    	// for(int i : v) cout << i << ' '; cout << '\n';
    	n = v.size(); // no longer care about old n and i like the name
    	// prefix sums for fast computations later on
    	vi pre(n);
    	partial_sum(v.begin(), v.end(), pre.begin());

    	vector<vector<ll>> memo(n, vector<ll>(n, -1));

    	ll sol = dp(0, n-1, pre, memo);
    	cout << sol << '\n';
    }
}