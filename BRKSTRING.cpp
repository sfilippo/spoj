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
	O(N²) solution using Knuth's optimization
*/

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
		vi sum(n);
		partial_sum(v.begin(), v.end(), sum.begin());

		vector<vector<ll>> dp(n, vector<ll>(n));
		// smallest index of optimal solution for dp[l, r]
		vvi opt(n, vi(n));
		for(int s = 1; s <= n; s++){
			for(int l = 0; l + s - 1 < n; l++){
				int r = l + s - 1;
				// base cases
				if(s <= 2){
					dp[l][r] = s == 2 ? sum[r] - (l ? sum[l-1] : 0) : 0;
					opt[l][r] = l;
				} else{
					int low = opt[l][r-1];
					int hi = opt[l+1][r];
					int current_sum = sum[r] - (l ? sum[l-1] : 0);
					dp[l][r] = LLINF;
					// solution must be in this range as\
						opt[l-1][r] <= opt[l][r] <= opt[l][r+1] 
					for(int m = low; m <= hi; m++){
						ll tmp = current_sum + dp[l][m] + dp[m+1][r];
						if(tmp < dp[l][r]){
							dp[l][r] = tmp;
							opt[l][r] = m;
						}
					} // dp[l][r] is found
				}
				// cout << l << ", " << r << " = " << dp[l][r] << '\n';
			}
		}
		cout << dp[0][n-1] << '\n';

	}
}