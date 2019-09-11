#include <deck.h>
#include <stack>

namespace DMAG{

Deck::Deck()
{

    // Populate the deck with the starting cards
    this->_deck.push(Card(4, "First", 2, 2)) ;
    this->_deck.push(Card(4, "Second", 2, 2)) ;
    this->_deck.push(Card(4, "Third", 2, 2)) ;
    this->_deck.push(Card(4, "fourth", 2, 2)) ;
    this->_deck.push(Card(4, "Fifth", 2, 2)) ;
    this->_deck.push(Card(4, "sixth", 2, 2)) ;
    this->_deck.push(Card(4, "seventh", 2, 2)) ;
    this->_deck.push(Card(4, "eight", 2, 2)) ;
    this->_deck.push(Card(4, "Ninth", 2, 2)) ;
    this->_deck.push(Card(4, "Le ten", 2, 2)) ;
}

void Deck::Shuffle(){

}

void Deck::IncludeGuild(){

}

Card Deck::GetTop(){
    Card c = this->_deck.top();
    this->_deck.pop();
    return c;
}

bool Deck::IsEmpty(){
    return this->_deck.empty();
}
}
