#include <wonder.h>
#include <player.h>

namespace DMAG {

Wonder::Wonder()
{
    this->stage = 0;
    this->wonder_points = 0;
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

int Wonder::GetWonderPoints() const{
    return this->wonder_points;
}

std::vector<int> Wonder::GetEffects() const{
    return this->effects;
}

bool Wonder::AddStage(Player* p) { return false; }

// TODO:
// Still need to figure out how we'll apply each wonder's special effects.
// Will we doi it through here or directly through player?

// GIZAH A

Gizah_a::Gizah_a(){
    this->id = WONDER_ID::gizah_a;
    this->name = "Gizah A";
    this->production = RESOURCE::stone; // Initial production.
    this->effects = {EFFECT::vp, EFFECT::vp, EFFECT::vp}; // Each stage effect.
    this->cost[RESOURCE::stone] = 2; // Required resources to build stage 1;
                                     // will change on AddStage().
}

bool Gizah_a::AddStage(Player *p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::wood] = 3;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            this->wonder_points += 5;
            this->cost[RESOURCE::stone] = 4;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// GIZAH B

Gizah_b::Gizah_b(){
    this->id = WONDER_ID::gizah_b;
    this->name = "Gizah B";
    this->production = RESOURCE::stone;
    this->effects = {EFFECT::vp, EFFECT::vp, EFFECT::vp, EFFECT::vp};
    this->cost[RESOURCE::wood] = 2;
}

bool Gizah_b::AddStage(Player *p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::stone] = 3;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 5;
            this->cost[RESOURCE::clay] = 3;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) {
            this->stage++;
            this->wonder_points += 5;
            this->cost[RESOURCE::stone] = 4;
            this->cost[RESOURCE::papyrus] = 1;
            stage_built = true;
        } break;

    // Building stage 4
    case 4:
        if ((p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) &&
            (p_resources[RESOURCE::papyrus] >= this->cost[RESOURCE::papyrus])) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// BABYLON A

Babylon_a::Babylon_a(){
    this->id = WONDER_ID::babylon_a;
    this->name = "Babylon A";
    this->production = RESOURCE::clay;
    this->effects = {EFFECT::vp, EFFECT::add_science, EFFECT::vp};
    this->cost[RESOURCE::clay] = 2;
}

bool Babylon_a::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::wood] = 3;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            // TODO: Gives 1x Compass OR 1x Gear OR 1x Tablet
            this->cost[RESOURCE::clay] = 4;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// BABYLON B

Babylon_b::Babylon_b(){
    this->id = WONDER_ID::babylon_b;
    this->name = "Babylon B";
    this->production = RESOURCE::clay;
    this->effects = {EFFECT::vp, EFFECT::play_seventh_card, EFFECT::add_science};
    this->cost[RESOURCE::clay] = 1;
    this->cost[RESOURCE::cloth] = 1;
}

bool Babylon_b::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if ((p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) &&
            (p_resources[RESOURCE::cloth] >= this->cost[RESOURCE::cloth])) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::glass] = 1;
            this->cost[RESOURCE::wood] = 2;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if ((p_resources[RESOURCE::glass] >= this->cost[RESOURCE::glass]) &&
            (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood])) {
            this->stage++;
            // TODO: Play seventh card instead of discarding it.
            this->cost[RESOURCE::clay] = 3;
            this->cost[RESOURCE::papyrus] = 1;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if ((p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) &&
            (p_resources[RESOURCE::papyrus] >= this->cost[RESOURCE::papyrus])) {
            this->stage++;
            // TODO: Gives 1x Compass OR 1x Gear OR 1x Tablet
            stage_built = true;
        } break;
    }

    return stage_built;
}

// OLYMPIA A

Olympia_a::Olympia_a(){
    this->id = WONDER_ID::olympia_a;
    this->name = "Olympia A";
    this->production = RESOURCE::wood;
    this->effects = {EFFECT::vp, EFFECT::hand_build_for_free, EFFECT::vp};
    this->cost[RESOURCE::wood] = 2;
}

bool Olympia_a::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::stone] = 2;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            // TODO: Once per Age, build a structure for free.
            this->cost[RESOURCE::ore] = 2;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::ore] >= this->cost[RESOURCE::ore]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// OLYMPIA B

