#ifndef __ALGO_INDIRECT_TOUCH_H__
#define __ALGO_INDIRECT_TOUCH_H__

#include <vector>
using namespace std;

namespace indirect_touch {

extern vector< vector<int> > arcs;
extern vector<bool> visited;
extern int start, finish;
extern bool found;

void init();
bool calculate();
void add_arc(int, int);
void remove_arc(int, int);
void remove_all_arcs(int);

}

#endif

