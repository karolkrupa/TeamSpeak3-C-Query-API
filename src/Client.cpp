#include <Client.hpp>

using namespace Ts3Api;

Client::Client(Server& server, IDs ids, bool clientListInit) :
  server(server),
  clientIDs(ids),
  clientInfo(clientInfoProperties(clientIDs, server, updateTime, clientListInit))
{
  if((clientIDs.uid == "unknown") && (clientIDs.dbid == "unknown")) {
    clientInfo.update();

    clientIDs.uid = clientInfo.getProperty("client_unique_identifier").value;
    clientIDs.dbid = clientInfo.getProperty("client_database_id").value;
  }
}

Client::IDs::IDs(string clid, string uid, string dbid) {
  this->clid  = clid;
  this->uid   = uid;
  this->dbid  = dbid;
}

Client::clientInfoProperties::clientInfoProperties(IDs &ids,  Server &server, time_t &updateTime, bool clientListInit) : ids(ids), ts3ObjectProperties(server, updateTime, clientListInit) {
  
}

void Client::clientInfoProperties::update() {
  ts3Response tmpData;
  map<string, string> returnedMap;

  updatedTime = updateTime+1;
  if(incompleteInit) incompleteInit = false;

  if(ids.clid != "unknown") {
    tmpData = server.executeCommand("clientinfo clid="+ids.clid);

    if(tmpData.error) {
      ids.clid = "unknown";

      if(ids.dbid == "unknown") {
        update();
        return;
      }

      if(ids.dbid == "unknown") {
        split(returnedMap, tmpData.data);
        if(returnedMap.find("client_unique_identifier") != returnedMap.end())
          ids.uid = returnedMap["client_unique_identifier"];
      }

      if(ids.uid == "unknown") {
        split(returnedMap, tmpData.data);
        if(returnedMap.find("client_database_id") != returnedMap.end())
          ids.dbid = returnedMap["client_database_id"];
      }

      tmpData = server.executeCommand("clientdbinfo cldbid="+ids.dbid);
      if(tmpData.error) return;
    }
  }else if(ids.uid != "unknown") {
    tmpData = server.executeCommand("clientgetids cluid="+ids.uid);
    if(tmpData.error) return;
    split(returnedMap, tmpData.data);

    if(ids.dbid == "unknown") {
      if(returnedMap.find("cldbid") != returnedMap.end())
        ids.dbid = returnedMap["cldbid"];
    }

    if(returnedMap.find("clid") != returnedMap.end()) {
      ids.clid = returnedMap["clid"];
      update();
      return;
    }else {
      tmpData = server.executeCommand("clientdbinfo cldbid="+ids.dbid);
      if(tmpData.error) return;
    }
  }else if(ids.dbid != "unknown") {
    tmpData = server.executeCommand("clientgetnamefromdbid cldbid="+ids.dbid);
    if(tmpData.error) return;
    split(returnedMap, tmpData.data);
    if(returnedMap.find("cluid") != returnedMap.end())
      ids.uid = returnedMap["cluid"];

    update();
    return;
  }else {
    return;
  }

  data = tmpData.data;

  clearBuff();
}

void Client::update() {
  updateTime = time(NULL);
}

bool Client::good() {
  if(clientIDs.dbid != "unknown")
    return true;

  if(clientIDs.uid != "unknown")
    return true;

  return false;
}

string Client::getDbid() {
  if(clientIDs.dbid == "unknown")
    clientInfo.update();

  return clientIDs.dbid;
}
string Client::getUid() {
  if(clientIDs.dbid == "unknown")
    clientInfo.update();

  return clientIDs.dbid;
}


Client::nickname::nickname(string nickname, string phonetic) {
  value = nickname;
  this->phonetic = phonetic;
}

timestampTime::timestampTime(string time) {
  value = time;
}

string timestampTime::toDate(string pattern) {
  char buffer[20];
  time_t timeVal = (time_t) strtol(value.c_str(), NULL, 10);
  strftime(buffer, 20, pattern.c_str(), localtime(&timeVal));
  return (string)buffer;
}

// Client::permission::permission(Client &client, string name, string value) :
//   client(client)
// {
//   this->name = name;
//   this->value = value;
// }

