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

  // Create new channel
  auto newChannel = server.createChannel("My new channel");

  if(newChannel.good())
    cout << "My new channel created succesful!" << endl;

  // Create new channel with parameters
  map<string, string> parameters;
  parameters["channel_description"] = "My channel description";
  parameters["channel_password"] = "My password";
  parameters["channel_maxclients"] = "15";
  
  auto newChannelWithParameters = server.createChannel("My new channel with params", parameters);

  if(newChannelWithParameters.good())
    cout << "My new channel created succesful!" << endl;

  return 0;
}