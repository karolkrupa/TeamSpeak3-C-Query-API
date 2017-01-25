#include <Group.hpp>

using namespace Ts3Api;

Channel::channelInfoProperties::channelInfoProperties(string &id, Server& server, time_t& updateTime, bool incomleteInit) : id(id), ts3ObjectProperties(server, updateTime, incomleteInit) {

}

bool Channel::good() {
  auto response = server.executeCommand("channelinfo cid="+id);

  if(response.error) return false;

  channelInfo.data = response.data;
  return true;
}

Channel::channelChangeableParam::channelChangeableParam(Channel *channel, string name, string value) :
  channel(channel)
{
  this->name = name;
  this->value = value;
}

ts3Response Channel::channelChangeableParam::change(string value) {
  if(channel != NULL)
    return channel->server.executeCommand("channeledit cid="+channel->id+" "+name+"="+messageEncode(value));
  else
    return ts3Response();
}

void Channel::channelInfoProperties::update() {
  auto response = server.executeCommand("channelinfo cid="+id);

  if(incompleteInit) incompleteInit = false;
  if(!response.error) data = response.data;
  updatedTime = updateTime+1;
}

Channel::Channel(Server &server, string id, bool incomleteInit) :
  server(server),
  id(id),
  channelInfo(channelInfoProperties(this->id, server, updateTime, incomleteInit))
{

}

void Channel::update() {
  updateTime = time(NULL);
}


Channel::channelChangeableParam Channel::getPid() {
  return channelChangeableParam(this, "pid" ,channelInfo.getProperty("pid").value);
}
Channel::channelName Channel::getName() {
  channelName name;
  name.name = channelChangeableParam(this, "channel_name" ,channelInfo.getProperty("channel_name").value);
  name.phonetic = channelChangeableParam(this, "channel_name_phonetic" ,channelInfo.getProperty("channel_name_phonetic").value);
  return name;
}
Channel::channelChangeableParam Channel::getTopic() {
  return channelChangeableParam(this, "channel_topic" ,channelInfo.getProperty("channel_topic").value);
}
Channel::channelChangeableParam Channel::getDescription() {
  return channelChangeableParam(this, "channel_description" ,channelInfo.getProperty("channel_description").value);
}
Channel::channelChangeableParam Channel::getPassword() {
  return channelChangeableParam(this, "channel_password" ,channelInfo.getProperty("channel_password").value);
}
Channel::channelCodec Channel::getCodec() {
  channelCodec codec;
  codec.codec = channelChangeableParam(this, "channel_codec" ,channelInfo.getProperty("channel_codec").value);
  codec.codec_quality = channelChangeableParam(this, "channel_codec_quality" ,channelInfo.getProperty("channel_codec_quality").value);
  codec.codec_latency_factor = channelChangeableParam(this, "channel_codec_latency_factor" ,channelInfo.getProperty("channel_codec_latency_factor").value);
  codec.codec_is_unencrypted = channelChangeableParam(this, "channel_codec_is_unencrypted" ,channelInfo.getProperty("channel_codec_is_unencrypted").value);

  return codec;
}
Channel::channelChangeableParam Channel::getMaxClients() {
  return channelChangeableParam(this, "channel_maxclients" ,channelInfo.getProperty("channel_maxclients").value);
}
Channel::channelChangeableParam Channel::getMaxFamilyClients() {
  return channelChangeableParam(this, "channel_maxfamilyclients" ,channelInfo.getProperty("channel_maxfamilyclients").value);
}
Channel::channelChangeableParam Channel::getOrder() {
  return channelChangeableParam(this, "channel_order" ,channelInfo.getProperty("channel_order").value);
}
Channel::channelFlags Channel::getFlags() {
  channelFlags flags;
  flags.flag_permanent = channelChangeableParam(this, "channel_flag_permanent" ,channelInfo.getProperty("channel_flag_permanent").value);
  flags.flag_semi_permanent = channelChangeableParam(this, "channel_flag_semi_permanent" ,channelInfo.getProperty("channel_flag_semi_permanent").value);
  flags.flag_default = channelChangeableParam(this, "channel_flag_default" ,channelInfo.getProperty("channel_flag_default").value);
  flags.flag_password = channelChangeableParam(this, "channel_flag_password" ,channelInfo.getProperty("channel_flag_password").value);
  flags.flag_maxclients_unlimited = channelChangeableParam(this, "channel_flag_maxclients_unlimited" ,channelInfo.getProperty("channel_flag_maxclients_unlimited").value);
  flags.flag_maxfamilyclients_unlimited = channelChangeableParam(this, "channel_flag_maxfamilyclients_unlimited" ,channelInfo.getProperty("channel_flag_maxfamilyclients_unlimited").value);
  flags.flag_maxfamilyclients_inherited = channelChangeableParam(this, "channel_flag_maxfamilyclients_inherited" ,channelInfo.getProperty("channel_flag_maxfamilyclients_inherited").value);
  flags.flag_private = channelChangeableParam(this, "channel_flag_private" ,channelInfo.getProperty("channel_flag_private").value);

  return flags;
}
property Channel::getSecuritySalt() {
  return channelInfo.getProperty("channel_security_salt");
}
property Channel::getDeleteDelay() {
  return channelInfo.getProperty("channel_delete_delay");
}
property Channel::getFilepath() {
  return channelInfo.getProperty("channel_filepath");
}
Channel::channelChangeableParam Channel::getNeedTalkPower() {
  return channelChangeableParam(this, "channel_needed_talk_power" ,channelInfo.getProperty("channel_needed_talk_power").value);
}
property Channel::getForcedSilence() {
  return channelInfo.getProperty("channel_forced_silence");
}
Channel::channelChangeableParam Channel::getIconId() {
  return channelChangeableParam(this, "channel_icon_id" ,channelInfo.getProperty("channel_icon_id").value);
}
property Channel::getSecondsEmpty() {
  return channelInfo.getProperty("seconds_empty");
}
