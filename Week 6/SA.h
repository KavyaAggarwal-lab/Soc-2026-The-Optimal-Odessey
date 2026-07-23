#pragma once
#include <vector>
#include <random>
using namespace std;

// ---------------------------------------------------------------------
// Week 6 — Simulated Annealing for TSP
// Fill in the bodies of these functions in SA.cpp (or inline here,
// ---------------------------------------------------------------------

// Cost of a closed tour (tour[0] == tour.back()) given an m x m distance
// matrix. Reuse this — you already wrote it in Week 5.
double tour_cost(const vector<int>& tour, vector<vector<double>>& dist){
    double x;
    for(int i=0;i<tour.size()-1;i++){
        x+=dist[tour[i]][tour[i+1]];
    }
    return x;
}

// A cheap starting tour. Any valid Hamiltonian cycle works (random shuffle
// is fine), but a greedy Nearest Neighbor tour gives SA a much better
// starting point and fewer iterations to converge.
vector<int> nearest_neighbor_tour(vector<vector<double>>& dist){
    int INF= 1000000;
    int mind,next, len = dist[0].size();
    bool visited[len];
    for(int i=0;i<len;i++) visited[i]=false;
    visited[0]=true;
    vector<int> nearest_neighbor ;
    nearest_neighbor.push_back(1);
    for(int k=0;k<len;k++){
        mind=INF;
        int j= nearest_neighbor[k];
        int next;
        for(int i=0;i<len-1;i++){
            if(visited[i]== false and i!=j){
                if(dist[i][j]<mind) {
                    next=i; mind= dist[i][j];}}
        }
        nearest_neighbor.push_back(next+1);
        visited[next]=true;
    }   
    nearest_neighbor.push_back(1);
    return nearest_neighbor;
}

// Produce a neighboring tour from `current` by perturbing it slightly.
// The classic choice for TSP is a 2-opt move: pick two positions i < j in
// the tour and reverse the segment between them. Keep node 0 fixed as the
// start/end so every "tour" stays a valid closed cycle.
vector<int> two_opt_neighbor(const vector<int>& current){
    random_device rd;
    mt19937 gen(rd());
    int n;    
    n=current.size() - 1;
    uniform_int_distribution<> dist1(0, n - 2);
    int i = dist1(gen);
    uniform_int_distribution<> dist2(i + 1, n - 1);
    int j = dist2(gen);
    vector<int> pert;
    for(int k=0;k<i;k++) pert[k]=current[k];
    for(int k=i;k<=j;k++) pert[k]=current[j+i-k];
    for(int k=j+1;k<n;k++) pert[k]= current[k];
    pert[n]=pert[0];
    return pert;
};

// Metropolis acceptance criterion. Always accept improving moves
// (new_cost < old_cost). Accept worsening moves with probability
// exp(-(new_cost - old_cost) / temperature) — this is what lets SA escape
// local minima early on, when temperature is high.
double acceptance_probability(double old_cost, double new_cost, double temperature){
    if(new_cost > old_cost) return exp(-(new_cost - old_cost) / temperature);
    else return 1;
}

// The main driver of the algorithm. Starting from an initial tour, repeat:
//   1. generate a neighbor (two_opt_neighbor)
//   2. decide whether to move to it (acceptance_probability)
//   3. track the best tour seen so far
//   4. cool down: temperature *= cooling_rate
// until temperature drops below min_temp.
//
// Returns the best tour found, using LOCAL indices [0..m-1] into `dist`
// (same convention as christofides() in Week 5's TSP.h) — driver.cpp maps
// these back to original node IDs.
vector<int> simulated_annealing(vector<vector<double>>& dist,double initial_temp = 10000.0,double cooling_rate = 0.995, double min_temp = 1e-6,int iters_per_temp = 200){
    random_device rd;
    mt19937 gen(rd());
    vector<int> current , neighbour , best;
    current = nearest_neighbor_tour(dist);
    double delta, t;
    t = initial_temp;
    while(t>min_temp){
        for(int i=0;i<iters_per_temp;i++){
            neighbour = two_opt_neighbor(current);
            delta  = tour_cost(neighbour,dist) - tour_cost(current,dist);
            double prob = acceptance_probability(tour_cost(current,dist),tour_cost(neighbour,dist), t);
            if(delta <0) current = neighbour;
            else {
                uniform_real_distribution<double> dist2(0.0, 1.0);
                double x = dist2(gen);
                if(x<= prob) current = neighbour;
            }
            if(tour_cost(current,dist)<tour_cost(best,dist)) best = current;
        }
        t = t*cooling_rate;
    }
    return best;
};
