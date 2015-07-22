#ifndef KATARINA2_HTTP_CLIENT_H
#define KATARINA2_HTTP_CLIENT_H

#include <curl/curl.h>
#include <sstream>
#include <string>
#include <stdexcept>

class HttpRequest{
private:
    CURL* curl;
    long code;
    std::stringstream ss;

    size_t
    write (void* buffer, size_t size, size_t nmemb){
        this->ss.write((const char*) buffer, size * nmemb);
        return size * nmemb;
    }

    static size_t
    writeData(void* buffer, size_t size, size_t nmemb, void* userp){
        return static_cast<HttpRequest*>(userp)->write(buffer, size, nmemb);
    }
public:
    HttpRequest(){
        this->curl = curl_easy_init();
        this->code = 0;
    }
    ~HttpRequest(){
        if(this->curl){
            curl_easy_cleanup(this->curl);
        }
    }

    std::string
    get(const std::string& url){
        CURLcode res;
        curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, this);

        this->ss.str("");
        this->code = 0;
        res = curl_easy_perform(this->curl);
        if(res != CURLE_OK){
            throw std::runtime_error(curl_easy_strerror(res));
        }

        curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &code);
        return this->ss.str();
    }

    long
    getHttpCode(){
        return this->code;
    }
};

#endif