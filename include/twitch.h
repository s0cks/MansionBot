#ifndef MANSIONBOT_TWITCH_H
#define MANSIONBOT_TWITCH_H

#include <string>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include "http_client.h"
#include "rapidjson/document.h"
#include "mansion_bot.h"
#include <map>
#include <time.h>
#include <vector>
#include <algorithm>

#define MAXDATA_SIZE 256

class TwitchBot;

typedef void (*CommandFunc)(TwitchBot*, std::string, std::vector<std::string> params);

namespace commands{
    static std::map<std::string, CommandFunc>* commands = new std::map<std::string, CommandFunc>();
}

class TwitchBot{
private:
    int sd;
    std::string nick;
    std::string token;

    static std::string heph;
    static std::string async;

    bool
    sendData(const char* msg){
        size_t len = strlen(msg);
        ssize_t sent = send(this->sd, msg, len, 0);
        std::cout << "> " << msg << std::endl;
        return sent != 0;
    }

    bool
    charSearch(char* toSearch, const char* searchFor){
        size_t len = strlen(toSearch);
        size_t forLen = strlen(searchFor);

        for(int i = 0; i < len; i++){
            if(searchFor[0] == toSearch[i]){
                bool found = true;
                for(int x = 1; x < forLen; x++){
                    if(toSearch[i + x] != searchFor[x]){
                        found = false;
                        break;
                    }
                }

                if(found){
                    return true;
                }
            }
        }

        return false;
    }

    void
    sendPong(char* buff){
        char* toSearch = "PING ";
        for(int i = 0; i < strlen(buff); i++){
            if(buff[i] == toSearch[0]){
                bool found = true;
                for(int x = 1; x < 4; x++){
                    if(buff[i + x] != toSearch[x]){
                        found = false;
                        break;
                    }
                }

                if(found){
                    int count = 0;
                    for(size_t x = (i + strlen(toSearch)); x < strlen(buff); x++){
                        count++;
                    }

                    char returnHost[count + 5];
                    returnHost[0] = 'P';
                    returnHost[1] = 'O';
                    returnHost[2] = 'N';
                    returnHost[3] = 'G';
                    returnHost[4] = ' ';

                    count = 0;
                    for(size_t x = (i + strlen(toSearch)); x < strlen(buff); x++){
                        returnHost[count + 5] = buff[x];
                        count++;
                    }

                    if(sendData(returnHost)){
                        std::cout << "Ping Pong" << std::endl;
                    }

                    return;
                }
            }
        }
    }

    std::vector<std::string>&
    split(const std::string& s, char delim, std::vector<std::string>& elems){
        std::string item;
        std::stringstream ss(s);
        while(std::getline(ss, item, delim)){
            elems.push_back(item);
        }
        return elems;
    }

    std::vector<std::string>
    split(const std::string& s, char delim){
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
public:
    TwitchBot(const std::string& nick, const std::string token){
        this->nick = nick;
        this->token = token;
    }
    ~TwitchBot(){
        close(this->sd);
    }

    void
    start(const std::string port){
        struct addrinfo hints, *servInfo;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        printf("Getting addrinfo\n");

        int res;
        if((res = getaddrinfo("irc.twitch.tv", port.c_str(), &hints, &servInfo)) != 0){
            fprintf(stderr, "[connect] getaddrinfo: %s\n", gai_strerror(res));
            return;
        }

        printf("creating socket\n");

        if((this->sd = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol)) == -1){
            perror("client: socket");
        }

        printf("Connecting\n");

        if(connect(this->sd, servInfo->ai_addr, servInfo->ai_addrlen) == -1){
            close(this->sd);
            perror("client: connect");
        }

        printf("Connected\n");

        ssize_t readBytes;
        char buff[MAXDATA_SIZE];

        this->sendData(("PASS " + this->token + "\r\n").c_str());
        this->sendData(("NICK " + this->nick + "\r\n").c_str());
        this->sendData(("USER " + this->nick + " 8 * : " + this->nick + "\r\n").c_str());

        int count = 0;
        while(true){
            count++;
            switch(count){
                case 3:{
                    this->sendData("JOIN #heph\r\n");
                    break;
                }
                default:{
                    break;
                }
            }

            readBytes = read(this->sd, buff, MAXDATA_SIZE - 1);
            buff[readBytes] = '\0';
            std::cout << buff;

            std::string readStr(buff);

            if(charSearch(buff, "PING")){
                sendPong(buff);
            } else if((readStr.substr(1, heph.size()) == heph || readStr.substr(1, async.size()) == async)){
                std::string msg = readStr.substr(readStr.find_last_of(":") + 1);
                if(msg.find(' ') != std::string::npos){
                    std::vector<std::string> tokens = split(msg, ' ');
                    std::string cmd = tokens.front().substr(1, tokens.front().size() - 1);
                    std::map<std::string, CommandFunc>::iterator iter = commands::commands->find(cmd);
                    if(iter != commands::commands->end()){
                        std::vector<std::string> params;
                        std::transform(tokens.begin() + 1, tokens.end(), std::back_inserter(params), [](const std::string& s){ return s; });
                        (*iter->second)(this, readStr.substr(1, readStr.find_first_of('!')), params);
                    }
                } else{
                    std::map<std::string, CommandFunc>::iterator iter = commands::commands->find(msg);
                    if(iter != commands::commands->end()){
                        std::vector<std::string> params;
                        (*iter->second)(this, readStr.substr(1, readStr.find_first_of('!')), params);
                    }
                }
            }

        if(readBytes == 0){
            std::cout << "------------- Connection Closed --------------" << std::endl;
            break;
        }
    }
}

void
sendMessage(const char* msg){
    std::stringstream stream;
    stream << "PRIVMSG #heph :" << msg << "\r\n";
        this->sendData(stream.str().c_str());
    }
};

#endif