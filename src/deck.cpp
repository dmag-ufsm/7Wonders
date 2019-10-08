#include <deck.h>
#include <stack>

namespace DMAG{

Deck::Deck()
{

    // Populate the deck with the starting cards
    // this->_deck.push_back(Card(4, "First", 2, 2)) ;
    // this->_deck.push_back(Card(4, "Second", 2, 2)) ;
    // this->_deck.push_back(Card(4, "Third", 2, 2)) ;
    // this->_deck.push_back(Card(4, "fourth", 2, 2)) ;
    // this->_deck.push_back(Card(4, "Fifth", 2, 2)) ;
    // this->_deck.push_back(Card(4, "sixth", 2, 2)) ;
    // this->_deck.push_back(Card(4, "seventh", 2, 2)) ;
    // this->_deck.push_back(Card(4, "eight", 2, 2)) ;
    // this->_deck.push_back(Card(4, "Ninth", 2, 2)) ;
    // this->_deck.push_back(Card(4, "Le ten", 2, 2)) ;

    /*
     * ToDo: populate the initial deck, which will be used
     * in the game setup phase.
     */
}

void Deck::Shuffle(){

}

void Deck::IncludeGuild(){

    //add the cards

    // and then shuffle
    this->Shuffle();

}

Card Deck::GetTop(){
    Card c = this->_deck.back();
    this->_deck.pop_back();
    return c;
}

bool Deck::IsEmpty(){
    return this->_deck.empty();
}
}
