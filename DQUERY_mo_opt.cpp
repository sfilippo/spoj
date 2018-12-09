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
    Offline solution using Mo's algorithm aka sqrt decomp on queries
	Includes optimization using Hilbert Curves,
		should bring complexity to O(NsqrtQ)
*/

// taken from: https://codeforces.com/blog/entry/61203
const int rotateDelta[] = {3, 0, 0, 1};
ll hilbertOrder(int x, int y, int pow, int rot = 0){
	if(pow == 0) return 0;
	int hpow = 1 << (pow-1);
	int seg = (x < hpow) ? 3 * (y >= hpow) : 1 + (y >= hpow);
	seg = (seg + rot) & 3;
	int nx = x & (x ^ hpow);
	int ny = y & (y ^ hpow);
	int nrot = (rot + rotateDelta[seg]) & 3;
	ll subSquareSize = 1LL << (2 * pow - 2);
	ll ans = seg * subSquareSize;
	ll add = hilbertOrder(nx, ny, pow-1, nrot);
	ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
	return ans;
}

int main(){
    cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);
    
    int n; cin >> n;
    vi v(n);
    for(int &i : v) cin >> i;

    // compress v
    vi z(v);
	sort(z.begin(), z.end());
	z.reserve(unique(z.begin(), z.end()) - z.begin());
	for(int &i : v) i = lower_bound(z.begin(), z.end(), i) - z.begin();


    int q; cin >> q;
    vector<pii> qq(q);
    for(pii &p : qq) {
    	cin >> p.first >> p.second;
    	p.first--, p.second--;
    }

    // to avoid sorting queries
    vi indexes(q); iota(indexes.begin(), indexes.end(), 0);
    // hilbert curve value
    const int max_k = 20; // 2^k >= n
    vll h(q);
    for(int i = 0; i < q; i++)
    	h[i] = hilbertOrder(qq[i].first, qq[i].second, max_k);
    sort(indexes.begin(), indexes.end(), [&](int i, int j){
    	return h[i] < h[j];
    });

    // to store queries' answers
    vi ans(q);
    
    // mo's algo
    int curr = 0;
    vi freq(z.size());
    int l = 0, r = -1;

    for(int i : indexes){
    	pii &q = qq[i];
    	// remove l: move right
    	for(int i = l; i < q.first; i++)
    		curr -= (--freq[v[i]]) == 0;
    	// remove r: move left
    	for(int i = r; i > q.second;  i--)
    		curr -= (--freq[v[i]]) == 0;
    	// add l: move left
    	for(int i = l-1; i >= q.first; i--)
    		curr += (++freq[v[i]]) == 1;
    	// add r : move right
    	for(int i = r + 1; i <= q.second; i++)
    		curr += (++freq[v[i]]) == 1;
    	// current answer
    	ans[i] = curr;
    	// update l, r
    	l = q.first, r = q.second;
    }

    // print solution
    for(int i : ans) cout << i << '\n';

    return cout << endl, 0;
}