Client::changeableParam::changeableParam(Client &client, string value, string paramName) :
  client(client)
{
  this->value = value;
  this->paramName = paramName;
}

Ts3Api::ts3Response Client::changeableParam::change(string value)
{
  if(client.clientIDs.clid == "unknown")
    return client.server.executeCommand("clientdbedit cldbid="+client.clientIDs.dbid +" "+paramName+"="+messageEncode(value));
  else
    return client.server.executeCommand("clientedit clid="+client.clientIDs.clid +" "+paramName+"="+messageEncode(value));
}


// Data functions

property Client::getCLID() {
  return property("clid", clientIDs.clid);
}
property Client::getUID() {
  return clientInfo.getProperty("client_unique_identifier");
}
property Client::getDBID() {
  return clientInfo.getProperty("client_database_id");
}
property Client::getIdleTime() {
  return clientInfo.getProperty("client_idle_time");
}
Client::nickname Client::getNickname() {
  return nickname(clientInfo.getProperty("client_nickname").value, clientInfo.getProperty("client_nickname_phonetic").value);
}
property Client::getVersion() {
  return clientInfo.getProperty("client_version");
}
property Client::getVersionSign() {
  return clientInfo.getProperty("client_version_sign");
}
property Client::getPlatform(){
  return clientInfo.getProperty("client_platform");
}
bool Client::getInputMuted(){
  return (clientInfo.getProperty("client_input_muted").value == "1")? true : false;
}
bool Client::getOutputMuted(){
  return (clientInfo.getProperty("client_output_muted").value == "1")? true : false;
}
bool Client::getOutputOnlyMuted(){
  return (clientInfo.getProperty("client_outputonly_muted").value == "1")? true : false;
}
bool Client::getInputHardware(){
  return (clientInfo.getProperty("client_input_hardware").value == "1")? true : false;
}
bool Client::getOutputHardware(){
  return (clientInfo.getProperty("client_output_hardware").value == "1")? true : false;
}
property Client::getDefaultChannel(){
  return clientInfo.getProperty("client_default_channel");
}
property Client::getMetaData(){
  return clientInfo.getProperty("client_meta_data");
}
bool Client::getRecordingStatus(){
  return (clientInfo.getProperty("client_is_recording").value == "1")? true : false;
}
property Client::getSecurityHash(){
  return clientInfo.getProperty("client_security_hash");
}
property Client::getLoginName(){
  return clientInfo.getProperty("client_login_name");
}
Group Client::getChannelGroup() {
  return Group(server, clientInfo.getProperty("cid").value);
}
property Client::getChannelGroupID(){
  return clientInfo.getProperty("client_channel_group_id");
}
vector<Group> Client::getServerGroups() {
  vector<Group> returnedVector;
  size_t pos=0, prevPos=0;
  string groups = clientInfo.getProperty("client_servergroups").value;
  groups += ",";
  while((pos = groups.find(",", pos+1)) != string::npos) {
    returnedVector.push_back(Group(server, groups.substr(prevPos, pos-prevPos)));
    prevPos = pos+1;
  }

  return returnedVector;
}
vector<string> Client::getServerGroupsList(){
  vector<string> returnedVector;
  size_t pos=0, prevPos=0;
  string groups = clientInfo.getProperty("client_servergroups").value;
  groups += ",";
  while((pos = groups.find(",", pos+1)) != string::npos) {
    returnedVector.push_back(groups.substr(prevPos, pos-prevPos));
    prevPos = pos+1;
  }

  return returnedVector;
}
timestampTime Client::getCreatedTime(){
  return timestampTime(clientInfo.getProperty("client_created").value);
}
timestampTime Client::getLastconnectedTime(){
  return timestampTime(clientInfo.getProperty("client_lastconnected").value);
}
property Client::getTotalConnections(){
  return clientInfo.getProperty("client_totalconnections");
}
bool Client::getAwayStatus(){
  return (clientInfo.getProperty("client_away").value == "1")? true : false;
}
property Client::getAwayMessage(){
  return clientInfo.getProperty("client_away_message");
}
property Client::getType(){
  return clientInfo.getProperty("client_type");
}
property Client::getAvatarFlag(){
  return clientInfo.getProperty("client_flag_avatar");
}
Ts3Api::Permission Client::getTalkPower(){
  return Permission(*this, "i_client_talk_power", clientInfo.getProperty("client_talk_power").value);
}
property Client::getTalkRequest(){
  return clientInfo.getProperty("client_talk_request");
}
property Client::getTalkRequestMsg(){
  return clientInfo.getProperty("client_talk_request_msg");
}
Client::changeableParam Client::getDescription(){
  return changeableParam(*this, clientInfo.getProperty("client_description").value, "client_description");
}
Client::changeableParam Client::getTalkerStatus(){
  return changeableParam(*this, clientInfo.getProperty("client_is_talker").value, "client_is_talker");
}
Client::transferInfo Client::getTransferInfo(){
  transferInfo transfer;
  transfer.month_bytes_uploaded = clientInfo.getProperty("client_month_bytes_uploaded").value;
  transfer.month_bytes_downloaded = clientInfo.getProperty("client_month_bytes_downloaded").value;
  transfer.total_bytes_uploaded = clientInfo.getProperty("client_total_bytes_uploaded").value;
  transfer.total_bytes_downloaded = clientInfo.getProperty("client_total_bytes_downloaded").value;
  return transfer;
}
bool Client::getPrioritySpeakerStatus() {
  return (clientInfo.getProperty("client_is_priority_speaker").value == "1")? true : false;
}
Ts3Api::Permission Client::getNeededServerqueryViewPower(){
  return Permission(*this, "i_client_needed_serverquery_view_power", clientInfo.getProperty("client_needed_serverquery_view_power").value);
}
property Client::getDefaultToken(){
  return clientInfo.getProperty("client_default_token");
}
Client::changeableParam Client::getIconId(){
  return changeableParam(*this, clientInfo.getProperty("client_icon_id").value, "client_icon_id");
}
Client::changeableParam Client::getChannelCommanderStatus(){
  return changeableParam(*this ,clientInfo.getProperty("client_is_channel_commander").value, "client_is_channel_commander");
}
property Client::getCountry(){
  return clientInfo.getProperty("client_country");
}
property Client::getChannelGroupInheritChannelID(){
  return clientInfo.getProperty("client_channel_group_inherited_channel_id");
}
property Client::getBadges(){
  return clientInfo.getProperty("client_badges");
}
property Client::getBase64HashClientUID(){
  return clientInfo.getProperty("client_base64HashClientUID");
}
Client::connectionInfo Client::getConnectionInfo(){
  connectionInfo info;
  info.filetransfer_bandwidth_sent = clientInfo.getProperty("connection_filetransfer_bandwidth_sent").value;
  info.filetransfer_bandwidth_received = clientInfo.getProperty("connection_filetransfer_bandwidth_received").value;
  info.packets_sent_total = clientInfo.getProperty("connection_packets_sent_total").value;
  info.bytes_sent_total = clientInfo.getProperty("connection_bytes_sent_total").value;
  info.packets_received_total = clientInfo.getProperty("connection_packets_received_total").value;
  info.bytes_received_total = clientInfo.getProperty("connection_bytes_received_total").value;
  info.bandwidth_sent_last_second_total = clientInfo.getProperty("connection_bandwidth_sent_last_second_total").value;
  info.bandwidth_sent_last_minute_total = clientInfo.getProperty("connection_bandwidth_sent_last_minute_total").value;
  info.bandwidth_received_last_second_total = clientInfo.getProperty("connection_bandwidth_received_last_second_total").value;
  info.bandwidth_received_last_minute_total = clientInfo.getProperty("connection_bandwidth_received_last_minute_total").value;
  info.connected_time = clientInfo.getProperty("connection_connected_time").value;
  info.connection_client_ip = clientInfo.getProperty("connection_client_ip").value;
  return info;
}

