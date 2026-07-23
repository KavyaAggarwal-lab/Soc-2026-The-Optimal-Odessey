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
	// your code goes here
	int n , m ,c=0; cin>>n>>m;
	vector<int> adj[n];
	vector<int> comp;
	bool visited[n];
	for(int i=0;i<n;i++) visited[i]=false;
	for(int i=0;i<m;i++){
	    int x,y; cin>>x>>y;
	    adj[x-1].push_back(y-1);
	    adj[y-1].push_back(x-1);
	}
	for(int i=0;i<n;i++) {
	    if(visited[i]== false){dfs(i,visited,adj);
	    c++;
	    comp.push_back(i);}}
	//make an array to q1, q2 to continue pop and fill adj. nodes in each other;
    queue<int> q1, q2;
    int imp=0, arr[n];
    for(int i=0;i<n;i++) arr[i]=0;
	for(auto i : comp){
	    q1.push(i);
	    while(!q1.empty() or !q2.empty()){
    	    while(!q1.empty()){
    	        if(arr[q1.front()]==2) {imp=1;cout<<"IMPOSSIBLE"; break;}
    	        arr[q1.front()]=1;
    	        for( auto x:adj[q1.front()]){ 
    	            if(arr[x]==0){arr[x]=2; q2.push(x);}
        	        if(arr[x]==1){imp=1;cout<<"IMPOSSIBLE"; break;}
    	        }
    	        if(imp==1 )break;
    	        q1.pop();}
    	   if(imp==1 )break;
    	    while(!q2.empty()){
    	        if(arr[q2.front()]==1){imp=1;cout<<"IMPOSSIBLE"; break;}
        	    arr[q2.front()]=2;
        	    for( auto x:adj[q2.front()]){
        	        if(arr[x]==0){arr[x]=1; q1.push(x);}
        	        if(arr[x]==2){imp=1;cout<<"IMPOSSIBLE"; break;}
        	    }
                q2.pop();
    	        if(imp==1 )break;
    	    }
            if(imp==1 )break;
	    }
	    if(imp==1 )break;
	}
	if(imp==0){
	    for(int i=0;i<n;i++){
	        cout<<arr[i]<<" ";
	    }
	}
}
