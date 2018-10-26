#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vvi = vector<vi>;

const int INF = 2e9+5;

/*
    filippos
    SPOJ
    Task : INCSEQ
	Solved using DP + fenwick tree, modifying standard fenwick tree approach
	for LIS by using solutions with length L to compute solutions of length L + 1
*/

constexpr int lsb(int n){
	return n&-n;
}

const int MOD = 5e6;

struct fenwick{
	vi fen;
	fenwick(int size) : fen(size+1){}
	int sum(int i){
		int ans = 0;
		for(++i; i; i -= lsb(i))
			ans += fen[i],
			ans %= MOD;
		return ans;
	}
	void add(int i, int k){
		for(++i; i < fen.size(); i+=lsb(i))
			fen[i] += k,
			fen[i] %= MOD;
	}
	void print(){
		for(int &i : fen)
			cout << i << ' ';
		cout << '\n';
	}
};

int main(){
    cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);
    int n, k; cin >> n >> k;
    vi v(n);
    for(int &i : v) cin >> i;
    //coordinate compress
    map<int,int> c;
	for(int &i : v)
		c[i] = 0;
	int count = 1;
	for(auto &x : c)
		x.second = count++;
	for(int &i : v)
		i = c[i];
	//build fenwicks
	vector<fenwick> fens(k+1, fenwick(n+1));
	fens[0].add(0, 1);
	//for each item, solve for each length
	for(int &i : v){
		for(int l = 1; l <= k; l++)
			fens[l].add(i, fens[l-1].sum(i-1));
	}
	cout << fens.back().sum(n);
    return cout << '\n', 0;
}