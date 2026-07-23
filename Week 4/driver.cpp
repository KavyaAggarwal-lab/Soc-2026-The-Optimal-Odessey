#include<bits/stdc++.h>
#include"json.hpp"
#include"graph.h"
#define large_dist 100000000
using namespace std;
using json=nlohmann::ordered_json;

void TSPBruteForce(vector<vector<int>> &matrix,vector<int> &nodes,vector<int> path_covered,vector<int> &min_path,int pos,vector<bool> visited,int &min_cost,int cost){
        if(path_covered.size()==nodes.size()){
                if(cost+matrix[nodes[pos]][0]<min_cost){
                        min_cost=cost+matrix[nodes[pos]][0];
                        path_covered.push_back(0);
                        min_path=path_covered;
                }
                return;
        }
        if(cost>=min_cost){
                return;
        }
        for(int i=0;i<nodes.size();i++){
                if(visited[i]){
                        continue;
                }
                visited[i]=true;
                path_covered.push_back(nodes[i]);
                TSPBruteForce(matrix,nodes,path_covered,min_path,i,visited,min_cost,cost+matrix[nodes[pos]][nodes[i]]);
                visited[i]=false;
                path_covered.pop_back();
        }
        return;
}

void TSPOptimized(vector<vector<int>>& matrix,vector<int>& nodes,vector<int>& tour,int& optimal_cost){
    int n=nodes.size(),full=(1<<n);
    vector<vector<int>> dp(full,vector<int>(n,large_dist)),parent(full,vector<int>(n,-1));
    dp[1][0]=0;
    for(int msk=1;msk<full;msk++) for(int u=0;u<n;u++){
        if(!(msk&(1<<u))||dp[msk][u]==large_dist) continue;
        for(int v=0;v<n;v++){
            if(msk&(1<<v)) continue;
            int new_msk=msk|(1<<v);
            int new_cost=dp[msk][u]+matrix[nodes[u]][nodes[v]];
            if(new_cost<dp[new_msk][v]){
                dp[new_msk][v]=new_cost;
                parent[new_msk][v]=u;
            }
        }
    }
    int final_msk=full-1,last=-1;
    optimal_cost=large_dist;
    for(int u=1;u<n;u++){
        int total_cost=dp[final_msk][u]+matrix[nodes[u]][nodes[0]];
        if(total_cost<optimal_cost){
            optimal_cost=total_cost;
            last=u;
        }
    }
    vector<int> rev;
    int msk=final_msk,now=last;
    while(now!=-1){
        rev.push_back(nodes[now]);
        int prev=parent[msk][now];
        msk^=(1<<now);
        now=prev;
    }
    reverse(rev.begin(),rev.end());
    rev.push_back(nodes[0]);
    tour=rev;
}


int main(int argc, char* argv[]){

    if (argc < 4) {
        std::cerr << "Usage: ./{executable} <graph.json> <queries.json> <output.json>\n";
        return 1;
    }

    std::string graph_json_file = argv[1];
    std::string query_json_file = argv[2];
    std::string output_file = argv[3];

    std::ifstream file1(graph_json_file);

    if (!file1.is_open()) {
        std::cerr << "Error: Could not open " << graph_json_file << '\n';
        return 1;
    }

    nlohmann::json graph_json;
    file1 >> graph_json;
    Graph map(graph_json);

    std::ifstream file2(query_json_file);

    if (!file2.is_open()) {
        std::cerr << "Error: Could not open " << query_json_file << '\n';
        return 1;
    }

    json query_json;
    file2 >> query_json;

    json output_json;

    output_json["meta"] = {{"id", query_json["meta"]["id"]}};
    output_json["results"] = nlohmann::json::array();

    std::string type;

    for(auto event : query_json["events"]){
        type = event["type"];
        if(type=="tsp"){
                json output;
                output["id"]=event["id"];
                vector<int> nodes;
                for(auto i:event["nodes"]){
                        nodes.push_back(i);
                }
                vector<int> final_path,path;
                path.push_back(0);
                int cost=0;
                int min_cost=large_dist;
                vector<bool> visited(nodes.size(),false);
                visited[0]=true;
                TSPBruteForce(map.matrix,nodes,path,final_path,0,visited,min_cost,cost);
                output["brute_force"]["optimal_cost"]=min_cost;
                output["brute_force"]["tour"]=final_path;
                vector<int> tour1;
                int costm=large_dist;
                TSPOptimized(map.matrix,nodes,tour1,costm);
                output["held_karp"]["optimal_cost"]=costm;
                output["held_karp"]["tour"]=tour1;
                output_json["results"].push_back(output);
        }
    }
    ofstream(output_file)<<output_json.dump(4);
    return 0;
}