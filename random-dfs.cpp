#include "ford-fulkerson.hpp"

bool randomDfs(Graph& g, long long u, long long sink, std::vector<bool>& visited, 
    std::vector<long long>& parent, std::vector<long long>& edge_used, 
    long long& vertices_visited, long long& edges_visited, 
    std::mt19937& rng) {

if (u == sink) return true;

std::vector<long long> edges = g.adj[u];
std::shuffle(edges.begin(), edges.end(), rng);

for (long long edge_idx : edges) {
Edge& e = g.edges[edge_idx];

if (!visited[e.to] && e.residual() > 0) {
 visited[e.to] = true;
 parent[e.to] = u;
 edge_used[e.to] = edge_idx;
 vertices_visited++;
 edges_visited++;
 
 if (randomDfs(g, e.to, sink, visited, parent, edge_used, 
                vertices_visited, edges_visited, rng)) {
     return true;
 }
}
}

return false;
}

long long randomizedFordFulkerson(Graph& g, long long source, long long sink, FFStats& stats) {
    std::vector<long long> parent(g.n + 1);
    std::vector<bool> visited(g.n + 1);
    std::vector<long long> edge_used(g.n + 1);
    
    std::mt19937 rng(42);
    
    long long max_flow = 0;
    stats = FFStats();
    
    while (true) {
        std::fill(visited.begin(), visited.end(), false);
        std::fill(parent.begin(), parent.end(), -1);
        
        visited[source] = true;
        long long vertices_visited = 1; 
        long long edges_visited = 0;
        
        bool path_found = randomDfs(g, source, sink, visited, parent, edge_used, 
                                    vertices_visited, edges_visited, rng);
        
        if (!path_found) break;
        
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
    
    std::cout << "Running Randomized Ford-Fulkerson algorithm on " << argv[1] << std::endl;
    std::cout << "Graph has " << g.n << " vertices and " << g.edges.size()/2 << " edges" << std::endl;
    
    FFStats stats;
    
    auto start = std::chrono::high_resolution_clock::now();
    long long max_flow = randomizedFordFulkerson(g, source, sink, stats);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "Maximum flow: " << max_flow << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    //stats.printSummary();
    
    return 0;
}
