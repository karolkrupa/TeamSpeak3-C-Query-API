#include <Server.hpp>
#include <iostream>

using namespace std;

int main() {
  // Connect to server
  Ts3Api::Server server("server_ip", "query_port");

  // Check connection
  if(server.connected()) {
    cout << "Connection Succesful" << endl;
  }else {
    cout << "Connection Error" << endl;
    return 1;
  }

  // Login to server
  if(server.logn("query_login", "query_password").error) {
    cout << "Bad login or password" << endl;
    return 1;
  }

  // Select server
  if(server.selectServer("server_port").error) {
    cout << "Bad port number" << endl;
    return 1;
  }

  // Create new server group
  auto newServerGroup = server.createServerGroup("My new group");

  if(newServerGroup.good())
    cout << "My new server group created succesful!" << endl;

  // Create new channel group
  auto newChannelGroup = server.createChannelGroup("My new channel group");

  if(newChannelGroup.good())
    cout << "My new channel group created succesful!" << endl;

  return 0;
}