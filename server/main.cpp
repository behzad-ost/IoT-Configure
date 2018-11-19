#include "server.hpp"
#include "loginHandler.hpp"
#include "configHandler.hpp"
#include <iostream>

using namespace std;

class IndexHandler : public RequestHandler {
public:
  Response *callback(Request *req) {
    Response *res = Response::redirect("/config");
    return res;
  }
};

int main(int argc, char **argv) {
  try {
    Server server(argc > 1 ? atoi(argv[1]) : 5000, "static/404.html");
    server.get("/", new IndexHandler());
    server.get("/login", new ShowPage("static/login.html"));
    server.post("/login", new LoginHandler());
    server.get("/config", new ConfigHandler());
    server.post("/config", new CommitConfigHandler());
    server.get("/success", new ShowPage("static/success.html"));
    server.run();
  } catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}