Ts3Api::ts3Response Client::addGroup(string groupId) {
  return server.executeCommand("servergroupaddclient sgid=" + groupId + " cldbid=" + getDbid());
}

Ts3Api::ts3Response Client::addGroup(const Group &group) {
	return addGroup(group.id);
}

Ts3Api::ts3Response Client::removeGroup(string groupId) {
  return server.executeCommand("servergroupdelclient sgid=" + groupId + " cldbid=" + getDbid());
}

Ts3Api::ts3Response Client::removeGroup(const Group &group) {
	removeGroup(group.id);
}

Ts3Api::ts3Response Client::setChannelGroup(string groupId, string channelId) {
  return server.executeCommand("setclientchannelgroup cgid=" + groupId + " cid="+ channelId +" cldbid=" + getDbid());
}

Ts3Api::ts3Response Client::setChannelGroup(string groupId, const Channel &channel) {
	return setChannelGroup(groupId, channel.id);
}

Ts3Api::ts3Response Client::setChannelGroup(const Group &group , const Channel &channel) {
	return setChannelGroup(group.id, channel.id);
}

Ts3Api::ts3Response Client::setChannelGroup(const Group &group , string channelId) {
	return setChannelGroup(group.id, channelId);
}

Ts3Api::Permission Client::getPermission(string permName) {
  return Permission(*this, permName);
}

