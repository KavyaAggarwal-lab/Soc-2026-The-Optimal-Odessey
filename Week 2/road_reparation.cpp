#include <bits/stdc++.h>
using namespace std;
int find(int a, int link[]){
    while(a!=link[a]) a=link[a];
    return link[a];
}
bool same(int a, int b, int link[]){
    return find(a,link)==find(b,link);
}
void unite(int a, int b, int size[],int link[]){
    a=find(a,link);
    b=find(b,link);
    if(size[a]<size[b]) {//swap(a,b);
        int c=a;
        a=b;
        b=c;
    }
    link[b]=link[a];
    size[a]+=size[b];
}

int main(){
    int n, m,count=0; cin>>n>>m;
    long long c=0;
    priority_queue <tuple<long long,int,int>> q;
    while(m--){
        long long z;
        int  p,r; cin>>p>>r>>z;
        q.push({-z,p-1,r-1});
    }
    int size[n],link[n];
    fill(size,size+n,1);
    for(int i=0;i<n;i++) link[i]=i;
    while(!q.empty()){
        if(count==n-1) break;
        auto [w,a,b] = q.top();
        q.pop();
        if(!same(a,b,link)) {unite(a,b,size,link);  c+=w; count++;}
    }
    if(count!= n-1) cout <<"IMPOSSIBLE";
    else cout<<-c;
    

    
}
    