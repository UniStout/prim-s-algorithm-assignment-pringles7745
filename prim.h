//Please put header information in this file.
//Author: Devyn Tyacke
#ifndef PRIM_H
#define PRIM_H

#include <vector>
#include <tuple>

using namespace std;

// Returns a pair
// If the graph is disconnected, the returned edge list contains the MST forest
// and totalWeight is the sum of all forest edges.
pair<vector<tuple<int,int,int>>, int> primMST(
    int n,
    const vector<vector<pair<int,int>>> &adj);

#endif // PRIM_H
