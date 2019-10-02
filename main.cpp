#include <iostream>
#include <card.h>
#include <player.h>
#include <deck.h>
#include <amqp.h> // The rabbit-mq broker library
#include <list>

using namespace std;
using namespace DMAG;

// define global variables
Deck d;
list<Player> player_list;

amqp_connection_state_t conn;

// Initiate global variables

void rabbit_start(){

    char const *hostname = "127.0.0.1";
    char const *exchange = "amq.direct";
    char const *binding = "Game_Control"; //This will be used to get messages from elsewhere
    int port = 5672;
    amqp_bytes_t queuename;

    {
        amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0,1,0,1,amqp_empty_table);
        queuename = amqp_bytes_malloc_dup(r->queue);
        if(queuename.bytes == 0){
            cerr<<"Failed to initialize connection with Rabbit-MQ"<<endl;
            exit(1);
        }
    }
    amqp_queue_bind(conn, 1, queuename, amqp_cstring_bytes(exchange),
                    amqp_cstring_bytes(binding), amqp_empty_table);
    amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0,
                                                                                       amqp_empty_table);

}
void rabbit_end(){
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
}


void game_init(int _players){
    d = Deck();
    for(int i = 0; i<_players; i++){
        player_list.push_back(Player());
    }


    return;
}

void game_close(){
    rabbit_end();

    // deallocate memory
}
void game_loop(){

    // create connection with RabbitMQ

    //show info

    //get commands

    //calculate stuff

    //end game?
}

int main()
{
   // game_init(3);
   // game_loop();
   // game_close();

    return 0;
}


