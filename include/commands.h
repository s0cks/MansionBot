#ifndef MANSIONBOT_COMMANDS_H
#define MANSIONBOT_COMMANDS_H

#include <string>
#include "heph.h"
#include "mansion_bot.h"
#include "twitch.h"

class Command{
protected:
    std::string name;
public:
    Command(std::string name){
        this->name = name;
    }
    virtual ~Command(){}

    virtual void
    invoke(std::string sender){}
};

class HephOnline : public Command{
private:
    void
    sendOnlineStatus(TwitchBot bot){
        Heph* heph = Heph::getInstance();
        if(heph->online()){
            bot.sendMessage("Heph is online");
        } else{
            bot.sendMessage("Heph isn't online");
        }
    }
public:
    HephOnline() : Command("online"){}
    ~HephOnline(){}

    void
    invoke(TwitchBot twitchBot, std::string sender){
        if(sender.compare("asyncronous") || sender.compare("heph")){
            MansionBot* bot = MansionBot::getInstance();
            if(bot->hasTimer("online")){
                Timer* timer = bot->getTimer("online");
                if(timer->elapsed() >= 30){
                    this->sendOnlineStatus(twitchBot);
                    timer->start();
                }
            } else{
                Timer t = Timer();
                bot->addTimer("online", t);
                this->sendOnlineStatus(twitchBot);
                t.start();
            }
        }
    }
};

#endif