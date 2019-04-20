#include "graphsearch.h"

#include <queue>
#include <iostream>

using namespace std;

vector< pair < int, pair<int,int> > > convertAdjMatrixWithWeights(const vector< vector<int> >& vec, const int& INF);

void dfsOrder (const vector<vector<int> > &g, vector<char> &visited,
		   int pos, vector<int>& order);
void dfsComponent(const vector<vector<int> > &g, vector<char> &visited,
		   int pos, vector<int>& currentComponent);

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

std::vector<std::vector<int> > GraphSearch::mstKruskals(const std::vector<std::vector<int> > &g)
{
	vector < pair < int, pair<int,int> > > gList;

	gList = convertAdjMatrixWithWeights(g, 0);

	int n = g.size(), m = gList.size();
	int cost = 0;
	vector < pair<int,int> > resultList;
	vector < vector<int> > result(n, vector<int>(n, 0));

	sort (gList.begin(), gList.end());
	vector<int> tree_id (n);
	for (int i=0; i<n; ++i) {
		tree_id[i] = i;
	}
	for (int i=0; i<m; ++i) {
		int a = gList[i].second.first,  b = gList[i].second.second,  l = gList[i].first;
		if (tree_id[a] != tree_id[b]) {
			cost += l;
			resultList.push_back (make_pair (a, b));
			int old_id = tree_id[b],  new_id = tree_id[a];
			for (int j=0; j<n; ++j) {
				if (tree_id[j] == old_id) {
					tree_id[j] = new_id;
				}
			}
		}
	}

	for (const auto& item : resultList) {
		result[item.first][item.second] = result[item.second][item.first] = g[item.second][item.first];
	}

	return result;
}
vector< pair < int, pair<int,int> > > convertAdjMatrixWithWeights(const vector< vector<int> >& vec, const int& INF) {
	vector< pair< int, pair<int, int> > > results;
	for (int i = 0; i < vec.size(); ++i) {
		for (int j = i; j < vec.size(); ++j) {
			if (vec[i][j] != INF) {
				results.push_back(make_pair(vec[i][j], make_pair(i, j))); // weight, (v1, v2)
			}
		}
	}
	return results;
}

std::vector<std::vector<int> > GraphSearch::stronglyConnectedComponents(const std::vector<std::vector<int> > &gForwards, const std::vector<std::vector<int> > &gBackwards)
{
	int n = gForwards.size();
	vector<char> used;
	vector<int> order, currentComponent;
	vector< vector<int> > result;

	used.assign (n, false);
	for (int i = 0; i < n; ++i) {
		if (!used[i])
			dfsOrder(gForwards, used, i, order);
	}

	used.assign (n, false);
	for (int i = 0; i < n; ++i) {
		int v = order[n-1-i];
		if (!used[v]) {
			dfsComponent(gBackwards, used, v, currentComponent);
//			... вывод очередной component ...
			result.push_back(currentComponent);
			currentComponent.clear();
		}
	}
	return result;
}
void dfsOrder (const vector<vector<int> > &g, vector<char> &visited,
		   int pos, vector<int>& order) {
	visited[pos] = true;
	for (size_t i = 0; i < g[pos].size(); ++i) {
		if (!visited[g[pos][i]]) {
			dfsOrder (g, visited, g[pos][i], order);
		}
	}
	order.push_back (pos);
}
void dfsComponent (const vector<vector<int> > &g, vector<char> &visited,
		   int pos, vector<int>& currentComponent) {
	visited[pos] = true;
	currentComponent.push_back (pos);
	for (size_t i = 0; i < g[pos].size(); ++i) {
		if (!visited[g[pos][i]]) {
			dfsComponent (g, visited, g[pos][i], currentComponent);
		}
	}
}