Ts3Api::ts3Response Client::dbDelete() {
  return server.executeCommand("clientdbdelete cldbid="+clientIDs.dbid);
}

Ts3Api::ts3Response Client::setServerqueryLogin(string loginName, string &password) {
  ts3Response response = server.executeCommand("clientsetserverquerylogin client_login_name="+loginName);
  if(response.error) {
    return response;
  }else {
    password = messageDecode(response.data.substr(response.data.find("=")+1, string::npos));
    return response;
  }
}

Ts3Api::ts3Response Client::move(string cid, string password) {
  string command = "clientmove clid="+clientIDs.clid + " cid="+cid;
  if(password != "") command += " cpw="+messageEncode(password);
  return server.executeCommand(command);
}

Ts3Api::ts3Response Client::move(Channel& channel, string password) {
	move(channel.id, password);
}

Ts3Api::ts3Response Client::kickFromChannel(string reason) {
  return server.executeCommand("clientkick clid="+clientIDs.clid + " reasonid=4 reasonmsg="+messageEncode(reason));
}

Ts3Api::ts3Response Client::kickFromServer(string reason) {
  return server.executeCommand("clientkick clid="+clientIDs.clid + " reasonid=5 reasonmsg="+messageEncode(reason));
}

Ts3Api::ts3Response Client::poke(string message) {
  return server.executeCommand("clientpoke clid="+clientIDs.clid + " msg="+messageEncode(message));
}

Ts3Api::ts3Response Client::complain(string message) {
  return server.executeCommand("complainadd tcldbid="+clientIDs.dbid + " message="+messageEncode(message));
}

Ts3Api::ts3Response Client::delComplain(string accuserDbid) {
  return server.executeCommand("complaindel tcldbid="+clientIDs.dbid + " fcldbid="+accuserDbid);
}

Ts3Api::ts3Response Client::delAllComplains() {
  return server.executeCommand("complaindelall tcldbid="+clientIDs.dbid);
}

Ts3Api::ts3Response Client::ban(string banTime, string reason) {
  string command = "banclient clid="+clientIDs.clid;
  if(banTime != "") command += " time="+banTime;
  if(reason != "") command += " banreason="+messageEncode(reason);
  return server.executeCommand(command);
}

Ts3Api::ts3Response Client::sendMessage(string message) {
  return server.executeCommand("sendtextmessage targetmode=1 target="+clientIDs.clid+" msg="+messageEncode(message));
}

map<string, Permission> Client::getPermissionList() {
	map<string, map<string, string>> permList;
	map<string, Permission> returnedMap;
	bool negated , skip;

	auto response = server.executeCommand("clientpermlist cldbid="+clientIDs.dbid+" -permsid");

	if(response.error) return returnedMap;

	split(permList, response.data, "permsid");

	if(permList.empty()) return returnedMap;

	for(auto it = permList.begin(); it != permList.end(); ++it) {
		negated = (it->second["permnegated"] == "1")? true : false;
		negated = (it->second["permnegated"] == "1")? true : false;
		returnedMap.emplace(it->first, Permission(*this, it->first, it->second["permvalue"], negated, skip));
	}

	return returnedMap;
}

Channel Client::getChannel() {
	return Channel(server, clientInfo.getProperty("cid").value);
}