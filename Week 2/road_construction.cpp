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
    link[b]=a;
    size[a]+=size[b];
    
}

int main(){
    int n, m,count,maxsize=1; cin>>n>>m;
    count=n;
    vector <pair<int,int>> q;
    int size[n],link[n];
    fill(size,size+n,1);
    for(int i=0;i<n;i++) link[i]=i;
    while(m--){
        int a,b; cin>>a>>b;
        a--; b--;
        q.push_back({a,b});
        
        if(!same(a,b,link)) {unite(a,b,size,link);  count--;}
        maxsize=max(maxsize,size[find(a,link)]);
        cout<<count<<" "<<maxsize<<endl;
    }
    

    
}
    