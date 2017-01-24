#include <Server.hpp>

using namespace Ts3Api;

Server::Server(string ip, string port) {
  connection[0] = ip;
  connection[1] = port;

  connectToServer();
  /*
  clock_t start = clock();
  sendData("clientlist");
  cout << clock() - start << endl;

  start = clock();
  ts3Response data = getNormalData();
  cout << clock() - start << endl;

  cout << "id:" << data.errorId << "koniec" <<  endl;
  cout << "MSG:" << data.errorMsg << "koniec"<<endl;
  */
}

Server::~Server() {
  receiverStop();
  close(sock);
}

int Server::connectToServer() {
  addrinfo serverInfo, *inetServerInfo = NULL;

  memset(&serverInfo, 0, sizeof(addrinfo));
  serverInfo.ai_family    = AF_UNSPEC;
  serverInfo.ai_socktype  = SOCK_STREAM;
  serverInfo.ai_protocol  = IPPROTO_TCP;

  if(getaddrinfo(connection[0].c_str(), connection[1].c_str(), &serverInfo, &inetServerInfo) != 0) {
    errors.push(error("Niepowodzenie podczas konwersji adresu serwera", 1));
    return false;
  }

  if((sock = socket(inetServerInfo->ai_family, inetServerInfo->ai_socktype, inetServerInfo->ai_protocol)) == -1) {
    errors.push(error("Niepowodzenie podczas tworzenia gniazda", 2));
    sock = -1;
    return false;
  }

  if(connect(sock, inetServerInfo->ai_addr, inetServerInfo->ai_addrlen) != 0) {
    errors.push(error("Niepowodzenie podczas łączenia z serwerem", 3));
    sock = -1;
    close(sock);
    return false;
  }

  freeaddrinfo(inetServerInfo);

  clearSocketBuffer();

  return true;
}

socketData Server::getSocketData() {
  char recvBuff[256] = "";

  int bytes = recv(sock, &recvBuff, 255, 0);

  return socketData(recvBuff, bytes);
}

void Server::clearSocketBuffer() {
  while(socketAvailable()) {
    getSocketData();
  }
}

bool Server::socketAvailable() {
  struct timeval tv;
  fd_set sockSet;

  tv.tv_sec = 1;
  tv.tv_usec = 500000;
  FD_ZERO(&sockSet);
  FD_SET(sock, &sockSet);

  select(sock+1, &sockSet, NULL, NULL, &tv);

  return (FD_ISSET(sock, &sockSet))? true : false;
}

bool Server::sendData(string data) {
  data = data + "\n";
  ssize_t bytesSended = 0;
  do{

    if(bytesSended > 0) {
      data = data.substr(bytesSended, string::npos);
    }

    bytesSended = send(sock, data.c_str(), data.length(), 0);

    if(bytesSended == -1) return false;

  }while(bytesSended < data.length());

  return true;
}

bool Server::connected() {
  int err = 0;
  socklen_t size = sizeof(err);
  return (getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &size) != 0)? false : true;
}

ts3Response Server::convertSocketDataToTs3Response(socketData data) {
  size_t dataEnd = 0, errorEnd = 0;
  string content, error;

  if(data.data.substr(0, 6) == "notify") return ts3Response();


  if((errorEnd = data.data.rfind("\n\r")) == string::npos) return ts3Response();

  if((dataEnd = data.data.find("\n\r")) == string::npos) return ts3Response();

  if(dataEnd == errorEnd) {
    error = data.data;
    content = data.data.substr(0, dataEnd);
  }else {
    string content = data.data.substr(0, dataEnd);
    error = data.data.substr(dataEnd+2, data.data.length()-(dataEnd+2));
  }

  size_t firstLetter, lastGap;

  firstLetter = error.find("id=")+3;
  lastGap = error.find(" ", firstLetter);
  string errorId = error.substr(firstLetter, lastGap-firstLetter);

  firstLetter = error.find("msg=")+4;
  lastGap = error.find("\n\r", firstLetter);
  string errorMsg = error.substr(firstLetter, lastGap-firstLetter);

  return ts3Response(content, errorId, errorMsg);

}