Olympia_b::Olympia_b(){
    this->id = WONDER_ID::olympia_b;
    this->name = "Olympia B";
    this->production = RESOURCE::wood;
    this->effects = {EFFECT::buy_raw_cheap, EFFECT::vp, EFFECT::copy_guild};
    this->cost[RESOURCE::wood] = 2;
}

bool Olympia_b::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            // TODO: Raw materials in neighboring cities for 1x Coin.
            this->cost[RESOURCE::stone] = 2;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 5;
            this->cost[RESOURCE::ore] = 2;
            this->cost[RESOURCE::cloth] = 1;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if ((p_resources[RESOURCE::ore] >= this->cost[RESOURCE::ore]) &&
            (p_resources[RESOURCE::cloth] >= this->cost[RESOURCE::cloth])) {
            this->stage++;
            // TODO: Copy a Guild in the neighboring cities.
            stage_built = true;
        } break;
    }

    return stage_built;
}

// RHODOS A

Rhodos_a::Rhodos_a(){
    this->id = WONDER_ID::rhodos_a;
    this->name = "Rhodos A";
    this->production = RESOURCE::ore;
    this->effects = {EFFECT::vp, EFFECT::add_shields, EFFECT::vp};
    this->cost[RESOURCE::wood] = 2;
}

bool Rhodos_a::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::clay] = 3;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) {
            this->stage++;
            p->AddShield(2);
            this->cost[RESOURCE::ore] = 4;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::ore] >= this->cost[RESOURCE::ore]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// RHODOS B

Rhodos_b::Rhodos_b(){
    this->id = WONDER_ID::rhodos_b;
    this->name = "Rhodos B";
    this->production = RESOURCE::ore;
    this->effects = {EFFECT::add_shield_vp_coin, EFFECT::add_shield_vp_coin};
    this->cost[RESOURCE::stone] = 3;
}

bool Rhodos_b::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 3;
            p->AddShield(1);
            p->AddResource(RESOURCE::coins, 3);
            this->cost[RESOURCE::ore] = 4;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::ore] >= this->cost[RESOURCE::ore]) {
            this->stage++;
            this->wonder_points += 4;
            p->AddShield(1);
            p->AddResource(RESOURCE::coins, 4);
            stage_built = true;
        } break;
    }

    return stage_built;
}

// EPHESOS A

Ephesos_a::Ephesos_a(){
    this->id = WONDER_ID::ephesos_a;
    this->name = "Ephesos A";
    this->production = RESOURCE::papyrus;
    this->effects = {EFFECT::vp, EFFECT::add_coins, EFFECT::vp};
    this->cost[RESOURCE::stone] = 2;
}

bool Ephesos_a::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::wood] = 2;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            p->AddResource(RESOURCE::coins, 9);
            this->cost[RESOURCE::papyrus] = 2;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::papyrus] >= this->cost[RESOURCE::papyrus]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// EPHESOS B

Ephesos_b::Ephesos_b(){
    this->id = WONDER_ID::ephesos_b;
    this->name = "Ephesos B";
    this->production = RESOURCE::papyrus;
    this->effects = {EFFECT::add_vp_coin, EFFECT::add_vp_coin, EFFECT::add_vp_coin};
    this->cost[RESOURCE::stone] = 2;
}

bool Ephesos_b::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 2;
            p->AddResource(RESOURCE::coins, 4);
            this->cost[RESOURCE::wood] = 2;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            this->wonder_points += 3;
            p->AddResource(RESOURCE::coins, 4);
            this->cost[RESOURCE::glass] = 1;
            this->cost[RESOURCE::cloth] = 1;
            this->cost[RESOURCE::papyrus] = 1;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if ((p_resources[RESOURCE::glass] >= this->cost[RESOURCE::glass]) &&
            (p_resources[RESOURCE::cloth] >= this->cost[RESOURCE::cloth]) &&
            (p_resources[RESOURCE::papyrus] >= this->cost[RESOURCE::papyrus])) {
            this->stage++;
            this->wonder_points += 5;
            p->AddResource(RESOURCE::coins, 4);
            stage_built = true;
        } break;
    }

    return stage_built;
}

// ALEXANDRIA A

