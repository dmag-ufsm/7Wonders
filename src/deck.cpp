#include <deck.h>

namespace DMAG{

Deck::Deck()
{

    // Populate the deck with the starting cards
    this->_deck.push_front(Card(4, "First", 2, 2)) ;
    this->_deck.push_front(Card(4, "Second", 2, 2)) ;
    this->_deck.push_front(Card(4, "Third", 2, 2)) ;
    this->_deck.push_front(Card(4, "fourth", 2, 2)) ;
    this->_deck.push_front(Card(4, "Fifth", 2, 2)) ;
    this->_deck.push_front(Card(4, "sixth", 2, 2)) ;
    this->_deck.push_front(Card(4, "seventh", 2, 2)) ;
    this->_deck.push_front(Card(4, "eight", 2, 2)) ;
    this->_deck.push_front(Card(4, "Ninth", 2, 2)) ;
    this->_deck.push_front(Card(4, "Le ten", 2, 2)) ;
}

void Deck::Shuffle(){

}

void Deck::IncludeGuild(){

    //add the cards

    // and then shuffle
    this->Shuffle();

}

int Deck::GetPoints(){
   int _points = 0;
   for(std::list<Card>::iterator it = _deck.begin(); it != _deck.end(); ++it){
       _points += (*it).GetValue();
   }
   return _points;
}

Card Deck::GetTop(){
    Card c = this->_deck.front();
    this->_deck.pop_front();
    return c;
}

bool Deck::IsEmpty(){
    return this->_deck.empty();
}

void Deck::Insert(Card _c) {
    this->_deck.push_front(_c);
}
}
