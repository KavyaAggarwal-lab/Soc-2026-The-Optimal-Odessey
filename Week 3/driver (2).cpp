#include<bits/stdc++.h>
#include"json.hpp"
#include<chrono>
using namespace std;
using json=nlohmann::ordered_json;
using namespace std::chrono;

const long long INF = 1e18;

struct Node {
    int pos;
    Node(int x) {pos=x;}
    Node(){}
};

class Graph{
    public:
        vector <bool> space;
        int m,n,p;
        Graph(json jsonfile){
            m= jsonfile["grid_size"]["rows"];
            n= jsonfile["grid_size"]["cols"];
            p=m*n;
            space = vector<bool> (m*n,true);
            for(auto k: jsonfile["obstacles"]){
                space[int(k["x"])+int(k["y"])*n]=false;
                p--;
            }

        }
        vector<Node> get_neighbors(Node current){
            vector<Node> v;
            int currpos = current.pos;
            if(!space[currpos]){return v;}
            if(currpos% n!= 0 && space[currpos-1]){
                v.push_back(Node(currpos-1));
            }
            if(currpos% n!= n-1 && space[currpos+1]){
                v.push_back(Node(currpos+1));
            }
            if(currpos/m != 0 && space[currpos-n]){
                v.push_back(Node(currpos-n));
            }
            if(currpos/m != m-1 && space[currpos+n]){
                v.push_back(Node(currpos+n));
            }
            return v;
        }
};


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
    file1 >> graph_json; // reading the graph_json file into json

        /!!! Need changes here !!!/

        // Create a Graph class that takes the json input
        // and stores the required graph data structures.
        // Remember to include the header file containing the class.
        // Uncomment the line below after implementing the class.
        Graph map(graph_json);

    std::ifstream file2(query_json_file);

    if (!file2.is_open()) {
        std::cerr << "Error: Could not open " << query_json_file << '\n';
        return 1;
    }

    json query_json;
    file2 >> query_json; // reading the query_json file into json object

    json output_json;

    output_json["meta"] = {{"id", query_json["meta"]["id"]}};
    output_json["results"] = json::array();

    std::string type;

    for(auto event : query_json["events"]){

        type = event["type"];

                /* Refer to the sample code below */
        /*
        if(type == "remove_edge") {
            int edge_id = event["edge_id"];
            bool b = remove_edge(map, edge_id);
            nlohmann::json out;
            out["id"] = event["id"];
            out["done"] = b;
            std::cout << out.dump(4) << std::endl;
            output_json["results"].push_back(out);
        }
        */
        float h[map.m * map.n],f[map.m * map.n];
        for(int i=0; i<=map.m * map.n ;i++){
            if(!map.space[i]) continue;
            h[i]= sqrt(pow((int(event["goal"]["x"])-i%map.n) ,2)+ pow(int(event["goal"]["y"])-i/map.n,2));
            f[i]=abs(int(event["goal"]["x"])-i%map.n )+abs(int(event["goal"]["y"])-i/map.n);
        }
        if(type == "find_path"){
            int start =map.n*int(event["start"]["y"])+int(event["start"]["x"]);
            int goal =map.n*int(event["goal"]["y"])+int(event["goal"]["x"]);
            Node A = Node(start),B = Node(goal);
            json out;
            out ["id"]=event["id"];
            vector<string> b;
            b={"dijkstra","astar_euclidean", "astar_manhattan"};
            bool path_found=false;
            int n = map.n; int m = map.m;
            for(int i=0;i<3;i++){
                auto begin=high_resolution_clock::now();

                    vector<long long> distance(n*m, INF);
                    vector<bool> processed(n*m,false);
                    priority_queue <pair<float,int>> q;
                    vector<int> arr(n*m,-1);
                    int nodes_explored=0;
                    distance[start]=0;
                    q.push({0,start});
                    while(!q.empty()){
                        if(processed[goal]) break;
                        int a=q.top().second; q.pop();
                        if(processed[a])continue;
                        processed[a]=true; nodes_explored++;
                        for(auto u: map.get_neighbors(Node(a))){
                            int b=u.pos;
                            if(distance[a]+1<distance[b]){
                                distance[b]=distance[a]+1;
                                if(i==0) q.push({-distance[b],b});
                                if(i==1)q.push({-distance[b]-h[b],b});
                                if(i==2)q.push({-distance[b]-f[b],b});
                            }
                        }
                    }
                if(distance[goal]==INF) path_found= false;
                else{ path_found =true;}
                auto end=high_resolution_clock::now();
                auto t=duration<double,milli>(end-begin);
                out[b[i]]["time_ms"]=t.count();
                out[b[i]]["nodes_explored"]=nodes_explored;
                out[b[i]]["path_length"]=distance[goal];
                out[b[i]]["path_found"]=path_found;

            }
            std::cout << out.dump(4) << std::endl;
            output_json["results"].push_back(out);
        }
    }

    ofstream(output_file)<<output_json.dump(4);
    return 0;
}