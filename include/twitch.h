#ifndef MANSIONBOT_TWITCH_H
#define MANSIONBOT_TWITCH_H

#include <string>
#include <iostream>
#include "http_client.h"
#include "rapidjson/document.h"

class Heph{
private:
    std::string json;
public:
    Heph(){}
    ~Heph(){}

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