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

  /**
   * @brief      Server Class.
   */
  class Server {
  private:
    /// Descriptor ID
    int sock = -1;

    /// Errors queue
    queue<error> errors;

    /**
     * @brief Receiver var control
     * 
     * When is true receiver will break own loop
     */
    bool receiverBreak = false;
    /**
     * @brief Receiver var control
     * 
     * When is true receiver it's off
     */
    bool receiverDown = true;

    /// Receiver thread
    thread receiverThd;

    /// Multithread mutex
    mutex mtx;

    /// Contain last message from server received by receiver
    ts3Response receiverLastMsg;

    /// Contain ipAddress[0] and serverPort[1]
    string connection[2];

    /**
     * @brief      Connects to server.
     *
     * @return     true if connected or false if not connected
     */
    bool connectToServer();

    /**
     * @brief      Checks wheter socket is connected
     *
     * @return     true if connected or false if not connected
     */
    bool socketAvailable();

    /**
     * @brief      Gets the socket data in raw from
     *
     * @return     The socket data.
     * 
     * @see socketData
     */
    socketData getSocketData();

    /**
     * @brief      Gets the socket data in normalized form
     *
     * @return     The normal data.
     * 
     * @see ts3Response
     */
    ts3Response getNormalData();

    /**
     * @brief      Clears socket buffer
     */
    void clearSocketBuffer();

    /**
     * @brief      Sends a data through socket
     *
     * @param[in]  data  Data to send
     *
     * @return     true if sended or false on error
     */
    bool sendData(string data);

    /**
     * @brief      Converting raw socket data to normalized form
     *
     * @param[in]  data  Raw socket data
     *
     * @return     Normal Ts3Server data
     * 
     * @see ts3Response
     * @see socketData
     */
    ts3Response convertSocketDataToTs3Response(socketData data);

    /**
     * @brief      Reciver thread loop
     */
    void receiverLoop();

    /**
     * @brief      Return receiver status
     *
     * @return     true if running or false if down
     */
    bool receiverStatus();

    /**
     * @brief      Stopping receiver thread
     *
     * @return     true if receiver stopped or false
     */
    bool receiverStop();

    /**
     * @brief      Starting receiver thread
     *
     * @return     true if succesful
     */
    bool receiverStart();

    public:
    /**
     * @brief      Performs selected command on Ts3 Server
     *
     * @param[in]  command  The command
     *
     * @return     Data returned from the server
     */
    ts3Response executeCommand(string command);

    /**
     * @brief      Creating and connectiong connection with Ts3Server
     *
     * @param[in]  ip    Ts3 server address
     * @param[in]  port  Ts3 server port
     */
    Server(string ip, string port);

    /**
     * @brief      Destroys the object.
     */
    ~Server();

    /**
     * @brief      Checks connection with Ts3 Server
     *
     * @return     true if connected
     */
    bool connected();

    /**
     * @brief      Return last error
     *
     * @return     The error.
     */
    error getError();

    /**
     * @brief      Selects Ts3 server
     *
     * @param[in]  port  Ts3 Server port
     *
     * @return     Data returned from the server
     */
    ts3Response selectServer(string port);

    /**
     * @brief      Login to Ts3 Server
     *
     * @param[in]  login     ServerQuery user login
     * @param[in]  password  ServerQuery user password
     *
     * @return     Data returned from the server
     */
    ts3Response login(string login, string password);

    /**
     * @brief      Gets the client by clid.
     *
     * @param[in]  clid  Client ID
     *
     * @return     Client Object
     * 
     * @see Client
     */
    Client getClientByClid(string clid);

    /**
     * @brief      Gets the client by dbid.
     *
     * @param[in]  clid  Client Database ID
     *
     * @return     Client Object
     * 
     * @see Client
     */
    Client getClientByDbid(string dbid);

    /**
     * @brief      Gets the client by uid.
     *
     * @param[in]  uid  Client Uniquer Identifier
     *
     * @return     Client Object
     * 
     * @see Client
     */
    Client getClientByUid(string uid);

    /**
     * @brief      Gets the client by nickname.
     *
     * @param[in]  nickname  Client Nickname
     *
     * @return     Client Object
     * 
     * @see Client
     */
    Client getClientByNickname(string nickname);

    /**
     * @brief      Gets the server group by identifier.
     *
     * @param[in]  id    Group ID
     *
     * @return     Group Object
     * 
     * @see Group
     */
    Group getServerGroupById(string id);

    /**
     * @brief      Gets the server group by name.
     *
     * @param[in]  name  Group Name
     *
     * @return     Group Object
     * 
     * @see Group
     */
    Group getServerGroupByName(string name);

    /**
     * @brief      Gets the channel group by identifier.
     *
     * @param[in]  id    Group Identifier
     *
     * @return     Group Object
     * 
     * @see Group
     */
    Group getChannelGroupById(string id);

    /**
     * @brief      Gets the channel group by name.
     *
     * @param[in]  name  Group Name
     *
     * @return     Group Object
     * 
     * @see Group
     */
    Group getChannelGroupByName(string name);
  };
}
#endif
