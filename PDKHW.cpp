
#include <PDKHW.hpp>
#include <phosphor-logging/lg2.hpp>

#include <iostream>

/*----------------------------------------------------------------------------
 * @fn PDK_PlatformInit
 *
 * @brief This function should be used to perform any platform specific
 * initialization.
 * @return      0  if success
 *                      -1 if error
 *---------------------------------------------------------------------------*/
int PDK_PlatformInit()
{
    // lg2::info("PDK_PlatformInit is invoked");
    return 0;
}

/*-----------------------------------------------------------------
 * @fn PDK_PostPlatformInit
 *
 * @brief This function should be used to perform any post platform specific
 * initialization.
 * @return  0  if success
 *          -1 if error
 *-----------------------------------------------------------------*/
int PDK_PostPlatformInit()
{
    // lg2::info("PDK_PostPlatformInit is invoked");
    return 0;
}

/*--------------------------------------------------------------------
 * @fn PDK_GetPSGood
 * @brief Returns the current PS_GOOD status of the power supply.
 * @param node   Host number (default is 0 for single-host systems; for
 *  multi-host systems, use the appropriate host number).
 *  return       1  PS_GOOD is asserted.
 *               0 PS_GOOD is not asserted.
 *              -1 Error
 *--------------------------------------------------------------------*/
int PDK_GetPSGood(int node)
{
    // lg2::info("PDK_GetPSGood is invoked");
    int pwrGood = 0;
    std::variant<std::string> pwrStatus;
    std::string value;
    auto bus = sdbusplus::bus::new_default();
    std::string pwrService =
        "xyz.openbmc_project.State.Chassis" + std::to_string(node);
    std::string pwrObjPath =
        "/xyz/openbmc_project/state/chassis" + std::to_string(node);
    try
    {
        auto method =
            bus.new_method_call(pwrService.c_str(), pwrObjPath.c_str(),
                                "org.freedesktop.DBus.Properties", "Get");
        method.append("xyz.openbmc_project.State.Chassis", "CurrentPowerState");
        auto reply = bus.call(method);
        reply.read(pwrStatus);
        value = std::get<std::string>(pwrStatus);
    }
    catch (sdbusplus::exception_t& e)
    {
        lg2::error("Failed to get pwrGood Value");
        return -1;
    }
    if (value == "xyz.openbmc_project.State.Chassis.PowerState.On")
    {
        pwrGood = 1;
    }
    return pwrGood;
}
