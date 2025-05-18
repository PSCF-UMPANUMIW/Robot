#include "EspNowClient.hpp"

std::unordered_map<PacketID, EspNowClient::HandlerBase> EspNowClient::s_handlers;