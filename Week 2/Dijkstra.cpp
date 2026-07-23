#include <bits/stdc++.h>
const long long INF = 1e11;
using namespace std;


int main(){
    int n, m; cin>>n>>m;
    vector<pair<int,long long >> adj[n];
    while(m--){
        int a,b;
        long long w;
        cin>>a>>b>>w;
        adj[a-1].push_back({b-1,w});
        adj[b-1].push_back({a-1,w});
    }
    vector<long long> distance(n, INF);
    vector<bool> processed(n,false);
    priority_queue <pair<int,int>> q;
    vector<int> path;
    vector<int> arr(n,-1);
    
    for(int i=1;i<n;i++) distance[i]=INF;
    distance[0]=0;
    q.push({0,0});
    while(!q.empty()){
        if(processed[n-1]) break;
        int a=q.top().second; q.pop();
        if(processed[a])continue;
        processed[a]=true;
        for(auto u:adj[a]){
            int b=u.first ;
            long long w=u.second;
            if(distance[a]+w<distance[b]){
                distance[b]=distance[a]+w;
                arr[b]=a;
                q.push({-distance[b],b});
            }
        }
    }
    if(distance[n-1]==INF) cout<<"-1";
    else{
        int y=n-1;
        while(y!=0){
            path.push_back(y);
            y=arr[y];}
        reverse(path.begin(), path.end());
        cout<<"1 ";
        for(auto k: path) cout<<k+1<<" ";}
}