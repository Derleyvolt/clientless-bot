#pragma once
#include <memory>
#include <thread>
#include "Socket.h"
#include "aux_function.h" 
#include "MD5Hash.h"
#include "Logging_config.h"
#include "Inventory.h"
#include "Party.h"
#include "Trade.h"
#include "Message.h"
#include "Basic_actions.h"
#include "Movement.h"
#include "Items.h"
#include "Craft.h"
#include "Script.h"
#include "Healing.h"
#include "general.h"

extern std::map<unsigned int, std::wstring> items_name;

class Player_structure {
public:

    void  connect();
    bool  config_socket(const char ip_address[], unsigned short port);
    void  set_credentials(Logging_config arg_credentials);

private:
    void  enter_world();
    void  keep_alive();
    void  send_request();
    void  recv_thread();

    SOCKET socket_descriptor;
    
    std::queue<std::vector<unsigned char>> data_queue;

    std::shared_ptr<Healing>        healing;
    std::shared_ptr<General>        general;
    std::shared_ptr<Items>          items;
    std::shared_ptr<Message>        message;
    std::shared_ptr<Trade>          trade;
    std::shared_ptr<Party>          party;
    std::shared_ptr<Inventory>      inventory;
    std::shared_ptr<Basic_actions>  basic_actions;
    std::shared_ptr<Movement>       movement;
    std::shared_ptr<Craft>          craft;
    std::shared_ptr<Script>         script; // classe destinada à criação de scripts específicos dentro do jogo

    Logging_config  credentials;
    MD5Hash         md5;
    Aux_function    aux;
    unsigned char   recv_buf[20000];

    // o nick do player nessa variável já vem com o dobro do bytes.. dps corrijo isso
    std::pair<unsigned int, std::string> player_basic_info;
    std::mutex mt;
};