Alexandria_a::Alexandria_a(){
    this->id = WONDER_ID::alexandria_a;
    this->name = "Alexandria A";
    this->production = RESOURCE::glass;
    this->effects = {EFFECT::vp, EFFECT::choose_raw, EFFECT::vp};
    this->cost[RESOURCE::stone] = 2;
}

bool Alexandria_a::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::ore] = 2;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::ore] >= this->cost[RESOURCE::ore]) {
            this->stage++;
            // TODO: Gives 1x Clay OR 1x Ore OR 1x Wood OR 1x Stone.
            this->cost[RESOURCE::glass] = 2;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::glass] >= this->cost[RESOURCE::glass]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// ALEXANDRIA B

Alexandria_b::Alexandria_b(){
    this->id = WONDER_ID::alexandria_b;
    this->name = "Alexandria B";
    this->production = RESOURCE::glass;
    this->effects = {EFFECT::choose_raw, EFFECT::choose_manuf, EFFECT::vp};
    this->cost[RESOURCE::clay] = 2;
}

bool Alexandria_b::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) {
            this->stage++;
            // TODO: Gives 1x Clay OR 1x Ore OR 1x Wood or 1x Stone.
            this->cost[RESOURCE::wood] = 2;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::wood] >= this->cost[RESOURCE::wood]) {
            this->stage++;
            // TODO: Gives 1x Glass OR 1x Cloth OR 1x Papyrus.
            this->cost[RESOURCE::stone] = 3;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::stone] >= this->cost[RESOURCE::stone]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// HALIKARNASSOS A

Halikarnassos_a::Halikarnassos_a(){
    this->id = WONDER_ID::halikarnassos_a;
    this->name = "Halikarnassos A";
    this->production = RESOURCE::cloth;
    this->effects = {EFFECT::vp, EFFECT::discard_build_for_free, EFFECT::vp};
    this->cost[RESOURCE::clay] = 2;
}

bool Halikarnassos_a::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) {
            this->stage++;
            this->wonder_points += 3;
            this->cost[RESOURCE::ore] = 3;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::ore] >= this->cost[RESOURCE::ore]) {
            this->stage++;
            // TODO: Look through all the discarded cards and take one.
            this->cost[RESOURCE::cloth] = 2;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if (p_resources[RESOURCE::cloth] >= this->cost[RESOURCE::cloth]) {
            this->stage++;
            this->wonder_points += 7;
            stage_built = true;
        } break;
    }

    return stage_built;
}

// HALIKARNASSOS B

Halikarnassos_b::Halikarnassos_b(){
    this->id = WONDER_ID::halikarnassos_b;
    this->name = "Halikarnassos B";
    this->production = RESOURCE::cloth;
    this->effects = {EFFECT::add_vp_discard, EFFECT::add_vp_discard, EFFECT::discard_build_for_free};
    this->cost[RESOURCE::ore] = 2;
}

bool Halikarnassos_b::AddStage(Player* p){
    std::map<int, unsigned char> p_resources = p->GetResources();
    bool stage_built = false;

    switch (this->stage) {
    // Building stage 1
    case 0:
        if (p_resources[RESOURCE::ore] >= this->cost[RESOURCE::ore]) {
            this->stage++;
            this->wonder_points += 2;
            // TODO: Look through discarded cards and take one.
            this->cost[RESOURCE::clay] = 3;
            stage_built = true;
        } break;

    // Building stage 2
    case 1:
        if (p_resources[RESOURCE::clay] >= this->cost[RESOURCE::clay]) {
            this->stage++;
            this->wonder_points += 1;
            // TODO: Look through all the discarded cards and take one.
            this->cost[RESOURCE::glass] = 1;
            this->cost[RESOURCE::cloth] = 1;
            this->cost[RESOURCE::papyrus] = 1;
            stage_built = true;
        } break;

    // Building stage 3
    case 2:
        if ((p_resources[RESOURCE::glass] >= this->cost[RESOURCE::glass]) &&
            (p_resources[RESOURCE::cloth] >= this->cost[RESOURCE::cloth]) &&
            (p_resources[RESOURCE::papyrus] >= this->cost[RESOURCE::papyrus])) {
            this->stage++;
            // TODO: Look through all the discarded cards and take one.
            stage_built = true;
        } break;
    }

    return stage_built;
}

}
