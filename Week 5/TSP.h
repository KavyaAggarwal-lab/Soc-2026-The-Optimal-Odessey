#include<bits/stdc++.h>
using namespace std;
using json=nlohmann::ordered_json;
const double INF = numeric_limits<double>::infinity();

class Graph{
    public:
        int n;
        vector<vector<double>> adj;
        Graph(json graph_json){
            n=graph_json["nodes"].size();
            adj=vector<vector<double>>(n,vector<double>(n,INF));
            for(int i=0;i<n;i++){
                adj[i][i]=0;
            }
            for(auto i:graph_json["edges"]){
                int u=i["u"],v=i["v"];
                double w=i["w"];
                adj[u][v]=w;
                adj[v][u]=w;
}}};

vector<vector<double>> floyd_warshall(vector<vector<double>>& adj,vector<int>& nodes){
    int n=adj.size();
    for(int k=0;k<n;k++){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                adj[i][j]=min(adj[i][j],adj[i][k]+adj[k][j]);
    }}}
    vector<vector<double>> submatrix(nodes.size(),vector<double>(nodes.size()));
    for(int i=0;i<submatrix.size();i++){
        for(int j=0;j<submatrix.size();j++){
            submatrix[i][j]=adj[nodes[i]][nodes[j]];
    }}
    return submatrix;
}

vector<pair<int,int>> prim_mst(vector<vector<double>>& dist){
    int n = dist.size();
    vector<double> min_dist(n,INF);
    vector<int> parent(n,-1);
    vector<bool> used(n,false);
    min_dist[0]=0;
    vector<pair<int,int>> mst;
    for(int i=0;i<n;i++){
        int u=-1;
        for(int j=0;j<n;j++){
            if(!used[j]&&(u==-1||min_dist[j]<min_dist[u])){
                u=j;
            }
        }
        if(u==-1||min_dist[u]==INF){
            break;
        }
        used[u]=true;
        if(parent[u]!=-1){
            mst.push_back({parent[u],u});
        }
        for(int v=0;v<n;v++){
            if(!used[v]&&dist[u][v]<min_dist[v]){
                min_dist[v]=dist[u][v];
                parent[v]=u;
            }
        }
    }
    return mst;
}

vector<pair<int,int>> perfect_matching(vector<int>& odds,vector<vector<double>>& dist){
    vector<pair<int,int>> v;
    priority_queue<tuple<double,int,int>> q;
    int n=odds.size();
    vector<bool> done(n,false);
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            q.push({-dist[odds[i]][odds[j]],i,j});
    }}
    int count=(n/2);
    while(count){
        auto [a,b,c]=q.top();
        q.pop();
        if(done[b]||done[c]){
            continue;
        }
        done[b]=true;
        done[c]=true;
        count--;
        v.push_back({odds[b],odds[c]});
    }
    return v;
}

vector<int> eulerian_circuit(int m,vector<vector<pair<int,int>>>& adj_list){
    int edgecount = 0;
    for(auto edges:adj_list){
        for(auto [vertex,edgeid]:edges){
            edgecount=max(edgecount,edgeid+1);
        }
    }
    vector<bool> used(edgecount,false);
    vector<int> col;
    vector<int> path;
    col.push_back(0);
    while(!col.empty()){
        int present=col.back();
        bool found=false;
        for(auto [next,edgeid]:adj_list[present]){
            if(!used[edgeid]){
                used[edgeid]=true;
                col.push_back(next);
                found=true;
                break;
            }
        }
        if(!found){
            path.push_back(present);
            col.pop_back();
        }
    }
    reverse(path.begin(),path.end());
    return path;
}

double tour_cost(const vector<int>& tour,vector<vector<double>>& dist){
    double sum=0;
    for(int i=0;i<tour.size()-1;i++){
        sum+=dist[tour[i]][tour[i+1]];
    }
    sum+=dist[tour[tour.size()-1]][tour[0]];
    return sum;
}


vector<int> christofides(vector<vector<double>>& adj,vector<int>& nodes){
    vector<vector<double>> adj1=floyd_warshall(adj,nodes);
    vector<pair<int,int>> v=prim_mst(adj1);
    vector<int> degree(nodes.size(),0);
    int edge_count=0;
    vector<vector<pair<int,int>>> adj_list(nodes.size());
    for(auto k:v){
        degree[k.first]++;
        degree[k.second]++;
        adj_list[k.first].push_back({k.second,edge_count});
        adj_list[k.second].push_back({k.first,edge_count});
        edge_count++;
    }
    vector<int> odds;
    for(int i=0;i<nodes.size();i++){
        if(degree[i]%2){
            odds.push_back(i);
    }}
    vector<pair<int,int>> v1=perfect_matching(odds,adj1);
    for(auto k:v1){
        adj_list[k.first].push_back({k.second,edge_count});
        adj_list[k.second].push_back({k.first,edge_count});
        edge_count++;
    }
    vector<int> tour=eulerian_circuit(nodes.size(),adj_list);
    for(int i=0;i<tour.size()-1;i++){
        if(tour[i]==-1){
            continue;
        }
        for(int j=i+1;j<tour.size();j++){
            if(tour[j]==tour[i]){
                tour[j]=-1;
    }}}
    vector<int> tour_final;
    for(int i=0;i<tour.size();i++){
        if(tour[i]!=-1){
            tour_final.push_back(nodes[tour[i]]);
    }}
    return tour_final;
}
