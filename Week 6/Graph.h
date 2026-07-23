#include<bits/stdc++.h>
using namespace std;
using json=nlohmann::ordered_json;
#define INF 10000000

class Graph{
        public:
                vector<vector<double>> adj;
                int n;
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
                        }
                        for(int i=0;i<n;i++){
                                for(int j=0;j<n;j++){
                                        for(int k=0;k<n;k++){
                                                adj[i][j]=min(adj[i][j],adj[i][k]+adj[k][j]);
                                        }
                                }
                        }
                }
};
