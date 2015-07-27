#include <iostream>
#include <twitch.h>
#include "heph.h"
#include "mansion_bot.h"
#include "commands.h"

Heph* Heph::instance = nullptr;
MansionBot* MansionBot::instance = nullptr;

std::string TwitchBot::async = "asyncronous";
std::string TwitchBot::heph = "heph";

int main(int argc, char** argv){
    commands::init();
    TwitchBot* bot = new TwitchBot("mansionbot", "oauth:7n6t92uunc6h380590zt1guiuev28l");
    bot->start("6667");
    delete bot;
    return 0;
}