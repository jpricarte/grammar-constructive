/*
 * Based on a code by M. Ritt
 */

#include "graph.hpp"
#include <fstream>
#include <cassert>
#include <queue>
#include <numeric>
#include <algorithm>

using namespace std;

void Graph::read(string ifname) {
    ifstream in(ifname);
    if (!in.is_open())
        throw invalid_argument("Input file does not exist.");
    read(in);
    in.close();
}

void Graph::read(istream& in) {
    in >> nNodes >> nEdges >> nColors >> nMaxColors;
    nodeDegree.resize(nNodes);

    vector<unsigned> sourceVec, destinationVec, colorVec;
    for (auto i = 0u; i != nEdges; i++) {
        unsigned edgeSource, edgeDest, edgeColor;
        in >> edgeSource >> edgeDest >> edgeColor;
        edgeSource--;
        edgeDest--;
        edgeColor--;
        sourceVec.push_back(edgeSource);
        destinationVec.push_back(edgeDest);
        colorVec.push_back(edgeColor);
        nodeDegree[edgeSource]++;
        nodeDegree[edgeDest]++;
    }
    build_from_vectors(sourceVec, destinationVec, colorVec);
}

void Graph::build_from_vectors(const vector<unsigned>& sourceVec, const vector<unsigned>& destVec, const vector<unsigned>& colorVec) {
    inc.resize(2 * nEdges);
    col.resize(2 * nEdges);

    inp.resize(nNodes + 1);
    inp[0] = 0;
    for (auto i = 1u; i != nNodes + 1; i++)
        inp[i] = inp[i - 1] + nodeDegree[i - 1];
    for (auto i = 0u; i != nEdges; i++) {
        inc[inp[sourceVec[i]]] = destVec[i];
        inc[inp[destVec[i]]] = sourceVec[i];
        col[inp[sourceVec[i]]] = colorVec[i];
        col[inp[destVec[i]]] = colorVec[i];
        inp[sourceVec[i]]++;
        inp[destVec[i]]++;
    }
    inp[0] = 0;
    for (auto i = 1u; i != nNodes + 1; i++)
        inp[i] = inp[i - 1] + nodeDegree[i - 1];

    nEdges *= 2;

    ce.resize(nColors);
    // compute_ce();
    compute_sf();

    // reorder colors by number of edges

    cep.resize(nColors);
    iota(cep.begin(), cep.end(), 0);
    sort(cep.begin(), cep.end(), [&](unsigned c1, unsigned c2) { return ce[c1].size() > ce[c2].size(); });
    vector<unsigned> icep(nColors, 0); // inverse permutation
    for (auto i = 0u; i != nColors; i++)
        icep[cep[i]] = i;
    for (auto i = 0u; i != nEdges; i++) // rename colors
        col[i] = icep[col[i]];
    compute_sf(); // and recompute
}

// store edges of color `c` in ce[c]
void Graph::compute_ce() {
    for (auto i = 0u; i != nColors; i++) {
        ce[i].clear();
        for (auto j = 0u; j != nNodes; ++j)
            for (auto k = inp[j], ke = inp[j + 1]; k != ke; ++k)
                if (col[k] == i && j < inc[k])
                    ce[i].push_back({ j, inc[k] });
    }
}

// store spanning forest edges of color `c` in ce[c]
void Graph::compute_sf() {
    for (auto i = 0u; i != nColors; i++) {
        ce[i].clear();
        sf_color(i);
    }
}

// compute spanning forest of color `c` store in ce[c]
void Graph::sf_color(const unsigned color) {
    assert(0 <= color && color < nColors);
    vector<bool> visited(nNodes, false);
    for (auto j = 0u; j != nNodes; ++j)
        if (!visited[j]) {
            visited[j] = true;
            bfs_color(j, color, visited);
        }
}

void Graph::bfs_color(unsigned uNode, const unsigned color, vector<bool>& visited) {
    queue<unsigned> queue;
    queue.push(uNode);
    while (!queue.empty()) {
        uNode = queue.front();
        queue.pop();
        for (auto k = inp[uNode], ke = inp[uNode + 1]; k != ke; ++k) {
            auto v = inc[k];
            if (col[k] == color && !visited[v]) {
                visited[v] = true;
                queue.push(v);
                ce[color].push_back({ uNode, v });
            }
        }
    }
}
