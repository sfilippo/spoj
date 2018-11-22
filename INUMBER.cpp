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
    Task : INUMBER
    Determine the smallest multiple of some integer n,
    such that the sum of it's digits is also n
    Implemented with persistent_stack for faster backtracking
    Solved using BFS on remainders/digits
*/

template <typename T>
struct persistent_stack{
	persistent_stack *prev = 0;
	T val;
	persistent_stack(T val, persistent_stack *prev = 0) : val(val), prev(prev){}
	vector<T> build_whole(){
		vector<T> ans;
		persistent_stack *tmp = this;
		while(tmp){
			ans.push_back(tmp->val);
			tmp = tmp->prev;
		}
		return move(ans);
	}
};

struct state{
	int rem;
	int sum;
	persistent_stack<char> *num;
	state(int rem, int sum, persistent_stack<char> *num = 0) : rem(rem), sum(sum), num(num){}
};


int main(){
    cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);

    int t; cin >> t;
    while(t--){
    	int n; cin >> n;

    	queue<state> q;
    	q.push(state(0, 0));

    	vector<vector<bool>> vis(n, vector<bool>(n+1));
    	vis[0][0] = 1;

    	while(!q.empty()){
    		state curr = q.front(); q.pop();
    		// if we found the number
    		if(curr.rem == 0 && curr.sum == n) {
    			auto ans = curr.num->build_whole();
    			reverse(ans.begin(), ans.end());
    			for(char c : ans)
    				cout << c;
    			cout << '\n';
    			break;
    		}
    		// try all digits
    		for(int digit = 0; digit <= 9; digit++){
    			// avoid leading zeroes
    			if(digit == 0 && !curr.num) continue;
    			int next_rem = (curr.rem * 10 + digit) % n;
    			int next_sum = curr.sum + digit;
    			// avoid too big sums
    			if(next_sum > n) break;
    			// avoid visiting same states twice
    			if(vis[next_rem][next_sum]) continue;
    			vis[next_rem][next_sum] = 1;
    			q.push(state(next_rem, next_sum, new persistent_stack<char>(digit + '0', curr.num)));
    		}
    	}

    }
    
    return cout << '\n', 0;
}