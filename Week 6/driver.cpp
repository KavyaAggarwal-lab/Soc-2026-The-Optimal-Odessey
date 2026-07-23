#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"json.hpp"
#include<chrono>
#include<random>
#include"SA.h"
#include"Graph.h"

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

    nlohmann::json query_json;
    file2 >> query_json;

    nlohmann::json output_json;

    output_json["meta"] = {{"id", query_json["meta"]["id"]}};
    output_json["results"] = nlohmann::json::array();

    std::string type;

    for(auto event : query_json["events"]){

        type = event["type"];

        if (type == "tsp") {
                vector<int> nodes=event["nodes"].get<vector<int>>();
                vector<vector<double>> dist(nodes.size(),vector<double>(nodes.size()));
                for(int i=0;i<nodes.size();i++){
                        for(int j=0;j<nodes.size();j++){
                                dist[i][j]=map.adj[nodes[i]][nodes[j]];
                        }
                }
            auto t0 = std::chrono::high_resolution_clock::now();
            vector<int> local_tour = simulated_annealing(dist);
            auto t1 = std::chrono::high_resolution_clock::now();
            double time_us = std::chrono::duration<double, std::micro>(t1 - t0).count();

            vector<int> tour;
            for (int i : local_tour) tour.push_back(nodes[i]);
            double cost = tour_cost(local_tour, dist);

            nlohmann::json out;
            out["id"] = event["id"];
            out["simulated_annealing"]={{"cost", cost},{"tour", tour},{"time_us", time_us}};
            output_json["results"].push_back(out);
        }
    }

    std::ofstream fout(output_file);
    fout << output_json.dump(4);
    return 0;
}
