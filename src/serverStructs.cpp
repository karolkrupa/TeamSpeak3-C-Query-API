#include <structs/serverStructs.hpp>

using namespace Ts3Api;

error::error(string msg, int id) {
  this->msg = msg;
  this->id = id;
}

socketData::socketData(string data, int bytes) {
  this->data = data;
  this->bytes = bytes;
}

ts3Response::ts3Response(string data, string errorId, string errorMsg) {
  this->data = data;
  this->errorId = errorId;
  this->errorMsg = messageDecode(errorMsg);

  if(errorId == "0") error = false;
}

string ts3Response::messageDecode(string message) {
  return Ts3Api::messageDecode(message);
}

string ts3Response::messageEncode(string message) {
  return Ts3Api::messageEncode(message);
}
