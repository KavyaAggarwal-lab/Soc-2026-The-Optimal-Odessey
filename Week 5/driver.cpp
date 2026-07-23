#include<bits/stdc++.h>
#include"json.hpp"
#include<chrono>
#include"TSP.h"
using namespace std;
using namespace std::chrono;
using json=nlohmann::ordered_json;

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
                auto start=high_resolution_clock::now();
                json result;
                result["id"]=event["id"];
                vector<int> node;
                for(auto i:event["nodes"]){
                        node.push_back(i);
                }
                vector<int> path=christofides(map.adj, node);
                result["tour"]=path;
                result["cost"]=tour_cost(path,map.adj);
                auto end=high_resolution_clock::now();
                result["time_us"]=(end-start).count();
                output_json["results"].push_back(result);
        }
    }
    ofstream(output_file)<<output_json.dump(4);
    return 0;
}
