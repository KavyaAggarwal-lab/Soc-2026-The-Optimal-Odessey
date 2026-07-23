#include <bits/stdc++.h>
using namespace std;
void dfs(int s, bool visited[], vector<int> adj[]) {
    if (visited[s]) return;
    visited[s] = true;
    for (auto u: adj[s]) {
        if(u==s) continue;
        dfs(u , visited , adj);
    }
}

int main() {
	int n,m; cin>>n>>m;
	vector<int> adj[n];
	bool visited[n];
	for(int i=0;i<n;i++) visited[i]=false;
	for(int i=0;i<m;i++){
	    int x,y;
	    cin>>x>>y;
	    adj[x-1].push_back(y-1);
	    adj[y-1].push_back(x-1);
	}
	//check components
	int c=0;
	vector<int> s ; //for storing a node for each component
	for(int i=0;i<n ;i++){
	   if(!visited[i]){
	       dfs(i,visited,adj);
	        s.push_back(i+1);
	        c++;}
	}
	cout<<c-1<<endl;
	for(int i=0;i<c-1;i++){
	    cout<<s[i]<<" "<<s[i+1]<<endl;	}
}
