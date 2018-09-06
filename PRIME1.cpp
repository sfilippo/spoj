#include <bits/stdc++.h>

using namespace std;

using vi = vector<int>;

const int INF = 2e9+5;

/*
	filippos
	SPOJ
	Task : PRIME1
	Tutorial-like task for segmented sieve
*/

template <int N>
struct segsieve{
	vector<bool> prime;
	vector<int> primes;
	segsieve(){
		prime.resize(sqrt(N) + 5, 1);
		primes.reserve(sqrt(N)*2);
		//special cases
		prime[0] = prime[1] = 0;
		//even numbers
		for(int i = 4; i < prime.size(); i += 2)
			prime[i] = 0;
		//odd numbers - O(NloglogN) as we go up to square root only
		for(int i = 3; i * i < prime.size(); i += 2)
			if(prime[i]) for(int j = i * i; j < prime.size(); j += i)
				prime[j] = 0;
		//add primes to list
		for(int i = 0; i < prime.size(); i++)
			if(prime[i])
				primes.push_back(i);
	}
	//return segmented sieve in O((r-l)*log(r-l) + #prime)
	vector<bool> get_range(int l, int r){
		vector<bool> ans(r-l+1, 1);
		if(l == 0) ans[0] = 0;
		if(l <= 1 && r >= 1) ans[1 - l] = 0;
		for(int i : primes) {
			int k = max(i, (l + i - 1) / i);
			for(int j = i*k; j <= r; j += i){
				// if(j < l) continue;
				int x = j - l;
				ans[x] = 0;
			}
		}
		return ans;
	}
};

const int MAXN = 1e9 + 5;

int main(){
	cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);
	
	//We build a sieve for numbers in [0, sqrt(MAXN)]
	//And later use it to get the primes in the required ranges
	segsieve<MAXN> sieve;

	int t; cin >> t;
	while(t--){
		int l, r; cin >> l >> r;
		auto prime = sieve.get_range(l, r);
		for(int i = 0; i < prime.size(); i++)
			if(prime[i])
				cout << i + l << '\n';
		cout << '\n';
	}
	return 0;
}