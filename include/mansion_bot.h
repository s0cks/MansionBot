#ifndef MANSIONBOT_MANSION_BOT_H
#define MANSIONBOT_MANSION_BOT_H

#include <map>
#include "timer.h"
#include "twitch.h"

class MansionBot{
private:
    std::map<std::string, Timer>* timerMap;

    MansionBot(){
        this->timerMap = new std::map<std::string, Timer>();
    }
    ~MansionBot(){
        delete this->timerMap;
    }
    MansionBot(const MansionBot& copy) = delete;
    void operator=(const MansionBot& copy) = delete;

    static MansionBot* instance;
public:
    static MansionBot*
    getInstance(){
        if(instance == nullptr){
            instance = new MansionBot();
        }

        return instance;
    }

    bool
    hasTimer(std::string str){
        return this->timerMap->find(str) != this->timerMap->end();
    }

    void
    addTimer(std::string str, Timer timer){
        this->timerMap->insert(std::make_pair(str, timer));
    }

    Timer*
    getTimer(std::string str){
        std::map<std::string, Timer>::iterator it = this->timerMap->find(str);
        if(it != this->timerMap->end()){
            return &it->second;
        } else{
            return nullptr;
        }
    }
};

#endif