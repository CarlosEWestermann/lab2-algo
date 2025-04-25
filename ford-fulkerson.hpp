#ifndef FORD_FULKERSON_H
#define FORD_FULKERSON_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstring>
#include <limits>
#include <fstream>

struct Edge {
    long long from;
    long long to;
    long long capacity;
    long long flow;
    long long residual() const { return capacity - flow; }
};

struct Graph {
    long long n;                   
    std::vector<Edge> edges;  
    std::vector<std::vector<long long>> adj; 

    void addEdge(long long u, long long v, long long c) {
        long long idx = edges.size();
        edges.push_back({u, v, c, 0});
        edges.push_back({v, u, 0, 0}); 
        
        if (u >= adj.size()) adj.resize(u + 1);
        if (v >= adj.size()) adj.resize(v + 1);
        
        adj[u].push_back(idx);
        adj[v].push_back(idx + 1);
    }
    
    void resetFlow() {
        for (auto& edge : edges) {
            edge.flow = 0;
        }
    }
};


struct FFStats {
    long long iterations = 0;      
    long long vertices_visited = 0; 
    long long edges_visited = 0;    
 
    std::vector<long long> iter_vertices;
    std::vector<long long> iter_edges;
    std::vector<long long> path_lengths;
    
    void addIterationStats(long long vertices, long long edges, long long path_length) {
        iter_vertices.push_back(vertices);
        iter_edges.push_back(edges);
        path_lengths.push_back(path_length);
    }
    
    void printSummary() const {
        std::cout << "Total iterations: " << iterations << std::endl;
        std::cout << "Total vertices visited: " << vertices_visited << std::endl;
        std::cout << "Total edges visited: " << edges_visited << std::endl;
        
        if (!iter_vertices.empty()) {
            std::cout << "Average vertices per iteration: " 
                      << (double)vertices_visited / iterations << std::endl;
            std::cout << "Average edges per iteration: " 
                      << (double)edges_visited / iterations << std::endl;
            std::cout << "Average path length: " 
                      << (double)std::accumulate(path_lengths.begin(), path_lengths.end(), 0) / iterations << std::endl;
        }
    }
};

Graph readDimacs(const std::string& filename, long long& source, long long& sink) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }
    
    Graph g;
    source = sink = -1;
    
    std::string line;
    long long n = 0, m = 0;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == 'c') continue;
        
        if (line[0] == 'p') {
            sscanf(line.c_str(), "p max %lld %lld", &n, &m);
            g.n = n;
            g.adj.resize(n + 1);  
        }
        else if (line[0] == 'n') {
            long long v;
            char type;
            sscanf(line.c_str(), "n %lld %c", &v, &type);
            if (type == 's') source = v;
            else if (type == 't') sink = v;
        }
        else if (line[0] == 'a') {
            long long u, v, c;
            sscanf(line.c_str(), "a %lld %lld %lld", &u, &v, &c);
            g.addEdge(u, v, c);
        }
    }
    
    return g;
}

#endif
