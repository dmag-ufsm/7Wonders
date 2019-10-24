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

Gizah_a::Gizah_a() {
    this->id = WONDER_ID::gizah_a;
    this->name = "Gizah A";
    this->production = RESOURCE::stone; // Initial production.
    this->effects = {EFFECT::vp, EFFECT::vp, EFFECT::vp};
    this->resources[RESOURCE::stone] = 2; // Required resources to build stage 1;
                                          // will change on AddStage().
}

// This will probably take Player as a parameter.
void Gizah_a::AddStage(){

}

// GIZAH B



// BABYLON A



// BABYLON B



// OLYMPIA A



// OLYMPIA B



// RHODOS A



// RHODOS B



// EPHESOS A



// EPHESOS B



// ALEXANDRIA A



// ALEXANDRIA B



// HALIKARNASSOS A



// HALIKARNASSOS B

}
