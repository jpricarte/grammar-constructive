/*
 * Based on a code by M. Ritt 
 */

#include <algorithm>
#include <vector>

struct UnionFind {
    std::vector<int> parent; // parent[i]=-r: i is root of rank r-1
    unsigned numComponents;  // inv.: nc == # { p[i]<0 } == # roots

    UnionFind() : numComponents(0) {};
    UnionFind(int n) : parent(n, -1), numComponents(n) {};
    UnionFind(struct UnionFind const &uf) : numComponents(uf.numComponents) {
        parent = uf.parent;
    };

    int find(int _e) {
        if (parent[_e] < 0)
            return _e;
        int e = _e;
        while (parent[e] >= 0)
            e = parent[e];
        parent[_e] = e;
        return e;
    }

    void unite(int e, int f) {
        e = find(e);
        f = find(f);

        if (e != f) {
            int re = -parent[e], rf = -parent[f];
            if (re < rf)
                std::swap(e, f);

            parent[f] = e;
            // sz[e] += sz[f];
            if (re == rf)
                parent[e]--;
            numComponents--;
        }
        else 
            e = e;
    }

    unsigned nComp() const { return numComponents; }

    unsigned size() const { return (unsigned) parent.size(); }
};