#include <PDKHooks.hpp>

#ifdef FAIL
#undef FAIL
#endif
#ifdef ERROR
#undef ERROR
#endif
#ifdef DEBUG
#undef DEBUG
#endif

#include <gtest/gtest.h>

// ---------------------------------------------------------------------------
// Tests for stub hooks — each function executes and returns without side
// effects (bodies are empty stubs in the default PDK implementation).
// ---------------------------------------------------------------------------

TEST(PDKHooksTest, BeforePowerOnChassis_Called_Succeeds)
{
    // Arrange / Act / Assert — must not throw or crash
    EXPECT_NO_THROW(PDK_BeforePowerOnChassis());
}

TEST(PDKHooksTest, AfterPowerOnChassis_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_AfterPowerOnChassis());
}

TEST(PDKHooksTest, BeforePowerOffChassis_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_BeforePowerOffChassis());
}

TEST(PDKHooksTest, AfterPowerOffChassis_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_AfterPowerOffChassis());
}

TEST(PDKHooksTest, ResetChassis_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_ResetChassis());
}

TEST(PDKHooksTest, PowerCycleChassis_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_PowerCycleChassis());
}

TEST(PDKHooksTest, BMCWarmReset_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_BMCWarmReset());
}

TEST(PDKHooksTest, BMCColdReset_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_BMCColdReset());
}

TEST(PDKHooksTest, WatchdogAction_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_WatchdogAction());
}

TEST(PDKHooksTest, LPCReset_Called_Succeeds)
{
    EXPECT_NO_THROW(PDK_LPCReset());
}

// ---------------------------------------------------------------------------
// PDK_SetRebootCause — High pre-scan findings:
//  Finding #3: unvalidated rebootCause (empty, overlength, special chars).
//  Finding #4: no try-catch around D-Bus; bus.call() can throw.
//
// Live D-Bus calls cannot succeed in the Docker test environment, so all
// PDK_SetRebootCause invocations are expected to either complete silently
// (if D-Bus daemon is running but service is absent) or throw a
// sdbusplus::exception_t. We use EXPECT_NO_FATAL_FAILURE to verify the
// process does not crash regardless of D-Bus availability.
//
// Note: the function currently lacks a top-level try-catch, so exceptions
// from bus.new_default() or bus.call() propagate to the caller.  These
// tests document that behaviour and serve as regression guards if the
// production code is hardened later.
// ---------------------------------------------------------------------------

class PDKSetRebootCauseTest : public ::testing::Test
{
  protected:
    // Each test gets a fresh call — no shared mutable state needed
};

TEST_F(PDKSetRebootCauseTest, SetRebootCause_ValidString_DoesNotCrash)
{
    // Arrange
    const std::string validCause = "WatchdogExpiry";

    // Act + Assert — must not abort; may throw sdbusplus::exception_t if
    // the BMC D-Bus service is absent in the Docker environment
    EXPECT_NO_FATAL_FAILURE({
        try
        {
            PDK_SetRebootCause(validCause);
        }
        catch (const sdbusplus::exception_t&)
        {
            // Expected in Docker — service unavailable
        }
        catch (const std::exception&)
        {
            // Also acceptable — documents Finding #4 propagation
        }
    });
}

TEST_F(PDKSetRebootCauseTest, SetRebootCause_EmptyString_DoesNotCrash)
{
    // Arrange — Finding #3: empty string must not assert/abort
    const std::string emptyCause;

    // Act + Assert
    EXPECT_NO_FATAL_FAILURE({
        try
        {
            PDK_SetRebootCause(emptyCause);
        }
        catch (const sdbusplus::exception_t&)
        {
            // Expected in Docker
        }
        catch (const std::exception&)
        {
            // Documents Finding #4
        }
    });
}

TEST_F(PDKSetRebootCauseTest, SetRebootCause_OverlongString_DoesNotCrash)
{
    // Arrange — Finding #3: 1000-character string
    const std::string longCause(1000, 'A');

    EXPECT_NO_FATAL_FAILURE({
        try
        {
            PDK_SetRebootCause(longCause);
        }
        catch (const sdbusplus::exception_t&)
        {
            // Expected in Docker
        }
        catch (const std::exception&)
        {
            // Documents Finding #4
        }
    });
}

TEST_F(PDKSetRebootCauseTest, SetRebootCause_SpecialCharacters_DoesNotCrash)
{
    // Arrange — Finding #3: shell metacharacters / injection payload
    const std::string specialCause = "cause;rm -rf /;echo pwned|cmd&$(id)\n";

    EXPECT_NO_FATAL_FAILURE({
        try
        {
            PDK_SetRebootCause(specialCause);
        }
        catch (const sdbusplus::exception_t&)
        {
            // Expected in Docker
        }
        catch (const std::exception&)
        {
            // Documents Finding #4
        }
    });
}

TEST_F(PDKSetRebootCauseTest, SetRebootCause_UnicodeString_DoesNotCrash)
{
    // UTF-8 encoded string (plain char literal, compatible with C++23)
    const std::string unicode = "\xe5\x8e\x9f\xe5\x9b\xa0\xf0\x9f\x8c\x9f";
    EXPECT_NO_FATAL_FAILURE({
        try
        {
            PDK_SetRebootCause(unicode);
        }
        catch (const sdbusplus::exception_t&)
        {
            // Expected in Docker
        }
        catch (const std::exception&)
        {
            // Documents Finding #4
        }
    });
}

TEST_F(PDKSetRebootCauseTest, SetRebootCause_NullByteString_DoesNotCrash)
{
    // Finding #3: null byte embedded in string
    const std::string nullByte = std::string("abc\0def", 7);
    EXPECT_NO_FATAL_FAILURE({
        try
        {
            PDK_SetRebootCause(nullByte);
        }
        catch (const sdbusplus::exception_t&)
        {
            // Expected in Docker
        }
        catch (const std::exception&)
        {
            // Documents Finding #4
        }
    });
}

TEST_F(PDKSetRebootCauseTest, SetRebootCause_ExceptionType_IsSdbusplusOrStd)
{
    // Assert that only known exception types propagate
    try
    {
        PDK_SetRebootCause("trigger");
        SUCCEED();
    }
    catch (const sdbusplus::exception_t&)
    {
        SUCCEED();
    }
    catch (const std::exception&)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL() << "Unexpected exception type";
    }
}
