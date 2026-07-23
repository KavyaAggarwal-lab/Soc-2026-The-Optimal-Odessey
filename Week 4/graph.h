#include<bits/stdc++.h>
using json=nlohmann::ordered_json;
using namespace std;
#define INF 100000000

class Graph{
        public:
                vector<vector<int>> matrix;
                int size;
                Graph(json graph_json){
                        size=graph_json["nodes"].size();
                        matrix=vector<vector<int>>(size,vector<int>(size,INF));
                        for(int i=0;i<size;i++){
                                matrix[i][i]=0;
                        }
                        for(auto i:graph_json["edges"]){
                                int u=i["u"];
                                int v=i["v"];
                                int w=i["w"];
                                matrix[u][v]=w;
                                matrix[v][u]=w;
                        }
                        for(int i=0;i<size;i++){
                                for(int j=0;j<size;j++){
                                        for(int k=0;k<size;k++){
                                                matrix[i][j]=min(matrix[i][j],matrix[i][k]+matrix[k][j]);
                                        }
                                }

                        }
                }

}