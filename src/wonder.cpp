#include <wonder.h>

namespace DMAG {

Wonder::Wonder(){
    this->stage = 0;
}

std::string Wonder::GetName() const{
    return this->name;
}

int Wonder::GetType() const{
    return this->id;
}

int Wonder::GetProduction() const{
    return this->production;
}

int Wonder::GetStage() const{
    return this->stage;
}

std::vector<int> Wonder::GetEffects() const{
    return this->effects;
}

void Wonder::AddStage() {}

// GIZAH A

Gizah_a::Gizah_a(){
    this->id = WONDER_ID::gizah_a;
    this->name = "Gizah A";
    this->production = RESOURCE::stone; // Initial production.
    this->effects = {EFFECT::vp, EFFECT::vp, EFFECT::vp}; // Each stage effect.
    this->cost[RESOURCE::stone] = 2; // Required resources to build stage 1;
                                     // will change on AddStage().
}

// This will probably take Player as a parameter.
void Gizah_a::AddStage(){
    // TODO
    // Update this->cost to reflect the cost to build the next stage;
    // Apply this stage's effects (here or in Player, to decide);
    // this->stage++.
}

// GIZAH B

Gizah_b::Gizah_b(){
    this->id = WONDER_ID::gizah_b;
    this->name = "Gizah B";
    this->production = RESOURCE::stone;
    this->effects = {EFFECT::vp, EFFECT::vp, EFFECT::vp, EFFECT::vp};
    this->cost[RESOURCE::wood] = 2;
}

void Gizah_b::AddStage(){

}

// BABYLON A

Babylon_a::Babylon_a(){
    this->id = WONDER_ID::babylon_a;
    this->name = "Babylon A";
    this->production = RESOURCE::clay;
    //this->effects = {};
    this->cost[RESOURCE::clay] = 2;
}

void Babylon_a::AddStage(){

}

// BABYLON B

Babylon_b::Babylon_b(){
    this->id = WONDER_ID::babylon_b;
    this->name = "Babylon B";
    this->production = RESOURCE::clay;
    //this->effects = {};
    this->cost[RESOURCE::clay] = 1;
    this->cost[RESOURCE::cloth] = 1;
}

void Babylon_b::AddStage(){

}

// OLYMPIA A

Olympia_a::Olympia_a(){
    this->id = WONDER_ID::olympia_a;
    this->name = "Olympia A";
    this->production = RESOURCE::wood;
    //this->effects = {};
    this->cost[RESOURCE::wood] = 2;
}

void Olympia_a::AddStage(){

}

// OLYMPIA B

Olympia_b::Olympia_b(){
    this->id = WONDER_ID::olympia_b;
    this->name = "Olympia B";
    this->production = RESOURCE::wood;
    //this->effects = {};
    this->cost[RESOURCE::wood] = 2;
}

void Olympia_b::AddStage(){

}

// RHODOS A

Rhodos_a::Rhodos_a(){
    this->id = WONDER_ID::rhodos_a;
    this->name = "Rhodos A";
    this->production = RESOURCE::ore;
    //this->effects = {};
    this->cost[RESOURCE::wood] = 2;
}

void Rhodos_a::AddStage(){

}

// RHODOS B

Rhodos_b::Rhodos_b(){
    this->id = WONDER_ID::rhodos_b;
    this->name = "Rhodos B";
    this->production = RESOURCE::ore;
    //this->effects = {};
    this->cost[RESOURCE::stone] = 3;
}

void Rhodos_b::AddStage(){

}

// EPHESOS A

Ephesos_a::Ephesos_a(){
    this->id = WONDER_ID::ephesos_a;
    this->name = "Ephesos A";
    this->production = RESOURCE::papyrus;
    //this->effects = {};
    this->cost[RESOURCE::stone] = 2;
}

void Ephesos_a::AddStage(){

}

// EPHESOS B

Ephesos_b::Ephesos_b(){
    this->id = WONDER_ID::ephesos_b;
    this->name = "Ephesos B";
    this->production = RESOURCE::papyrus;
    //this->effects = {};
    this->cost[RESOURCE::stone] = 2;
}

void Ephesos_b::AddStage(){

}

// ALEXANDRIA A

Alexandria_a::Alexandria_a(){
    this->id = WONDER_ID::alexandria_a;
    this->name = "Alexandria A";
    this->production = RESOURCE::glass;
    //this->effects = {};
    this->cost[RESOURCE::stone] = 2;
}

void Alexandria_a::AddStage(){

}

// ALEXANDRIA B

Alexandria_b::Alexandria_b(){
    this->id = WONDER_ID::alexandria_b;
    this->name = "Alexandria B";
    this->production = RESOURCE::glass;
    //this->effects = {};
    this->cost[RESOURCE::clay] = 2;
}

void Alexandria_b::AddStage(){

}

// HALIKARNASSOS A

Halikarnassos_a::Halikarnassos_a(){
    this->id = WONDER_ID::halikarnassos_a;
    this->name = "Halikarnassos A";
    this->production = RESOURCE::cloth;
    //this->effects = {};
    this->cost[RESOURCE::clay] = 2;
}

void Halikarnassos_a::AddStage(){

}

// HALIKARNASSOS B

Halikarnassos_b::Halikarnassos_b(){
    this->id = WONDER_ID::halikarnassos_b;
    this->name = "Halikarnassos B";
    this->production = RESOURCE::cloth;
    //this->effects = {};
    this->cost[RESOURCE::ore] = 2;
}

void Halikarnassos_b::AddStage(){

}

}
