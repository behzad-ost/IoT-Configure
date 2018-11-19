#ifndef __LOGIN__
#define __LOGIN__

#include "../utils/request.hpp"
#include "../utils/response.hpp"
#include "../config/config"

class RequestHandler;

class LoginHandler : public RequestHandler {
public:
  Response *callback(Request *req) {
    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");
    if(username != ADMIN_USERNAME || password != ADMIN_PASSWORD){
        Response *res = Response::redirect("/login");
        return res;    
    }
    Response *res = Response::redirect("/config");
    res->setSessionId(TOKEN);
    return res;
  }
};

#endif