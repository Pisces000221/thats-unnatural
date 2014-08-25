#include "level_reader.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

namespace level_reader {

map<string, level_objective> objective_str2enum;
map<string, int> bricktype_str2int;
vector<level> levels;

void init()
{
    // Initialize vectors
    levels = vector<level>();
    // Initialize the maps (I miss the taste to a sweeter life!)
    objective_str2enum = map<string, level_objective>();
    objective_str2enum["min_height"] = level_objective::MIN_HEIGHT;
    objective_str2enum["stacking"] = level_objective::MIN_HEIGHT;
    objective_str2enum["max_hitbricks"] = level_objective::MAX_HITBRICKS;
    bricktype_str2int = map<string, int>();
    bricktype_str2int["balls"] = 0;
    bricktype_str2int["circles"] = 0;
    bricktype_str2int["boxes"] = 1;
    bricktype_str2int["rects"] = 1;
    bricktype_str2int["rectangles"] = 1;
    bricktype_str2int["triangles"] = 2;

}

level get_level(int id)
{
    return levels.at(id - 1);
}

int level_count()
{
    return levels.size();
}

bool read_all()
{
    string filename = "data/levels.json";
    if (!FileUtils::getInstance()->isFileExist(filename)) {
        CCLOG("Level data file not found!");
        return false;
    }

    string file_str = FileUtils::getInstance()->getStringFromFile(filename);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(file_str.c_str());
    if (doc.HasParseError() || !doc.IsArray()) {
        CCLOG("Something unpleasant happened. Failed to read. T^T");
        if (doc.HasParseError()) CCLOG("  > Not a JSON file or internal problems with rapidjson.");
        else CCLOG("  > Incorrect format/fields. Contact the level designers.");
        return false;
    }

    levels.clear();
    for (unsigned int i = 0; i < doc.Size(); i++) {
        rapidjson::Value &v = doc[i];
        if (!v.HasMember("id") || !v.HasMember("objective")) continue;

        level l;
        l.id = v["id"].GetInt();
        l.objective = objective_str2enum[v["objective"].GetString()];
        for (int j = 0; j < BRICK_TYPE_COUNT; j++) l.brick_type_enabled[j] = false;
        if (v.HasMember("brick_types")) {
            rapidjson::Value &a = v["brick_types"];
            for (unsigned int j = 0; j < a.Size(); j++) {
                string s = a[j].GetString();
                l.brick_type_enabled[bricktype_str2int[s]] = true;
            }
        }

        switch (l.objective) {
        case level_objective::MIN_HEIGHT:
            l.min_height = v["arg"].GetDouble();
            break;
        case level_objective::MAX_HITBRICKS:
            l.max_hitbricks = v["arg"].GetInt();
            break;
        default:
            continue;
        }

        levels.push_back(l);
    }
    return true;
}

}

