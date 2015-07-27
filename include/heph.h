#ifndef MANSIONBOT_HEPH_H
#define MANSIONBOT_HEPH_H

#include <rapidjson/document.h>
#include "http_client.h"

class Heph{
private:
    std::string json;

    Heph(){}
    ~Heph(){}
    Heph(const Heph& copy) = delete;
    void operator=(const Heph&) = delete;

    static Heph* instance;
public:
    static Heph*
    getInstance(){
        if(instance == nullptr){
            instance = new Heph();
        }

        return instance;
    }

    bool
    online(){
        this->json = HttpRequest().get("https://api.twitch.tv/kraken/streams/heph");
        rapidjson::Document doc;
        doc.Parse(this->json.c_str());
        rapidjson::Value& stream = doc["stream"];
        return !stream.IsNull();
    }
};

#endif