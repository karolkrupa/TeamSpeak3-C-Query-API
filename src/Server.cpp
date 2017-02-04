#include <Server.hpp>

using namespace Ts3Api;


Server::serverChangeableParam::serverChangeableParam(Server* server, string name, string value) :
  server(server)
{
  this->name = name;
  this->value = value;
}

ts3Response Server::serverChangeableParam::change(string value) {
  if(server == NULL) return ts3Response();
  return server->executeCommand("serveredit "+name+"="+value);
}

void Server::serverInfoProperties::update() {
  auto response = server.executeCommand("serverinfo");

  if(!response.error) data = response.data;
}







Server::Server(string ip, string port) :
  serverInfo(serverInfoProperties(*this, updateTime))
{
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

Server::Server() :
  serverInfo(serverInfoProperties(*this, updateTime))
{

}




Server::~Server() {
  receiverStop();
  close(sock);
}

bool Server::serverConnect(string ip, string port) {
  connection[0] = ip;
  connection[1] = port;

  return connectToServer();
}

bool Server::connectToServer() {
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
  mtx.lock();
  if(receiverStatus()) {
    receiverLastMsg = ts3Response();
    sendData(command);

    while(receiverLastMsg.data == "") usleep(100);
    ts3Response localResponse = receiverLastMsg;
    receiverLastMsg = ts3Response();
    mtx.unlock();
    return localResponse;
  }else {
    sendData(command);
    auto response =  getNormalData();
    mtx.unlock();
    return response;
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
  connection[2] = port;
  return executeCommand("use port="+port);
}

Group Server::getServerGroupById(string id) {
  return Group(*this, id, Group::SERVER);
}

Group Server::getServerGroupByName(string name) {
  map<string, map<string, string>> groupList;
  auto response = executeCommand("servergrouplist");
  if(response.error) return Group(*this, "unknown");

  split(groupList, response.data, "name");

  if(groupList.find(name) != groupList.end())
    return Group(*this, groupList[name]["cgid"], Group::CHANNEL);
  else
    return Group(*this, "unknown");
}

Group Server::getChannelGroupById(string id) {
  return Group(*this, id, Group::CHANNEL);
}

Group Server::getChannelGroupByName(string name) {
  map<string, map<string, string>> groupList;
  auto response = executeCommand("channelgrouplist");
  if(response.error) return Group(*this, "unknown");

  split(groupList, response.data, "name");

  if(groupList.find(name) != groupList.end())
    return Group(*this, groupList[name]["cgid"], Group::CHANNEL);
  else
    return Group(*this, "unknown");
}

map<string, Server> Server::getServerList() {
  map<string, map<string, string>> serverList;
  map<string, Server> returnedMap;
  ts3Response response;

  if(response.error) return returnedMap;

  split(serverList, response.data, "virtualserver_id");

  for(auto it = serverList.begin(); it != serverList.end(); ++it) {
    returnedMap[it->first].serverConnect(connection[0], connection[1]);
    returnedMap[it->first].selectServer(it->second["virtualserver_port"]);
  }

  return returnedMap;
}

void Server::update() {
  updateTime = time(NULL);
}

ts3Response Server::serverDelete(string id) {
  if(id == "") id = serverInfo.getProperty("virtualserver_id").value;

  return executeCommand("serverdelete sid="+id);
}

ts3Response Server::serverCreate(map<string, string> serverProperties) {
  string command = "servercreate";

  for(auto it = serverProperties.begin(); it != serverProperties.end(); ++it) {
    command += " " + it->first + "=" + messageEncode(it->second);
  }

  return executeCommand(command);
}

ts3Response Server::serverStart(string id) {
  if(id == "") id = serverInfo.getProperty("virtualserver_id").value;

  return executeCommand("serverstart sid="+id);
}

ts3Response Server::serverStop(string id) {
  if(id == "") id = serverInfo.getProperty("virtualserver_id").value;

  return executeCommand("serverstop sid="+id);
}

ts3Response Server::serverProcessStop() {
  return executeCommand("serverprocessstop");
}

property Server::getUID() {
  return serverInfo.getProperty("virtualserver_unique_identifier");
}

Server::serverNameProperties Server::getName() {
  serverNameProperties props;
  props.name = serverChangeableParam(this, "virtualserver_name", serverInfo.getProperty("virtualserver_name").value);
  props.phonetic = serverChangeableParam(this, "virtualserver_name_phonetic", serverInfo.getProperty("virtualserver_name_phonetic").value);

  return props;
}
Server::serverChangeableParam Server::getWelcomeMessage() {
  return serverChangeableParam(this, "virtualserver_welcomemessage", serverInfo.getProperty("virtualserver_welcomemessage").value);
}
property Server::getPlatform() {
  return serverInfo.getProperty("virtualserver_platform");
}
property Server::getVersion() {
  return serverInfo.getProperty("virtualserver_version");
}
Server::serverChangeableParam Server::getMaxClients() {
  return serverChangeableParam(this, "virtualserver_maxclients", serverInfo.getProperty("virtualserver_maxclients").value);
}
Server::serverChangeableParam Server::getPassword() {
  return serverChangeableParam(this, "virtualserver_password", serverInfo.getProperty("virtualserver_password").value);
}
Server::serverClientsOnlineProperties Server::getClientsOnline() {
  serverClientsOnlineProperties props;
  props.normal = serverInfo.getProperty("virtualserver_clientsonline").value;
  props.query = serverInfo.getProperty("virtualserver_queryclientsonline").value;

  return props;
}
property Server::getChannelsOnline() {
  return serverInfo.getProperty("virtualserver_channelsonline");
}
property Server::getCreatedTime() {
  return serverInfo.getProperty("virtualserver_created");
}
property Server::getUptime() {
  return serverInfo.getProperty("virtualserver_uptime");
}
Server::serverChangeableParam Server::getCodecEncryptionMode() {
  return serverChangeableParam(this, "virtualserver_codec_encryption_mode", serverInfo.getProperty("virtualserver_codec_encryption_mode").value);
}
Server::serverChangeableParam Server::getHostMessage() {
  return serverChangeableParam(this, "virtualserver_hostmessage", serverInfo.getProperty("virtualserver_hostmessage").value);
}
Server::serverChangeableParam Server::getHostMessageMode() {
  return serverChangeableParam(this, "virtualserver_hostmessage_mode", serverInfo.getProperty("virtualserver_hostmessage_mode").value);
}
property Server::getFileBase() {
  return serverInfo.getProperty("virtualserver_filebase");
}
Server::serverChangeableParam Server::getDefaultServerGroup() {
  return serverChangeableParam(this, "virtualserver_default_server_group", serverInfo.getProperty("virtualserver_default_server_group").value);
}
Server::serverChangeableParam Server::getDefaultChannelGroup() {
  return serverChangeableParam(this, "virtualserver_default_channel_group", serverInfo.getProperty("virtualserver_default_channel_group").value);
}
Server::serverChangeableParam Server::getPasswordFlag() {
  return serverChangeableParam(this, "virtualserver_flag_password", serverInfo.getProperty("virtualserver_flag_password").value);
}
Server::serverChangeableParam Server::getDefaultChannelAdminGroup() {
  return serverChangeableParam(this, "virtualserver_default_channel_admin_group", serverInfo.getProperty("virtualserver_default_channel_admin_group").value);
}

Server::serverChangeableParam Server::getHostBannerURL() {
  return serverChangeableParam(this, "virtualserver_hostbanner_url", serverInfo.getProperty("virtualserver_hostbanner_url").value);
}
Server::serverChangeableParam Server::getHostBannerGfxURL() {
  return serverChangeableParam(this, "virtualserver_hostbanner_gfx_url", serverInfo.getProperty("virtualserver_hostbanner_gfx_url").value);
}
Server::serverChangeableParam Server::getHostBannerGfxInterval() {
  return serverChangeableParam(this, "virtualserver_hostbanner_gfx_interval", serverInfo.getProperty("virtualserver_hostbanner_gfx_interval").value);
}
Server::serverComplainSettings Server::getComplainSettings() {
  serverComplainSettings props;
  props.autoban_count = serverChangeableParam(this, "virtualserver_complain_autoban_count", serverInfo.getProperty("virtualserver_complain_autoban_count").value);
  props.autoban_time = serverChangeableParam(this, "virtualserver_complain_autoban_time", serverInfo.getProperty("virtualserver_complain_autoban_time").value);
  props.remove_time = serverChangeableParam(this, "virtualserver_complain_remove_time", serverInfo.getProperty("virtualserver_complain_remove_time").value);

  return props;
}
Server::serverChangeableParam Server::getMinClientsInChannelBeforeSilence() {
  return serverChangeableParam(this, "virtualserver_min_clients_in_channel_before_forced_silence", serverInfo.getProperty("virtualserver_min_clients_in_channel_before_forced_silence").value);
}
Server::serverChangeableParam Server::getPrioritySpeakerDimmModificator() {
  return serverChangeableParam(this, "virtualserver_priority_speaker_dimm_modificator", serverInfo.getProperty("virtualserver_priority_speaker_dimm_modificator").value);
}
property Server::getServerID() {
  return serverInfo.getProperty("virtualserver_id");
}
Server::serverAntifolldSettings Server::getAntifloodSettings() {
  serverAntifolldSettings props;
  props.points_tick_reduce = serverChangeableParam(this, "virtualserver_antiflood_points_tick_reduce", serverInfo.getProperty("virtualserver_antiflood_points_tick_reduce").value);
  props.points_tick_reduce = serverChangeableParam(this, "virtualserver_antiflood_points_needed_command_block", serverInfo.getProperty("virtualserver_antiflood_points_needed_command_block").value);
  props.points_tick_reduce = serverChangeableParam(this, "virtualserver_antiflood_points_needed_ip_block", serverInfo.getProperty("virtualserver_antiflood_points_needed_ip_block").value);

  return props;
}
property Server::getClientConnections() {
  return serverInfo.getProperty("virtualserver_client_connections");
}
property Server::getQueryClientConnections() {
  return serverInfo.getProperty("virtualserver_query_client_connections");
}
Server::serverHostButtonSettings Server::getHostbuttonSettings() {
  serverHostButtonSettings props;
  props.tooltip = serverChangeableParam(this, "virtualserver_hostbutton_tooltip", serverInfo.getProperty("virtualserver_hostbutton_tooltip").value);
  props.url = serverChangeableParam(this, "virtualserver_hostbutton_url", serverInfo.getProperty("virtualserver_hostbutton_url").value);
  props.gfx_url = serverChangeableParam(this, "virtualserver_hostbutton_gfx_url", serverInfo.getProperty("virtualserver_hostbutton_gfx_url").value);

  return props;
}

Server::serverTransferSettings Server::getTransferSettings() {
  serverTransferSettings props;
  props.download_quota = serverChangeableParam(this, "virtualserver_download_quota", serverInfo.getProperty("virtualserver_download_quota").value);
  props.upload_quota = serverChangeableParam(this, "virtualserver_upload_quota", serverInfo.getProperty("virtualserver_upload_quota").value);

  return props;
}
Server::serverTransferProperties Server::getTransferInfo() {
  serverTransferProperties props;
  props.month_bytes_downloaded = serverInfo.getProperty("virtualserver_month_bytes_downloaded").value;
  props.month_bytes_uploaded = serverInfo.getProperty("virtualserver_month_bytes_uploaded").value;
  props.total_bytes_downloaded = serverInfo.getProperty("virtualserver_total_bytes_downloaded").value;
  props.total_bytes_uploaded = serverInfo.getProperty("virtualserver_total_bytes_uploaded").value;

  return props;
}
Server::serverChangeableParam Server::getPort() {
  return serverChangeableParam(this, "virtualserver_port", serverInfo.getProperty("virtualserver_port").value);
}
Server::serverChangeableParam Server::getAutostart() {
  return serverChangeableParam(this, "virtualserver_autostart", serverInfo.getProperty("virtualserver_autostart").value);
}
property Server::getMachineID() {
  return serverInfo.getProperty("virtualserver_machine_id");
}
Server::serverChangeableParam Server::getNeededIdentitySecurityLevel() {
  return serverChangeableParam(this, "virtualserver_needed_identity_security_level", serverInfo.getProperty("virtualserver_needed_identity_security_level").value);
}
Server::serverLogSettings Server::getServerLogSettings() {
  serverLogSettings props;
  props.log_client = serverChangeableParam(this, "virtualserver_log_client", serverInfo.getProperty("virtualserver_log_client").value);
  props.log_query = serverChangeableParam(this, "virtualserver_log_query", serverInfo.getProperty("virtualserver_log_query").value);
  props.log_channel = serverChangeableParam(this, "virtualserver_log_channel", serverInfo.getProperty("virtualserver_log_channel").value);
  props.log_permissions = serverChangeableParam(this, "virtualserver_log_permissions", serverInfo.getProperty("virtualserver_log_permissions").value);
  props.log_server = serverChangeableParam(this, "virtualserver_log_server", serverInfo.getProperty("virtualserver_log_server").value);
  props.log_filetransfer = serverChangeableParam(this, "virtualserver_log_filetransfer", serverInfo.getProperty("virtualserver_log_filetransfer").value);

  return props;
}
Server::serverChangeableParam Server::getIconID() {
  return serverChangeableParam(this, "virtualserver_icon_id", serverInfo.getProperty("virtualserver_icon_id").value);
}
Server::serverChangeableParam Server::getReservedSlots() {
  return serverChangeableParam(this, "virtualserver_reserved_slots", serverInfo.getProperty("virtualserver_reserved_slots").value);
} 
Server::serverPacketLossProperties Server::getPacketlossStatus() {
  serverPacketLossProperties props;
  props.speech = serverInfo.getProperty("virtualserver_total_packetloss_speech").value;
  props.keepalive = serverInfo.getProperty("virtualserver_total_packetloss_keepalive").value;
  props.control = serverInfo.getProperty("virtualserver_total_packetloss_control").value;
  props.total = serverInfo.getProperty("virtualserver_total_packetloss_total").value;

  return props;
}
property Server::getPing() {
  return serverInfo.getProperty("virtualserver_total_ping");
}
property Server::getIP() {
  return serverInfo.getProperty("virtualserver_ip");
}
Server::serverChangeableParam Server::getWeblistEnabled() {
  return serverChangeableParam(this, "virtualserver_weblist_enabled", serverInfo.getProperty("virtualserver_weblist_enabled").value);
}
property Server::getAskForPrivilegekey() {
  return serverInfo.getProperty("virtualserver_weblist_enabled");
}
Server::serverChangeableParam Server::getHostbannerMode() {
  return serverChangeableParam(this, "virtualserver_hostbanner_mode", serverInfo.getProperty("virtualserver_hostbanner_mode").value);
}
Server::serverChangeableParam Server::getTemporaryChannelDeleteDelay() {
  return serverChangeableParam(this, "virtualserver_channel_temp_delete_delay_default", serverInfo.getProperty("virtualserver_channel_temp_delete_delay_default").value);
}
Server::serverClientsMinimumVersion Server::getClientsMinimumVersion() {
  serverClientsMinimumVersion props;
  props.android = serverChangeableParam(this, "virtualserver_min_android_version", serverInfo.getProperty("virtualserver_min_android_version").value);
  props.ios = serverChangeableParam(this, "virtualserver_min_ios_version", serverInfo.getProperty("virtualserver_min_ios_version").value);
  props.pc = serverChangeableParam(this, "virtualserver_min_client_version", serverInfo.getProperty("virtualserver_min_client_version").value);

  return props;
}
property Server::getStatus() {
  return serverInfo.getProperty("virtualserver_status");
}
Server::serverConnectionProperties Server::getConnectionInfo() {
  serverConnectionProperties props;
  props.filetransfer_bandwidth_sent = serverInfo.getProperty("connection_filetransfer_bandwidth_sent").value;
  props.filetransfer_bandwidth_received = serverInfo.getProperty("connection_filetransfer_bandwidth_received").value;
  props.filetransfer_bytes_sent_total = serverInfo.getProperty("connection_filetransfer_bytes_sent_total").value;
  props.filetransfer_bytes_received_total = serverInfo.getProperty("connection_filetransfer_bytes_received_total").value;
  props.packets_sent_speech = serverInfo.getProperty("connection_packets_sent_speech").value;
  props.bytes_sent_speech = serverInfo.getProperty("connection_bytes_sent_speech").value;
  props.packets_received_speech = serverInfo.getProperty("connection_packets_received_speech").value;
  props.bytes_received_speech = serverInfo.getProperty("connection_bytes_received_speech").value;
  props.packets_sent_keepalive = serverInfo.getProperty("connection_packets_sent_keepalive").value;
  props.bytes_sent_keepalive = serverInfo.getProperty("connection_bytes_sent_keepalive").value;
  props.packets_received_keepalive = serverInfo.getProperty("connection_packets_received_keepalive").value;
  props.bytes_received_keepalive = serverInfo.getProperty("connection_bytes_received_keepalive").value;
  props.packets_sent_control = serverInfo.getProperty("connection_packets_sent_control").value;
  props.bytes_sent_control = serverInfo.getProperty("connection_bytes_sent_control").value;
  props.packets_received_control = serverInfo.getProperty("connection_packets_received_control").value;
  props.bytes_received_control = serverInfo.getProperty("connection_bytes_received_control").value;
  props.packets_sent_total = serverInfo.getProperty("connection_packets_sent_total").value;
  props.bytes_sent_total = serverInfo.getProperty("connection_bytes_sent_total").value;
  props.packets_received_total = serverInfo.getProperty("connection_packets_received_total").value;
  props.bytes_received_total = serverInfo.getProperty("connection_bytes_received_total").value;
  props.bandwidth_sent_last_second_total = serverInfo.getProperty("connection_bandwidth_sent_last_second_total").value;
  props.bandwidth_sent_last_minute_total = serverInfo.getProperty("connection_bandwidth_sent_last_minute_total").value;
  props.bandwidth_received_last_second_total = serverInfo.getProperty("connection_bandwidth_received_last_second_total").value;
  props.bandwidth_received_last_minute_total = serverInfo.getProperty("connection_bandwidth_received_last_minute_total").value;

  return props;
}

Channel Server::createChannel(string channelName) {
  map<string, string> channelID;
  auto response = executeCommand("channelcreate channel_name="+messageEncode(channelName));
  if(response.error) return Channel(*this);

  split(channelID, response.data);
  return Channel(*this, channelID["cid"]);
}
Channel Server::createChannel(string channelName, map<string, string> channelProperties) {
  map<string, string> channelID;
  string command = "channelcreate channel_name="+messageEncode(channelName);

  for(auto it = channelProperties.begin(); it != channelProperties.end(); ++it) {
    command += " " + it->first + "=" + it->second;
  }

  auto response = executeCommand(command);
  if(response.error) return Channel(*this);

  split(channelID, response.data);
  return Channel(*this, channelID["cid"]);
}
Group Server::channelGroupCreate(string name, groupDbType type) {
  map<string, string> groupID;
  auto response = executeCommand("channelgroupadd name="+messageEncode(name)+" type="+to_string(type));
  if(response.error) return Group(*this);

  split(groupID, response.data);
  return Group(*this, groupID["cgid"]);
}
Group Server::serverGroupCreate(string name, groupDbType type) {
  map<string, string> groupID;
  auto response = executeCommand("servergroupadd name="+messageEncode(name)+" type="+to_string(type));
  if(response.error) return Group(*this);

  split(groupID, response.data);
  return Group(*this, groupID["sgid"]);
}

ts3Response Server::updateProperty(string propertyName, string value) {
	return executeCommand("clientupdate "+propertyName+"="+messageEncode(value));
}

ts3Response Server::setNickname(string nickname) {
	return updateProperty("client_nickname", nickname);
}