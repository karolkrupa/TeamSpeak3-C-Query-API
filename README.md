# TeamSpeak3-C-Query-API
API who provide functions for operations on ts3 server

## Building
``mkdir build && cd build && cmake .. && make``

## First steps with API
At the beginning, you must inclue API to your projec.
To do this, follow these steps:

1. Build API to get library file ``libteamspeak-api.a``

2. Copy headers folder ``src/includes`` and library file to your project folder.

3. Modify your build command adding -I parameter with path to folder with API header files and -l parameter with name of library file. For example ``g++ -I./includeTs3Api -lteamspeak-api main.cpp -o my_program``

## Connecting to server
```
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
  
  return 0;
}
```

## Getting User
```
#include <Server.hpp>
#include <iostream>

using namespace std;

int main() {
  //
  // Connection code
  //
  
  auto client = server.getClientByNickname("some nick");
  
  if(client.good()) {
    // We have that client
  }else {
    // Probably client with that nick doesn't exist
  }
  
  return 0;
}
```

## Maniplating user
```
#include <Server.hpp>
#include <iostream>

using namespace std;

int main() {
  //
  // Connection code
  //
  
  auto client = server.getClientByNickname("some nick");
  
  if(client.good()) {
    // Changing user description
    client.getDescription().change("New description");
    
    // Adding group
    client.addGroup("15");
    
    // Sending poke
    client.poke("Hello!");
    
    // Sending message
    client.sendMessage("What's up?");
    
    // Kicking from channel
    client.kickFromChanne("Bye");
  }else {
    // Probably client with that nick doesn't exist
  }
  
  return 0;
}
```

