#include "indirect_touch.h"
#include <algorithm>
#include <vector>
using namespace std;

namespace indirect_touch {

vector< vector<int> > arcs;
vector<bool> visited;
int start, finish;
bool found;

void init()
{
    arcs = vector< vector<int> >();
    visited = vector<bool>();
}

void indirect_touch_dfs(int idx)
{
    visited[idx] = true;
    if (idx == finish) {
        found = true;
        return;
    }
    for (int i: arcs[idx]) {
        if (!visited[i]) indirect_touch_dfs(i);
    }
}

bool calculate()
{
    visited.clear();
    visited.resize(arcs.size());
    found = false;
    indirect_touch_dfs(start);
    return found;
}

void add_arc(int i, int j)
{
    if (max(i, j) > (int)arcs.size() - 1) arcs.resize(max(i, j) + 1);
    arcs[i].push_back(j);
    arcs[j].push_back(i);
}

void remove_arc(int i, int j)
{
    auto p = find(arcs[i].begin(), arcs[i].end(), j);
    if (p != arcs[i].end()) arcs[i].erase(p);
    p = find(arcs[j].begin(), arcs[j].end(), i);
    if (p != arcs[j].end()) arcs[j].erase(p);
}

void remove_all_arcs(int vert)
{
    for (int i: arcs[vert]) {
        auto p = find(arcs[i].begin(), arcs[i].end(), vert);
        if (p != arcs[i].end()) arcs[i].erase(p);
    }
    arcs[vert].clear();
}

}

