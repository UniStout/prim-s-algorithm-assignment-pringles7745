//All procedures including main
//Author: Devyn Tyacke

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <queue>
#include <functional>
#include <limits>
#include "prim.h"

using namespace std;

pair<vector<tuple<int,int,int>>, int> primMST(
    int n,
    const vector<vector<pair<int,int>>> &adj)
{
    const int INF = numeric_limits<int>::max();
    vector<int> key(n, INF);
    vector<int> parent(n, -1);
    vector<char> inMST(n, 0);

    // min-heap of (key, vertex)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    int totalWeight = 0;
    vector<tuple<int,int,int>> mstEdges;

    for (int start = 0; start < n; ++start) {
        if (inMST[start]) continue; // already processed as part of a connected component

        // start a new component
        key[start] = 0;
        pq.emplace(0, start);

        while (!pq.empty()) {
            auto top = pq.top(); pq.pop();
            int k = top.first;
            int u = top.second;
            if (inMST[u]) continue;
            inMST[u] = 1;
            totalWeight += k; // for start vertex k==0 contributes nothing

            if (parent[u] != -1) {
                // record edge parent[u] - u with weight k
                mstEdges.emplace_back(parent[u], u, k);
            }

            for (auto &e : adj[u]) {
                int v = e.first;
                int w = e.second;
                if (!inMST[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                    pq.emplace(key[v], v);
                }
            }
        }
    }

    return {mstEdges, totalWeight};
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Prefer a file named "inputData.txt" in the program directory.
    // If it's not present, allow overriding via first command-line argument,
    // otherwise fall back to stdin.
    istream *in = &cin;
    ifstream infile;

    infile.open("primsData.txt");
    if (infile) {
        in = &infile;
    } else if (argc >= 2) {
        infile.open(argv[1]);
        if (!infile) {
            cerr << "Failed to open file: " << argv[1] << '\n';
            return 1;
        }
        in = &infile;
    }

    int n;
    if (!(*in >> n)) {
        cerr << "Failed to read number of vertices.\n";
        return 1;
    }

    int m;
    if (!(*in >> m)) {
        cerr << "Failed to read number of edges.\n";
        return 1;
    }

    if (n < 0) {
        cerr << "Number of vertices must be non-negative.\n";
        return 1;
    }

    vector<vector<pair<int,int>>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        if (!(*in >> u >> v >> w)) {
            cerr << "Failed to read edge #" << i+1 << '\n';
            return 1;
        }
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cerr << "Edge endpoints out of range: " << u << " " << v << '\n';
            return 1;
        }
        // undirected graph
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    auto mstResult = primMST(n, adj);
    auto &edges = mstResult.first;
    auto &totalWeight = mstResult.second;

    // Check connectivity (MST for a connected graph has exactly n-1 edges)
    if ((int)edges.size() != max(0, n - 1)) {
        cout << "Graph is disconnected; produced a minimum spanning forest.\n";
    }

    // Print MST edges (u v weight) one per line
    for (auto &t : edges) {
        int u, v, w;
        tie(u, v, w) = t;
        cout << u << " " << v << " " << w << "\n";
    }

    cout << "Total weight: " << totalWeight << "\n";

    return 0;
}