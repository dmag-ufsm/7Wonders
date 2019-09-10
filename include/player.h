#ifndef PLAYER_H
#define PLAYER_H

namespace DMAG {


class Player
{
private:
    int score;
public:
    Player();
    void CalculateScore();
    int GetScore();

};
}
#endif // PLAYER_H
