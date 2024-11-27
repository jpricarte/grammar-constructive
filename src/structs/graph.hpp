#pragma once

#include <iostream>
#include <vector>

using Edge = std::pair<unsigned, unsigned>;

struct Graph {
    Graph() : nNodes(0), nEdges(0), nColors(0), nMaxColors(0) {}
    inline Graph(std::string ifname) { read(ifname); };         // read from file
    unsigned nNodes, nEdges, nColors, nMaxColors;               // nodes, 2*edges, number of different colors, number of labels we want to select
    std::vector<unsigned> nodeDegree;                           // node degree (0-based)
    std::vector<unsigned> inc, col;                             // global edge list, and colors (0-based)
    std::vector<unsigned> inp;                                  // inp[i] is the start of vertex i's edge list (0-based)
    std::vector<std::vector<Edge>> ce;                            // edge-list per color (List of elements)
    std::vector<unsigned> cep;                                  // permutation of colors (we reorder)

    void read(std::istream& in);
    void read(std::string ifname);
    void build_from_vectors(const std::vector<unsigned>&, const std::vector<unsigned>&, const std::vector<unsigned>&);

    // compute edge lists, per-color
    void compute_ce();
    // compute spanning forests, per-color
    void compute_sf();
    void sf_color(const unsigned);
    void bfs_color(unsigned, const unsigned, std::vector<bool>&);
};
