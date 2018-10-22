/*
	filippos
	SPOJ
	Task: FASTFLOW
	Testing Scaling Dinic algorithm for Max Flow 
*/

/*
	Dinic's Max Flow Algorithm ~ O(V²E)
	WARNING: In extreme cases, switch to scaling Dinic
*/

// common template
using namespace std;

#include <bits/stdc++.h>
using vi = vector<int>;
using vvi = vector<vi>;
const int INF = 2e9 + 5; // il maggico
using ll = long long;


// We represent the graph as an adiacency list of edge list indexes

struct edge{
	int from, to; // the connected nodes
	int f; // the flow we can still push
};

using graph = vvi;
using edge_list = vector<edge>;

// Also, we use i^1 to get back-edge of i-th edge

constexpr int back_edge(int i){ return i ^ 1;}

// Assumes directed edges
const bool IS_DIRECTED = 1;

void add_edge(graph &g, edge_list &e, int a, int b, int c, bool directed = IS_DIRECTED){
	// index of current edge
	int index = e.size();
	// updates adjiacency list
	g[a].push_back(index);
	g[b].push_back(back_edge(index));
	// create edges
	e.push_back({a, b, c});
	// for back edge, checks if directed or undirected edge
	e.push_back({b, a, directed ? 0 : c});
}

// recursive dfs that finds and adds a blocking flow
// following level graph, from source to sink,

int level_dfs(graph &g, edge_list &el, vi &level, vi &pointer, int source, int sink, int flow = INF){
	// if we can return flow
	if(source == sink) return flow;
	// we try each edge, from last attempted one
	for(int &idx = pointer[source]; idx < g[source].size(); idx++){
		int i = g[source][idx];
		edge &e = el[i];
		// follows level graph && possibility to send flow
		if(level[e.to] == level[source] + 1 && e.f > 0){
			int pushed = level_dfs(g, el, level, pointer, e.to, sink, min(flow, e.f));
			// if we could push flow
			if(pushed > 0){
				// update edge and back edge
				el[i].f -= pushed;
				el[back_edge(i)].f += pushed;
				// return to avoid more pushes
				return pushed;
			}
		}
	}
	// couldn't push flow
	return 0;
}

// bfs that builds the level graph and return 
// if it's possible to respect some given limits

bool level_bfs(graph &g, edge_list &el, vi &level, int source, int sink){
	// resets level graph
	fill(level.begin(), level.end(), INF);
	// source is level[0]
	level[source] = 0;
	// begin bfs from source
	queue<int> q;
	q.push(source);
	
	// graph over or reached sink
	while(!q.empty() && level[sink] == INF){
		// current node
		int curr = q.front(); q.pop();
		// try all edges
		for(int &i : g[curr]){
			// current edge
			edge &e = el[i];
			// if edge not empty and leads to unvisited
			if(e.f > 0 && level[e.to] == INF){
				// updates level and adds to queue
				level[e.to] = level[curr] + 1;
				q.push(e.to);
			}
		}
	}
	// return true if sink was reached
	return level[sink] != INF;
}

ll dinic(graph &g, edge_list &el, int source, int sink){
	// Scaling part: we try starting from big to small values
	// Vector used for level graph
	vi level(g.size());
	ll flow = 0;
	// while we can send flow
	while(level_bfs(g, el, level, source, sink)){
		// core optimization trick, for chained DFSs
		vi pointer(g.size());
		// we send all flow that's possible
		int tmp;
		while((tmp = level_dfs(g, el, level, pointer, source, sink))){
			flow += tmp;
		}
	}
	// returns the found flow
	return flow;
}


int main(){
    cin.tie(0), cin.sync_with_stdio(0), cout.tie(0), cout.sync_with_stdio(0);
    int n, m; cin >> n >> m;
    graph g(n);
    edge_list el;
    for(int i = 0; i < m; i++){
    	int a, b, c;
    	cin >> a >> b >> c;
    	a--, b--;
    	add_edge(g, el, a, b, c, 0);
    }

    cout << dinic(g, el, 0, n-1);
    return cout << '\n', 0;
}