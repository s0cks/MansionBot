#ifndef MANSIONBOT_MANSION_BOT_H
#define MANSIONBOT_MANSION_BOT_H

#include <map>
#include "timer.h"
#include "commands.h"

class MansionBot{
private:
    std::map<std::string, Timer>* timerMap;
    std::map<std::string, Command>* cmdMap;

    MansionBot(){
        this->timerMap = new std::map<std::string, Timer>();
        this->cmdMap = new std::map<std::string, Command>();

        this->cmdMap->insert(std::make_pair("online", HephOnline()));
    }
    ~MansionBot(){
        delete this->cmdMap;
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

    bool
    hasCommand(std::string str){
        return this->cmdMap->find(str) != this->cmdMap->end();
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

    Command*
    getCommand(std::string str){
        std::map<std::string, Command>::iterator it = this->cmdMap->find(str);
        if(it != this->cmdMap->end()){
            return &it->second;
        } else{
            return nullptr;
        }
    }
};

#endif