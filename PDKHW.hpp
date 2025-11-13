#pragma once
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <phosphor-logging/log.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/bus.hpp>

#include <functional>
#include <iostream>
#include <map>
#include <variant>

// PDKHook Function Declaration
int PDK_PlatformInit();
int PDK_PostPlatformInit();
int PDK_GetPSGood(int node = 0);
