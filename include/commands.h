#ifndef MANSIONBOT_COMMANDS_H
#define MANSIONBOT_COMMANDS_H

#include <string>
#include "heph.h"
#include "mansion_bot.h"
#include "twitch.h"

static void
test(TwitchBot* bot, std::string sender, std::vector<std::string> params){
    std::cout << "Hello World" << std::endl;
    MansionBot* mansion = MansionBot::getInstance();
    if(mansion->hasTimer("test")){
        Timer* t = mansion->getTimer("test");
        if(t->elapsed() >= 30){
            bot->sendMessage("Hello World");
            t->start();
        }
    } else{
        Timer t = Timer();
        mansion->addTimer("test", t);
        t.start();
        bot->sendMessage("Hello World");
    }
}

static void
hello(TwitchBot* bot, std::string sender, std::vector<std::string> params){
    MansionBot* mansion = MansionBot::getInstance();
    if(mansion->hasTimer("hello")){
        Timer* t = mansion->getTimer("hello");
        if(t->elapsed() >= 30){
            bot->sendMessage(("Hello " + sender).c_str());
            t->start();
        }
    } else{
        Timer t = Timer();
        mansion->addTimer("hello", t);
        t.start();
        bot->sendMessage(("Hello " + sender).c_str());
    }
}

namespace commands{
    static void
    init(){
        commands::commands->insert(std::make_pair("test", &test));
        commands::commands->insert(std::make_pair("hello", &hello));
    }
}

#endif