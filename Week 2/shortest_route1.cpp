#include <bits/stdc++.h>
using namespace std;
#define INF 1e14
int main() {
	int n, m; cin>>n>>m;
	long long dist[n];
	vector<pair<int,long long>> adj[n];
	priority_queue <pair<long long,int>> q;
	bool processed[n];
	while(m--){
	    int a,b;
	    long long c;
	    cin>>a>>b>>c;
	    adj[a-1].push_back({b-1,c});
	}
	for(int i=0;i<n;i++) {processed[i]=false; dist[i]= INF;}
	dist[0]=0;
	q.push({0,0});
	while(!q.empty()){
	    int a = q.top().second; q.pop();
	    if (processed[a]) continue;
	    processed[a] =true;
	    for(auto u: adj[a]){
	        int b=u.first ;
	        long long w=u.second;
	        if(dist[a]+w<dist[b]){
                dist[b]=dist[a]+w;
                q.push({-dist[b],b});
            }
	    }
	    
	}
	for(int i=0;i<n;i++){
	    cout<<dist[i]<<" ";
	}
	
}
