
#include <PDKHooks.hpp>

#include <iostream>

/*-------------------------------------------------------------------------
 * @fn PDK_BeforePowerOnChassis
 * @brief This function is called by the core before performing power on chassis
 *------------------------------------------------------------------------*/
void PDK_BeforePowerOnChassis()
{
    // lg2::info("PDK BeforePowerOnChassis is invoked\n");
    return;
}

/*-------------------------------------------------------------------------
 * @fn PDK_AfterPowerOnChassis
 * @brief This function is called by the core after performing power on chassis
 *------------------------------------------------------------------------*/
void PDK_AfterPowerOnChassis()
{
    // lg2::info("PDK AfterPowerOnChassis is invoked\n");
    return;
}
/*-------------------------------------------------------------------------
 * @fn PDK_BeforePowerOffChassis
 * @brief This function is called by the core before performing power off
 *chassis
 *------------------------------------------------------------------------*/
void PDK_BeforePowerOffChassis()
{
    // lg2::info("PDK BeforePowerOffChassis is invoked\n");
    return;
}

/*-------------------------------------------------------------------------
 * @fn PDK_AfterPowerOffChassis
 * @brief This function is called by the core after performing power off chassis
 *------------------------------------------------------------------------*/
void PDK_AfterPowerOffChassis()
{
    // lg2::info("PDK AfterPowerOffChassis is invoked\n");
    return;
}

/*--------------------------------------------------------------------
 * @fn PDK_ResetChassis
 * @brief This function is called by the core reset
 *--------------------------------------------------------------------*/
void PDK_ResetChassis()
{
    // lg2::info("PDK ResetChassis is invoked\n");
    return;
}

/*-------------------------------------------------------------------------
 * @fn PDK_PowerCycleChassis
 * @brief This function is called by the core before performing power cycle
 *chassis
 *------------------------------------------------------------------------*/
void PDK_PowerCycleChassis()
{
    // lg2::info("PDK PowerCycleChassis is invoked\n");
    return;
}

/*-------------------------------------------------------------------------
 * @fn PDK_BeforeBMCWarmReset
 * @brief This function is called by the core warm reset function
 *------------------------------------------------------------------------*/
void PDK_BMCWarmReset()
{
    // lg2::info("PDK BMCWarmReset is invoked\n");
    return;
}

/*-------------------------------------------------------------------------
 * @fn PDK_BeforeBMCColdReset
 * @brief This function is called by the core cold reset function
 *------------------------------------------------------------------------*/
void PDK_BMCColdReset()
{
    // lg2::info("PDK BMCColdReset is invoked\n");
    return;
}

/*----------------------------------------------------------------------
 * @fn PDK_WatchdogAction
 * @brief This function is used to do custom actions on watchdog expiry
 * @returns nothing
 *----------------------------------------------------------------------*/
void PDK_WatchdogAction()
{
    // lg2::info("PDK WatchdogAction is invoked\n");
    //  Do custom actions
    return;
}

/**
 * @fn PDK_SetRebootCause
 * @brief This function is called to set reboot cause
 * @param RebootCause - Cause for the BMC Reboot
 */
void PDK_SetRebootCause(const std::string& rebootCause)
{
    // lg2::info("PDK SetRebootCause is invoked\n");
    auto bus = sdbusplus::bus::new_default();
    auto method = bus.new_method_call("xyz.openbmc_project.State.BMC",
                                      "/xyz/openbmc_project/state/bmc0",
                                      "org.freedesktop.DBus.Properties", "Set");
    method.append("xyz.openbmc_project.State.BMC", "LastRebootCause");
    method.append(std::variant<std::string>(rebootCause));

    auto reply = bus.call(method);

    if (reply.is_method_error())
    {
        std::cerr << "Failed to set rebootCause reason \n";
    }

    return;
}

/*-------------------------------------------------------------------------
 * @fn PDK_LPCReset
 * @brief This function is called by the core during LPC Reset
 *------------------------------------------------------------------------*/
void PDK_LPCReset()
{
    // lg2::info("PDK LPC Reset is invoked\n");
    return;
}
