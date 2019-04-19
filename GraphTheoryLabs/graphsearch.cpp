#include "graphsearch.h"

#include <queue>

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
