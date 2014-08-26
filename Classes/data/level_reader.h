#ifndef __DATA_LEVEL_READER_H__
#define __DATA_LEVEL_READER_H__

#include <map>
#include <vector>
#include "Bricks.h"
using namespace std;

namespace level_reader {

typedef enum {
    MIN_HEIGHT,
    MAX_HITBRICKS, MAX_HITCOUNT,
    MIN_WEIGHT
} level_objective;

typedef struct {
    int id;
    level_objective objective;
    bool brick_type_enabled[bricks::BRICK_TYPE_COUNT];
    float tot_time;
    // Objectives
    // For stacking levels only
    double min_height;
    float endurance_time;
    // Hit bricks/count only
    int max_hit;
    // Weight mode only
    float min_weight;
} level;

extern map<string, level_objective> objective_str2enum;
extern map<string, int> bricktype_str2int;
extern vector<level> levels;

void init();
bool read_all();
level get_level(int);
int level_count();

}

#endif

