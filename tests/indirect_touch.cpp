#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

vector< vector<int> > arcs;
vector<bool> visited;
int start, finish;
bool found;

void indirect_touch_dfs(int idx)
{
    cout << "Visit: " << idx << endl;
    visited[idx] = true;
    if (idx == finish) {
        found = true;
        return;
    }
    for (int i: arcs[idx]) {
        if (!visited[i]) indirect_touch_dfs(i);
    }
    cout << "End visit: " << idx << endl;
}

bool indirect_touch()
{
    visited.clear();
    visited.resize(arcs.size());
    found = false;
    indirect_touch_dfs(start);
    return found;
}

int main()
{
    cout << "Indirect touch test" << endl;
    cin >> start >> finish;
    start--; finish--;
    arcs = vector< vector<int> >();
    visited = vector<bool>();
    while (1) {
        char ch; int i, j;
        cin >> ch >> i >> j;
        if (i == 0 && j == 0) break;
        if (i > arcs.size() || j > arcs.size()) {
            cout << "Resizing vector to " << max(i, j) << endl;
            arcs.resize(max(i, j));
        }
        i--; j--;
        if (ch == 'a') {
            arcs[i].push_back(j);
            arcs[j].push_back(i);
        } else if (ch == 'r') {
            auto p = find(arcs[i].begin(), arcs[i].end(), j);
            if (p != arcs[i].end()) arcs[i].erase(p);
            p = find(arcs[j].begin(), arcs[j].end(), i);
            if (p != arcs[j].end()) arcs[j].erase(p);
        } else {
            cout << "Invalid command. Only receive 'a' and 'r'." << endl;
        }
    }
    bool b = indirect_touch();
    cout << (b ? "Found" : "Not found") << endl;
    return 0;
}

