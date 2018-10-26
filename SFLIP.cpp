#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vvi = vector<vi>;

const int INF = 2e9+5;
const ll LLINF = 2e18 + 5;

/*
    filippos
    SPOJ
    Task : SFLIP
    Solution based on greedy + mss segment tree
    We store both minimum and maximum subarray sum in a segment tree
    and use this information to greedily pick the best possible intervals.
    Picking an interval causes it to flip (that's done with lazy propagation).
    We can prove that flipping k overlapping intervals can always
    be reduce to flipping k disjoint intervals.
*/

template <typename T>
T get_mini(T a, T b){
	return min(a, b);
}
template <typename T>
T get_maxi(T a, T b){
	return max(a, b);
}

struct mss_tree{
	struct interval{
		ll val = 0;
		int l = -1, r = -1;
		interval(ll val, int l, int r) : val(val), l(l), r(r){}
		interval(){}
		bool operator< (const interval &other) const {
			return val < other.val;
		}
		bool operator> (const interval &other) const {
			return val > other.val;
		}
		interval operator+(interval other){
			return interval(val + other.val, l != -1 ? l : other.l, other.r != -1 ? other.r : r);
		}
		interval operator-() const{
			return interval(-val, l, r);
		}
	};
	// nodes are very heavy, there are probably better solutions
	struct info{
		interval pref, suff, all, mss;
		info(ll base, int pos){
			pref = suff = all = mss = interval(base, pos, pos);
		}
		info(interval pref, interval suff, interval all, interval mss) :
			pref(pref), suff(suff), all(all), mss(mss){}

		template <typename Func>
		info merge(info &other, Func f){
			return info(
				f(pref, all + other.pref),
				f(suff + other.all, other.suff),
				all + other.all,
				f(suff + other.pref, f(mss, other.mss)));
		}
		info operator-() const{
			return info(-pref, -suff, -all, -mss);
		}
		info(){}
	};
	struct node{
		bool lazy = 0;
		info mini, maxi;
		node *l = 0, *r = 0;
		node(vi::iterator a, vi::iterator b, int pos = 0){
			int size = b - a;
			if(size == 1){
				info base(*a, pos);
				mini = mini.merge(base, get_mini<interval>);
				maxi = maxi.merge(base, get_maxi<interval>);
			} else{
				auto m = a + size/2;
				l = new node(a, m, pos);
				r = new node(m, b, pos + size/2);
				mini = l->mini.merge(r->mini, get_mini<interval>);
				maxi = l->maxi.merge(r->maxi, get_maxi<interval>);
			}
		}
		void propagate(){
			if(!lazy) return;
			flip();
			lazy = 0;
		}
		void flip(){
			swap(mini, maxi);
			mini = -mini;
			maxi = -maxi;
			if(l) l->lazy ^= 1, r->lazy ^= 1;
		}
		void range_flip(int a, int b){
			// query/updates -> check lazy
			propagate();
			// get range info
			int from = mini.all.l, to = mini.all.r;
			// cout << "range_flip: " << a << ' ' << b << " @ " << from << ", " << to << '\n';
			// fully contained
			if(from >= a && to <= b){
				flip();
				return;
			}
			// not contained
			if(from > b || to < a){
				return;
			}
			// partial overlap
			l->range_flip(a, b);
			r->range_flip(a, b);
			mini = l->mini.merge(r->mini, get_mini<interval>);
			maxi = l->maxi.merge(r->maxi, get_maxi<interval>);
		}
	};

	node * root;
	mss_tree(vi &v){
		root = new node(v.begin(), v.end());
	}

	ll get_next(){
		interval best = root->maxi.mss;
		ll ans = best.val;
		// flip back the tree
		root->range_flip(best.l, best.r);
		return best.val;
	}

	void print(node *x){
		if(x->l) print(x->l);
		cout << x->maxi.mss.val << '\n';
		if(x->r) print(x->r);
	}

};

int main(){
    cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);

    int n, k; cin >> n >> k;
    vi v(n);
    for(int &i : v) cin >> i;
	// count starting value
    ll ans = accumulate(v.begin(), v.end(), 0LL);
    // we negate the array, because we consider the value of flipping\
    	instead of the original value
    transform(v.begin(), v.end(), v.begin(), [](int n){return -n;});

    mss_tree mt(v);
    // when you flip, you get two times the value
    while(k--)
    	ans += 2*mt.get_next();
    cout << ans;

    return cout << '\n', 0;
}