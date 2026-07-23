#include<bits/stdc++.h>
using namespace std;
using json=nlohmann::ordered_json;

struct Node{
        int pos;
        Node(int a){
                pos=a;
        }
        Node(){
        }
};

class Graph{
        vector<bool> space;
        public:
                int m,n;
                Graph(json jsonfile){
                        m=jsonfile["grid_size"]["cols"];
                        n=jsonfile["grid_size"]["rows"];
                        space=vector<bool>(m*n,true);
                        for(auto k:jsonfile["obstacles"]){
                                space[m*int(k["y"])+int(k["x"])]=false;
                        }
                }
                vector<Node> get_neighbors(Node current){
                        vector<Node> v;
                        int pos=current.pos;
                        if(!space[pos]){
                                return v;
                        }
                        if(pos%m>0&&space[pos-1]){
                                v.push_back(Node(pos-1));
                        }
                        if(pos%m<m-1&&space[pos+1]){
                                v.push_back(Node(pos+1));
                        }
                        if(pos>m-1&&space[pos-m]){
                                v.push_back(Node(pos-m));
                        }
                        if(pos<(n-1)*m&&space[pos+m]){
                                v.push_back(Node(pos+m));
                        }
                        return v;
                }
};
