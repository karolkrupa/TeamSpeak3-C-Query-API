#include <Permission.hpp>

using namespace Ts3Api;

Permission::Permission(Client &client, string permName, string permValue, bool negated, bool skip) :
  server(client.server)
{
  this->type = PermGroupTypeGlobalClient;
  this->client = &client;
  this->name = permName;
  this->value = permValue;
  this->negated = negated;
  this->skip = skip;
  if(permValue == "unknown") updateValue();
}

Permission::Permission(Channel &channel, string permName, string permValue, bool negated, bool skip) :
  server(channel.server)
{
  this->type = PermGroupTypeChannelGroup;
  this->channel = &channel;
  this->name = permName;
  this->value = permValue;
  this->negated = negated;
  this->skip = skip;
  if(permValue == "unknown") updateValue();
}

Permission::Permission(Group &group, PermissionGroupTypes permType, string permName, string permValue, bool negated, bool skip) :
  server(group.server)
{
  this->type = permType;
  this->group = &group;
  this->name = permName;
  this->value = permValue;
  this->negated = negated;
  this->skip = skip;
  if(permValue == "unknown") updateValue();
}

Permission::Permission(Channel &channel, Client &client, string permName, string permValue, bool negated, bool skip) :
  server(client.server)
{
  this->type = PermGroupTypeChannelClient;
  this->channel = &channel;
  this->client = &client;
  this->name = permName;
  this->value = permValue;
  this->negated = negated;
  this->skip = skip;
  if(permValue == "unknown") updateValue();
}

string Permission::getValue() {
  if(!checked) updateValue();
  return value;
}

Ts3Api::ts3Response Permission::updateValue() {
  checked = true;
  ts3Response response;
  switch(type) {
    case PermGroupTypeServerGroup: {
      response = server.executeCommand("servergrouppermlist sgid="+group->id+" -permsid");
      break;
    }
    case PermGroupTypeGlobalClient: {
      response = server.executeCommand("clientpermlist cldbid="+client->clientIDs.dbid+" -permsid");
      break;
    }
    case PermGroupTypeChannel: {
      response = server.executeCommand("channelpermlist cid="+channel->id+" -permsid");
      break;
    }
    case PermGroupTypeChannelGroup: {
      response = server.executeCommand("channelgrouppermlist cgid="+group->id+" -permsid");
      break;
    }
    case PermGroupTypeChannelClient: {
      response = server.executeCommand("channelclientpermlist cid="+channel->id+" cldbid="+client->clientIDs.dbid+" -permsid");
      break;
    }
    default: {
      return ts3Response();
      break;
    }
  }
  if(response.error) return response;

  size_t pos;
  if((pos = response.data.find("permsid="+name)) != string::npos) {
  	pos = response.data.find("permvalue=", pos)+11;
  	value = response.data.substr(pos, response.data.find(" ", pos)-pos);
  	pos = response.data.find("permnegated=", pos)+13;
  	negated = (response.data.substr(pos, response.data.find(" ", pos)-pos) == "1")? true : false;
  	pos = response.data.find("permskip=", pos)+10;
  	skip = (response.data.substr(pos, response.data.find(" ", pos)-pos) == "1")? true : false;
  }else {
    value = "unknown";
  }

  return response;
}

/*
switch(type) {
  case PermGroupTypeServerGroup: {

    break;
  }
  case PermGroupTypeGlobalClient: {

    break;
  }
  case PermGroupTypeChannel: {

    break;
  }
  case PermGroupTypeChannelGroup: {

    break;
  }
  case PermGroupTypeChannelClient: {

    break;
  }
  default: {
    return ts3Response();
    break;
  }
}
*/

ts3Response Permission::newValue(string value, bool permSkip, bool permNegated) {
  ts3Response response;
  switch(type) {
    case PermGroupTypeServerGroup: {
      response = server.executeCommand("servergroupaddperm sgid="+group->id+" permsid="+name+" permvalue="+value+" permnegated="+to_string(permNegated)+" permskip="+to_string(permSkip));
      break;
    }
    case PermGroupTypeGlobalClient: {
      response = server.executeCommand("clientaddperm cldbid="+client->clientIDs.dbid+" permsid="+name+" permvalue="+value+" permskip="+to_string(permSkip));
      break;
    }
    case PermGroupTypeChannel: {
      response = server.executeCommand("channeladdperm cid="+channel->id+" permsid="+name + " permvalue="+value);
      break;
    }
    case PermGroupTypeChannelGroup: {
      response = server.executeCommand("channelgroupaddperm cgid="+channel->id+" permsid="+name+" permvalue="+value);
      break;
    }
    case PermGroupTypeChannelClient: {
      response = server.executeCommand("channelclientaddperm cid="+channel->id+" cldbid="+client->clientIDs.dbid+" permsid="+name+" permvalue="+value);
      break;
    }
    default: {
      return ts3Response();
      break;
    }
  }
  if(!response.error) this->value = value;
  return response;
}

ts3Response Permission::remove() {
  ts3Response response;
  switch(type) {
    case PermGroupTypeServerGroup: {
      response = server.executeCommand("servergroupdelperm sgid="+group->id+" permsid="+name);
      break;
    }
    case PermGroupTypeGlobalClient: {
      response = server.executeCommand("clientdelperm cldbid="+client->clientIDs.dbid+" permsid="+name);
      break;
    }
    case PermGroupTypeChannel: {
      response = server.executeCommand("channeldelperm cid="+channel->id+" permsid="+name);
      break;
    }
    case PermGroupTypeChannelGroup: {
      response = server.executeCommand("channelgroupdelperm cgid="+channel->id+" permsid="+name);
      break;
    }
    case PermGroupTypeChannelClient: {
      response = server.executeCommand("channelclientdelperm cid="+channel->id+" cldbid="+client->clientIDs.dbid+" permsid="+name);
      break;
    }
    default: {
      return ts3Response();
      break;
    }
  }

  if(!response.error) value = "unknown";
  return response;
}