ts3Response Server::getNormalData() {
  socketData recvData;
  string msg, recvMsg, errorId, errorMsg, data;
  size_t pos = 0, dataPos = 0;

  while((recvData = getSocketData()).bytes > 0) {
    msg += recvData.data;
    if(msg.length() < 6) continue;

    if(msg.substr(0, 6) == "notify") {
      if((pos = msg.find("\n\r")) == string::npos) continue;

      recvMsg = msg.substr(0, pos);

      pos = 0;
      msg.erase(0, recvMsg.length()+2);
    }else if(msg.substr(0, 5) == "error") {
      if((pos = msg.find("\n\r")) == string::npos) continue;

      recvMsg = msg.substr(0, pos);

      errorId = recvMsg.substr(9, (pos = recvMsg.find(" ", 9))-9);
      errorMsg = recvMsg.substr(pos+5, string::npos);

      return ts3Response(recvMsg, errorId, errorMsg);

      pos = 0;
      msg.erase(0, recvMsg.length()+2);
    }else {
      if((dataPos = msg.find("\n\r")) == string::npos) continue;
      if((pos = msg.find("\n\r", dataPos+1)) == string::npos) continue;

      recvMsg = msg.substr(0, pos);

      data = recvMsg.substr(0, dataPos);
      dataPos += 11;
      errorId = recvMsg.substr(dataPos, (pos = recvMsg.find(" ", dataPos))-dataPos);
      errorMsg = recvMsg.substr(pos+5, string::npos);

      return ts3Response(data, errorId, errorMsg);

      pos = 0;
      dataPos = 0;
      msg.erase(0, recvMsg.length()+2);
    }
  }
  // string errorData;
  // size_t dataPos;
  //
  // socketData data = getSocketData();
  //
  // if(data.bytes < 1) {
  //   errors.push(error("Niepowodzenie podczas łączenia z serwerem", 3));
  //   return ts3Response("", "-1");
  // }
  //
  // while(data.bytes < 6)
  //   data.data += getSocketData().data;
  //
  // while(data.data.substr(0, 6) == "notify") {
  //   while((dataPos = data.data.find("\n\r")) == string::npos)
  //     data.data += getSocketData().data;
  //
  //   data.data = data.data.substr(dataPos+2, string::npos);
  //
  //   while(data.bytes < 6)
  //     data.data += getSocketData().data;
  // }
  //
  // if((dataPos = data.data.rfind("\n\r")) == string::npos) {
  //   if(data.data.substr(0, 5) != "error")
  //     while(data.data.rfind("\n\r", dataPos-1) == string::npos) {
  //       data.data += getSocketData().data;
  //     }
  // }
  //
  // return convertSocketDataToTs3Response(data);
}

void Server::receiverLoop() {
  socketData recvData;
  string msg, recvMsg, errorId, errorMsg, data;
  size_t pos = 0, dataPos = 0;

  receiverDown = false;

  while((recvData = getSocketData()).bytes > 0) {

    msg += recvData.data;
    if(msg.length() < 6) continue;

    if(msg.substr(0, 6) == "notify") {
      if((pos = msg.find("\n\r")) == string::npos) continue;

      recvMsg = msg.substr(0, pos);

      pos = 0;
      msg.erase(0, recvMsg.length()+2);
    }else if(msg.substr(0, 5) == "error") {
      if((pos = msg.find("\n\r")) == string::npos) continue;

      recvMsg = msg.substr(0, pos);

      errorId = recvMsg.substr(9, (pos = recvMsg.find(" ", 9))-9);
      errorMsg = recvMsg.substr(pos+5, string::npos);

      receiverLastMsg = ts3Response(recvMsg, errorId, errorMsg);

      pos = 0;
      msg.erase(0, recvMsg.length()+2);
    }else {
      if((dataPos = msg.find("\n\r")) == string::npos) continue;
      if((pos = msg.find("\n\r", dataPos+1)) == string::npos) continue;

      recvMsg = msg.substr(0, pos);

      data = recvMsg.substr(0, dataPos);
      dataPos += 11;
      errorId = recvMsg.substr(dataPos, (pos = recvMsg.find(" ", dataPos))-dataPos);
      errorMsg = recvMsg.substr(pos+5, string::npos);

      receiverLastMsg = ts3Response(data, errorId, errorMsg);

      pos = 0;
      dataPos = 0;
      msg.erase(0, recvMsg.length()+2);
    }

    if(receiverBreak) {
      cout << "Wyłacza" << endl;
      receiverDown = true;
      receiverBreak = false;
      return;
    }
  }
    //ts3Response operator=(ts3Response obj);
  //Rozłączono z serwerem
}

bool Server::receiverStatus() {
  return !receiverDown;
}

bool Server::receiverStop() {
  if(!receiverStatus()) return true;
  receiverBreak = true;
  sendData("ALIVE");

  for(int i = 0; i<10; i++) {
    if(receiverBreak == false) {
      receiverThd.join();
      return true;
    }
    sleep(1);
  }
  return false;
}

bool Server::receiverStart() {
  receiverThd = thread([this] {
    receiverLoop();
  });

  sleep(1);
  return !receiverDown;
}

ts3Response Server::executeCommand(string command) {
  if(receiverStatus()) {
    mtx.lock();
    receiverLastMsg = ts3Response();
    sendData(command);

    while(receiverLastMsg.data == "") usleep(100);
    ts3Response localResponse = receiverLastMsg;
    receiverLastMsg = ts3Response();
    mtx.unlock();
    return localResponse;
  }else {
    sendData(command);
    return getNormalData();
  }
}

error Server::getError() {
  if(errors.empty()) return error();
  return errors.back();
}

Client Server::getClientByClid(string clid) {
  return Client(*this, Client::IDs(clid));
}

Client Server::getClientByDbid(string dbid) {
  return Client(*this, Client::IDs("", "", dbid));
}

Client Server::getClientByUid(string uid) {
  return Client(*this, Client::IDs("", uid));
}

Client Server::getClientByNickname(string nickname) {
  map<string, string> returnedMap;
  split(returnedMap, executeCommand("clientfind pattern="+ nickname).data);

  if(returnedMap.find("clid") == returnedMap.end()) return Client(*this, Client::IDs());

  return getClientByClid(returnedMap["clid"]);
}

ts3Response Server::login(string login, string password) {
  return executeCommand("login "+messageEncode(login)+" "+messageEncode(password));
}

ts3Response Server::selectServer(string port) {
  return executeCommand("use port="+port);
}
