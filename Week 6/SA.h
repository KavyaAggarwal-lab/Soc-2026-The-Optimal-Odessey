#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

// ---------------------------------------------------------------------
// Week 6 — Simulated Annealing for TSP
// ---------------------------------------------------------------------

// Cost of a closed tour (tour[0] == tour.back()) given an m x m distance matrix.
double tour_cost(const vector<int>& tour, const vector<vector<double>>& dist) {
    double x = 0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        x += dist[tour[i]][tour[i+1]];
    }
    return x;
}

// A cheap starting tour using a Greedy Nearest Neighbor approach.
vector<int> nearest_neighbor_tour(const vector<vector<double>>& dist) {
    int len = dist.size();
    vector<bool> visited(len, false);
    visited[0] = true;

    vector<int> tour;
    tour.reserve(len + 1);
    tour.push_back(0);

    int curr = 0;
    // We need to add len-1 more cities to complete the initial pass
    for (int k = 1; k < len; k++) {
        double mind = numeric_limits<double>::max();
        int next_node = -1;

        for (int i = 0; i < len; i++) {
            if (!visited[i] && dist[curr][i] < mind) {
                mind = dist[curr][i];
                next_node = i;
            }
        }

        tour.push_back(next_node);
        visited[next_node] = true;
        curr = next_node;
    }

    tour.push_back(0); // Close the cycle
    return tour;
}

// Produce a neighboring tour from current using a 2-opt move.
vector<int> two_opt_neighbor(const vector<int>& current) {
    static random_device rd;
    static mt19937 gen(rd());
    int n = current.size() - 1; // Number of distinct cities

    // Pick two distinct indices to reverse the segment between them
    uniform_int_distribution<int> d1(1, n - 2);
    int i = d1(gen);
    uniform_int_distribution<int> d2(i + 1, n - 1);
    int j = d2(gen);

    vector<int> next = current;
    reverse(next.begin() + i, next.begin() + j + 1);

    return next;
}

// Metropolis acceptance criterion.
double acceptance_probability(double old_cost, double new_cost, double temperature) {
    if (new_cost < old_cost) return 1.0;
    return exp(-(new_cost - old_cost) / temperature);
}

// The main driver of the algorithm.
vector<int> simulated_annealing(const vector<vector<double>>& dist,
                                double temp = 10000,
                                double cooling = 0.995,
                                double min_temp = 1e-6,
                                int iters = 200) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> prob(0.0, 1.0);

    vector<int> current = nearest_neighbor_tour(dist);
    vector<int> best = current;

    // Track costs in variables to avoid O(N) recalculations inside the loop
    double current_cost = tour_cost(current, dist);
    double best_cost = current_cost;

    while (temp > min_temp) {
        for (int k = 0; k < iters; k++) {
            vector<int> next = two_opt_neighbor(current);
            double next_cost = tour_cost(next, dist);

            if (prob(gen) <= acceptance_probability(current_cost, next_cost, temp)) {
                current = next;
                current_cost = next_cost;
            }

            if (current_cost < best_cost) {
                best = current;
                best_cost = current_cost;
            }
        }
        temp *= cooling;
    }

    return best;
}
