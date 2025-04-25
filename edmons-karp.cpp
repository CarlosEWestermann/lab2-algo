#include "ford-fulkerson.hpp"

long long edmondsKarp(Graph& g, long long source, long long sink, FFStats& stats) {
    std::vector<long long> parent(g.n + 1);
    std::vector<bool> visited(g.n + 1);
    std::vector<long long> edge_used(g.n + 1);
    
    long long max_flow = 0;
    stats = FFStats(); 
    
    while (true) {

        std::fill(visited.begin(), visited.end(), false);
        std::fill(parent.begin(), parent.end(), -1);
        
        std::queue<long long> q;
        q.push(source);
        visited[source] = true;
        
        long long vertices_visited = 1; 
        long long edges_visited = 0;
        
        while (!q.empty() && !visited[sink]) {
            long long u = q.front();
            q.pop();
            
            for (long long edge_idx : g.adj[u]) {
                Edge& e = g.edges[edge_idx];
                
                if (!visited[e.to] && e.residual() > 0) {
                    visited[e.to] = true;
                    parent[e.to] = u;
                    edge_used[e.to] = edge_idx;
                    q.push(e.to);
                    vertices_visited++;
                    edges_visited++;
                }
            }
        }
      
        if (!visited[sink]) break;
        
    
        long long path_flow = std::numeric_limits<long long>::max();
        long long path_length = 0;
        
        for (long long v = sink; v != source; v = parent[v]) {
            long long edge_idx = edge_used[v];
            path_flow = std::min(path_flow, g.edges[edge_idx].residual());
            path_length++;
        } 
        
        for (long long v = sink; v != source; v = parent[v]) {
            long long edge_idx = edge_used[v];
            g.edges[edge_idx].flow += path_flow;
            g.edges[edge_idx ^ 1].flow -= path_flow; 
        }
        
        max_flow += path_flow;
        stats.iterations++;
        stats.vertices_visited += vertices_visited;
        stats.edges_visited += edges_visited;
        stats.addIterationStats(vertices_visited, edges_visited, path_length);
    }
    
    return max_flow;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    long long source, sink;
    Graph g = readDimacs(argv[1], source, sink);
    
    std::cout << "Running Edmonds-Karp algorithm on " << argv[1] << std::endl;
    std::cout << "Graph has " << g.n << " vertices and " << g.edges.size()/2 << " edges" << std::endl;
    
    FFStats stats;
    
    auto start = std::chrono::high_resolution_clock::now();
    long long max_flow = edmondsKarp(g, source, sink, stats);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "Maximum flow: " << max_flow << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    //stats.printSummary();
    
    return 0;
}
