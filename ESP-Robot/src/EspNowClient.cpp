#include "EspNowClient.hpp"

std::unordered_map<PacketID, std::function<void(void*)>> EspNowClient::s_handlers;