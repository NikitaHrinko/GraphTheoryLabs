#ifndef GRAPHSEARCH_H
#define GRAPHSEARCH_H

#include <vector>

class GraphSearch
{
    std::vector<int> dfsSimple(const std::vector< std::vector<int> >& g, std::vector<bool>& visited,
                               int pos, std::vector<int>& res);

public:
    GraphSearch();
    std::vector<int> dfs(const std::vector< std::vector<int> >& g, int pos);
    std::vector<int> bfs(const std::vector< std::vector<int> >& g, int pos);

    std::vector< std::vector<int> > spanningTreePrim(const std::vector< std::vector<int> >& g);

    enum SearchMethod {
        DFS, BFS
    };
};

#endif // GRAPHSEARCH_H
