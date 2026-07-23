#include <bits/stdc++.h>
using namespace std;

int main() {
	int n,m; cin>>n>>m;
	vector<int> adj[n];
	queue<int> q;
	int distance[n];
	bool visited[n];
	distance[n-1]=0;
	for(int i=0;i<n;i++) visited[i]=false;
	
	for(int i=0;i<m;i++){
	    int x,y;
	    cin>>x>>y;
	    adj[x-1].push_back(y-1);
	    adj[y-1].push_back(x-1);
	}
	
	//through bfs store distance and then distance[n-1] and commonbtw adj[n] and whose distance is just less than distance[n-1] 
	visited[0]=true;
	distance[0]=0;
	q.push(0);
	while(!q.empty()){
	    int s =q.front(); q.pop();
	    for(auto u :adj[s]){
	        if(visited[u]) continue;
	        visited[u]=true;
	        distance[u] = distance[s]+1;
	        q.push(u);
	        if(u==n-1) break;
	    }
	}
	if(distance[n-1]==0) cout<<"IMPOSSIBLE";    
	else{cout<<distance[n-1]+1<<endl;
	    int p;
	    p=distance[n-1]; 
	    vector<int> path;
	    //common btw distance[x]=p-i and adj[path[i-1]] 
	    path.push_back(n-1);
	    for(int i=1;i<p+1;i++){
	        for(auto x : adj[path[i-1]]){
	            if(distance[x]==p-i) {
	                path.push_back(x);
	                break;}
	        }
	    }
	    cout<<"1 ";
	    for(int i=p-1;i>=0;i--)cout<<path[i]+1<<" ";
	
	}
	
	
}
