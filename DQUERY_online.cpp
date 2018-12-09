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
    Online solution using persistent segment trees
    Similar to offline-BIT-solution but with
    persistency to allow an online approach.
*/

// Rushed implementation, as old one got TLE

// Persistent segment tree for "Count distinct numbers" queries
// Data must be already compressed
struct ds{
	struct node;
	static vector<node> nodes;
	// recursive segment tree with indexes instead of pointers
	struct node{
		int val, sz;
		int left = -1, right = -1;
		node(int x, int size = 1) : val(x), sz(size){}
		int rsq(int l, int r){
			if(r - l + 1 == sz) return val;
			if(l > r) return 0;
			return nodes[left].rsq(l, min(sz/2 - 1, r)) + nodes[right].rsq(max(0, l - sz/2), r - sz/2);
		}
		void print(){
			cout << "[" << (this - &(nodes[0])) << "] " << val << " @ " << sz << " (" << left << ", " << right << ")\n";
			if(left != -1){
				nodes[left].print();
				nodes[right].print();
			}
		}
		node(){}
	};
	// pointers to segtree versions
	vi roots;
	// builds the whole structure
	ds (const vi &v){
		// nodes required to build main and new versions
		nodes.reserve(v.size() * 2 + v.size() * log2(v.size()*2) * 2);
		// allocate pointers for roots
		roots.reserve(v.size() + 1);
		// create main, empty tree
		roots.push_back(nodes.size());
		build(v.size());
		// we build a version for each prefix, in which
		// we count # of distinct in a suffix
		// v[i] = 1 if last[k] = i, for any k in [prefix]
		vi last(v.size(), -1);
		for(int i = 0; i < v.size(); i++){
			int x = v[i];
			// update last array
			int last_ver = roots.back();
			if(last[x] != -1){
				// turn off
				int tmp = nodes.size();
				update(last_ver, last[x], 0);
				last_ver = tmp;
			}
			// mark new node
			last[x] = i;
			// create new node
			int new_v = nodes.size();
			// add new version to roots
			roots.push_back(nodes.size());
			update(last_ver, i, 1);
			// // debug print
			// nodes[new_v].print();
			// cout << "\n";
		}
	};
	// builds empty tree of given size
	void build(int n){
		int i = nodes.size();
		nodes.push_back(node(0, n));
		if(n > 1){
			nodes[i].left = nodes.size();
			build(n/2);
			nodes[i].right = nodes.size();
			build(n - n/2);
		}
	}
	// recursive point set with persistency
	void update(int ref, int pos, int val){
		int i = nodes.size();
		// create copy of ref
		nodes.push_back(nodes[ref]);
		// do we need to recur?
		node &x = nodes[i];
		if(x.sz > 1){
			// check index
			if(pos < x.sz / 2) {
				x.left = nodes.size();
				update(nodes[ref].left, pos, val);
			} else {
				x.right = nodes.size();
				update(nodes[ref].right, pos - x.sz / 2, val);
			}
			// update value
			x.val = nodes[x.left].val + nodes[x.right].val;
		} else{
			// update leaf
			x.val = val;
		}
	}
	int count_dist(int a, int b){
		return nodes[roots[b+1]].rsq(a, b);
	}
};
vector<ds::node> ds::nodes; // c++ magic shit

int main(){
	cin.tie(0), cin.sync_with_stdio(0);
	int n; cin >> n;
	vi v(n);
	for(int &i : v)
		cin >> i;
	// compress v
	vi z(v); sort(z.begin(), z.end());
	z.resize(unique(z.begin(), z.end()) - z.begin());
	for(int &i : v) i = lower_bound(z.begin(), z.end(), i) - z.begin();

	ds st(v);
	int q; cin >> q;
	while(q--){
		int a, b, k;
		cin >> a >> b;
		a--, b--;
		cout << st.count_dist(a, b) << '\n';
	}
}