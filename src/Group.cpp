#include <Group.hpp>

using namespace Ts3Api;

Group::groupInfoProperties::groupInfoProperties(string &id, Server& server, time_t& updateTime, bool incomleteInit) : id(id), ts3ObjectProperties(server, updateTime, incomleteInit) {

}

void Group::groupInfoProperties::update() {
  auto response = server.executeCommand("channelinfo cid="+id);

  if(incompleteInit) incompleteInit = false;
  if(!response.error) data = response.data;
  updatedTime = updateTime+1;
}

Group::Group(Server &server, string id, ts3GroupType groupType, bool incomleteInit) :
  server(server),
  id(id),
  groupInfo(groupInfoProperties(this->id, server, updateTime, incomleteInit))
{
  this->groupType = groupType;
}

bool Group::good() {
  return (id == "unknown")? false : true;
}

void Group::update() {
  updateTime = time(NULL);
}

vector<Permission> Group::splitPerm(string input) {
  vector<Permission> returnedVector;
  map<string, map<string, string>> returnedMap;

  split(returnedMap, input, "permsid");
  if(returnedMap.empty()) return returnedVector;

  for(auto it = returnedMap.begin(); it != returnedMap.end(); ++it) {
    returnedVector.push_back(Permission(*this, Permission::PermGroupTypeServerGroup, it->first));
  }
}


ts3Response Group::addClient(string dbid) {
  if(groupType == SERVER)
    return server.executeCommand("servergroupaddclient sgid="+id+" cldbid="+dbid);
  else
    return server.executeCommand("setclientchannelgroup cgid="+id+" cid=0 cldbid="+dbid);
}

ts3Response Group::addClient(string dbid, string cid) {
  if(groupType == SERVER)
    return ts3Response();
  else
    return server.executeCommand("setclientchannelgroup cgid="+id+" cid="+cid+" cldbid="+dbid);
}
ts3Response Group::addClient(const Client& client) {
  return addClient(client.clientIDs.dbid);
}

ts3Response Group::deleteClient(string dbid) {
  if(groupType == SERVER)
    return server.executeCommand("servergroupdelclient sgid="+id+" cldbid="+dbid);
  else
    return ts3Response();
}

ts3Response Group::addPermission(string name, string value, bool negated, bool skip) {
  if(groupType == SERVER)
    return server.executeCommand("servergroupaddperm sgid="+id+" permsid="+messageEncode(name)+" permvalue="+value+" permnegated="+to_string(negated)+" permskip="+to_string(skip));
  else
    return server.executeCommand("channelgroupaddperm cgid="+id+" permsid="+messageEncode(name)+" permvalue="+value);
}
ts3Response Group::addPermission(const Permission& permission) {
  return addPermission(permission.name, permission.value, permission.negated, permission.skip);
}

ts3Response Group::deletePermission(string name) {
  if(groupType == SERVER)
    return server.executeCommand("servergroupdelperm sgid="+id+" permsid="+messageEncode(name));
  else
    return server.executeCommand("channelgroupdelperm cgid="+id+" permsid="+messageEncode(name));
}
ts3Response Group::deletePermission(const Permission& permission) {
  return deletePermission(permission.name);
}

vector<Client> Group::getClientList() {

}

ts3Response Group::groupCopy(string tgid, string name, string type) {
  if(groupType == SERVER)
    return server.executeCommand("servergroupcopy ssgid="+id+" tsgid="+tgid+" name="+messageEncode(name)+" type="+type);
  else
    return server.executeCommand("channelgroupcopy scgid="+id+" tsgid="+tgid+" name="+messageEncode(name)+" type="+type);
}
ts3Response Group::groupDelete(bool force) {
  if(groupType == SERVER)
    return server.executeCommand("servergroupdel sgid="+id+" force="+to_string(force));
  else
    return server.executeCommand("channelgroupdel cgid="+id+" force="+to_string(force));
}
ts3Response Group::groupRename(string name) {
  if(groupType == SERVER)
    return server.executeCommand("servergrouprename sgid="+id+" name="+messageEncode(name));
  else
    return server.executeCommand("channelgrouprename cgid="+id+" name="+messageEncode(name));
}

Permission Group::getPermission(string permsid) {
	if(groupType == SERVER)
		return Permission(*this, Permission::PermGroupTypeServerGroup, permsid);
	else
		return Permission(*this, Permission::PermGroupTypeChannelGroup, permsid);
}

map<string, Permission> Group::getPermissionList() {
	map<string, map<string, string>> permList;
	map<string, Permission> returnedMap;
	bool negated, skip;

	ts3Response response;
	if(groupType == SERVER)
		response = server.executeCommand("servergrouppermlist sgid="+id);
	else
		response = server.executeCommand("channelgrouppermlist cgid="+id);

	if(response.error) return returnedMap;

	split(permList, response.data, "permsid");

	if(permList.empty()) return returnedMap;

	for (auto it = permList.begin(); it != permList.end(); ++it)
	{
		negated = (it->second["permnegated"] == "1")? true : false;
		skip = (it->second["permskip"] == "1")? true : false;
		if(groupType == SERVER)
			returnedMap.emplace(it->first, Permission(*this, Permission::PermGroupTypeServerGroup, it->first, it->second["permvalue"], negated, skip));
		else
			returnedMap.emplace(it->first, Permission(*this, Permission::PermGroupTypeChannelGroup, it->first, it->second["permvalue"], negated, skip));
	}

	return returnedMap;
}