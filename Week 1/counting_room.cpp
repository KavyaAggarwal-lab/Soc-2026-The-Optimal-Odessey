#include <bits/stdc++.h>
using namespace std;
void dfs(int s, bool visited[], vector<int> adj[]) {
    if (visited[s]) return;
    visited[s] = true;
    for (auto u: adj[s]) {
        if(u==s) continue;
        dfs(u , visited , adj);
}
}

int main() {
	int x , y,n=0; cin>>x>>y;
	char mat[x][y];
    bool visited[x*y+1];
    for (int i=0;i<x*y+1;i++){visited[i]=false;}
    int c=0;
    vector <int> adj[(x*y)+ 1];
    
    
	for(int i=0; i< x;i++){
	    for(int p=0; p<y;p++){
	        cin >> mat[i][p];
	        if(mat[i][p]=='.') n++;
	    }
	}
    for(int i=0; i<x ; i++){
        for(int p=0;p<y;p++){
            if(mat[i][p]=='.'){
                if(p+1<y && mat[i][p+1]=='.') {
                    adj[i*y+p+1].push_back(i*y+p+2);
                    adj[i*y+p+2].push_back(i*y+p+1);}
                if( i+1< x && mat[i+1][p]=='.') {
                    adj[i*y+p+1].push_back((i+1)*y+p+1);
                    adj[(i+1)*y+p+1].push_back((i)*y+p+1);}
                if(adj[(i)*y+p+1].empty()) adj[(i)*y+p+1].push_back((i)*y+p+1);
            }
        }
        
    }
    for(int i=0; i<x ; i++){
        for(int p=0;p<y;p++){
            if(!adj[i*y+p+1].empty() and visited[(i)*y+p+1] == false){
                dfs((i)*y+p+1, visited,adj);
                c++;
            }
        }
    }
    cout<<c<<endl;
}
