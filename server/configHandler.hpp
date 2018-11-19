#ifndef __CONFIG__
#define __CONFIG__

#include <fstream>
#include "../utils/request.hpp"
#include "../utils/response.hpp"
#include "../config/config"

std::vector<std::pair<std::string,std::string> > readConfigsFromFile(){
    std::ifstream infile(INPUT_FILE);
    std::string config, type;
    std::vector<std::pair<std::string,std::string> > configs;
    while (infile >> config >> type)
    {
        std::pair<std::string,std::string> item;
        item.first = config;
        item.second = type;
        configs.push_back(item);
    }
    return configs;
}

std::string getPlaceHolder(std::string type){
    if(type == "ip") 
        return "0.0.0.0";
    else if (type == "text")
        return "Text Value";
    else if (type == "number")
        return "Enter a number";
    else 
        return type;
}


std::string getInputType(std::string type) {
    if(type == "number") 
        return "number";
    else 
        return "text";
}

std::string generateInputsFromConfigs(std::vector<std::pair<std::string,std::string> > configs){
    std::string res = "";
    size_t i = 0;
    for(; i < configs.size() ; i++){
        std::string placeholder = getPlaceHolder(configs[i].second);
        std::string type = getInputType(configs[i].second);
        res += "<p>"+ configs[i].first +"</p><input name=\"" + configs[i].first + "\" type=\"" + type + "\" placeholder=\""+ placeholder +"\" /></br></br>";
    }
    return res;
}

class ConfigHandler : public RequestHandler {
public:
  Response *callback(Request *req) {
    Response *res = new Response;
    if(req->getSessionId() != TOKEN){
        Response *res = Response::redirect("/login");
        return res;        
    }
    res->setHeader("Content-Type", "text/html");
    std::string body;
    body += "<!DOCTYPE html>";
    body += "<html>";
    body += "<body style=\"text-align: center;\">";
    body += "<h1>Available Configs</h1>";
    std::vector<std::pair<std::string,std::string> > configs = readConfigsFromFile();
    body += "<form action=\"/config\" method=\"post\">";
    std::string inputs = generateInputsFromConfigs(configs);
    body += inputs;
    body += "<button type=\"submit\">Commit</button>";
    body += "</form>";
    body += "</body>";
    body += "</html>";
    res->setBody(body);
    return res;
  }
};

class CommitConfigHandler : public RequestHandler {
public:
  Response *callback(Request *req) {
    cimap body = req->getBodyMap();
    std::ofstream commitedFile;
    commitedFile.open (OUTPUT_FILE);
    for (cimap::iterator it=body.begin(); it!=body.end(); ++it){
        commitedFile << it->first << " " << urlDecode(it->second) << "\n";
    }
    commitedFile.close();
    Response *res = Response::redirect("/success");
    return res;
  }
};


#endif