#pragma once
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <phosphor-logging/lg2.hpp>
#include <phosphor-logging/log.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/bus.hpp>

#include <functional>
#include <iostream>
#include <map>
#include <variant>

// PDKHook Function Declaration
void PDK_BeforePowerOnChassis();
void PDK_AfterPowerOnChassis();
void PDK_BeforePowerOffChassis();
void PDK_AfterPowerOffChassis();
void PDK_ResetChassis();
void PDK_PowerCycleChassis();

void PDK_BMCWarmReset();
void PDK_BMCColdReset();
void PDK_WatchdogAction();
void PDK_SetRebootCause(const std::string& rebootCause);
void PDK_LPCReset();
