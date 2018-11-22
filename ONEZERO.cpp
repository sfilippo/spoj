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
    Task : ONEZERO
    Determine the smallest multiple of some integer n,
    using only digits 0 and 1.
    Solved using BFS on remainders/digits
*/

struct state{
	int rem;
	string num;
	state(int rem, string num) : rem(rem), num(num){}
};


int main(){
    cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);

    int t; cin >> t;
    while(t--){
    	int n; cin >> n;

    	queue<state> q;
    	q.push(state(1 % n, "1"));

    	vector<bool> vis(n);
    	vis[1 % n] = 1;

    	while(!q.empty()){
    		state curr = q.front(); q.pop();
    		// if we found the number
    		if(curr.rem == 0) {
    			cout << curr.num << '\n';
    			break;
    		}
    		// add 0, then 1
    		for(int digit = 0; digit <= 1; digit++){
    			int next_rem = (curr.rem * 10 + digit) % n;
    			if(vis[next_rem]) continue;
    			vis[next_rem] = 1;
    			q.push(state(next_rem, curr.num + to_string(digit)));
    		}
    	}

    }
    
    return cout << '\n', 0;
}