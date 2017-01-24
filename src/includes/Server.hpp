#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netdb.h>
#include <unistd.h>
#include <queue>

#include <cstdio>
#include <ctime>

#include <regex>

#include <mutex>
#include <thread>

#include <Client.hpp>
#include <functions.hpp>
#include <Channel.hpp>
#include <Group.hpp>
#include <structs/serverStructs.hpp>

#ifndef _SERVER_HPP
#define _SERVER_HPP

using namespace std;

namespace Ts3Api {
  class Client;
  class Channel;
  class Group;

  class Server {
    friend class Client;

  private:
    int sock = -1;
    queue<error> errors;

    bool receiverBreak = false;
    bool receiverDown = true;

    thread receiverThd;
    mutex mtx;

    ts3Response receiverLastMsg;

    string connection[2];

    int connectToServer();

    bool socketAvailable();

    socketData getSocketData();

    ts3Response getNormalData();

    void clearSocketBuffer();

    bool sendData(string data);

    ts3Response convertSocketDataToTs3Response(socketData data);
  public:
    void receiverLoop();

    bool receiverStatus();

    bool receiverStop();

    bool receiverStart();

    ts3Response executeCommand(string command);
  //public:
    Server(const Server&) = delete;

    Server(string ip, string port);

    ~Server();

    bool connected();

    error getError();

    ts3Response selectServer(string port);

    ts3Response login(string login, string password);

    Client getClientByClid(string clid);

    Client getClientByDbid(string clid);

    Client getClientByUid(string clid);

    Client getClientByNickname(string nickname);

    Group getServerGroupById(string id);

    Group getServerGroupByName(string name);

    Group getChannelGroupById(string id);

    Group getChannelGroupByName(string name);
  };
}
#endif
