#include "graphsearch.h"

#include <queue>
#include <iostream>

using namespace std;
std::vector<int> GraphSearch::dfsSimple(const vector<vector<int> > &g, vector<bool> &visited,
                                        int pos, vector<int>& res)
{
    visited[pos] = true;
    res.push_back(pos);
    for (const auto& i : g[pos])
        if (!visited[i])
            dfsSimple(g, visited, i, res);
    return res;
}

GraphSearch::GraphSearch()
{

}

std::vector<int> GraphSearch::dfs(const std::vector<std::vector<int> > &g, int pos)
{
    auto visited = vector<bool>(g.size(), false);
    auto res = vector<int>();
    return dfsSimple(g, visited, pos, res);
}

std::vector<int> GraphSearch::bfs(const std::vector<std::vector<int> > &g, int pos)
{
    vector<int> results;
    queue<int> q;

    q.push (pos);
    results.push_back(pos);
    int n = g.size();

    vector<bool> visited(n, false);
    visited[pos] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (const auto& item : g[current]) {
            int to = item;
            if (!visited[to]) {
                visited[to] = true;
                q.push (to);
                results.push_back(to);
            }
        }
    }

	return results;
}

std::vector<std::vector<int> > GraphSearch::mstPrims(const std::vector<std::vector<int> > &g, const int& INF)
{
	int n = g.size();

	vector< vector<int> > result(n, vector<int>(n, INF));

	vector<bool> visited (n);
	vector<int> minimalEdgeCosts (n, INF), selectedEdges (n, -1);
	minimalEdgeCosts[0] = 0;
	for (int i = 0; i < n; ++i) {
		int v = -1;
		for (int j = 0; j < n ; ++j) {
			if (!visited[j] && (v == -1 || minimalEdgeCosts[j] < minimalEdgeCosts[v]))
				v = j;
		}
		if (minimalEdgeCosts[v] == INF) {
			return vector< vector<int> > ();
		}

		visited[v] = true;
		if (selectedEdges[v] != -1){
			result[selectedEdges[v]][v] = result[v][selectedEdges[v]] = g[v][selectedEdges[v]];
			cout << v << ' ';
			cout << selectedEdges[v] << " : ";
			cout << g[v][selectedEdges[v]] << endl;
		}

		for (int to = 0; to < n; ++to) {
			if (g[v][to] < minimalEdgeCosts[to]) {
				minimalEdgeCosts[to] = g[v][to];
				selectedEdges[to] = v;
			}
		}
	}
	return result;
}
