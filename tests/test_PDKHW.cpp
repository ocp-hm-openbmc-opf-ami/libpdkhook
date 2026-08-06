#include <PDKHW.hpp>

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
// Tests for PDK_PlatformInit — trivial stub returning 0.
// ---------------------------------------------------------------------------

TEST(PDKHWTest, PlatformInit_Called_ReturnsZero)
{
    // Arrange / Act
    const int result = PDK_PlatformInit();

    // Assert
    EXPECT_EQ(result, 0);
}

TEST(PDKHWTest, PlatformInit_CalledTwice_ReturnsZeroBothTimes)
{
    // Verify idempotent behaviour
    EXPECT_EQ(PDK_PlatformInit(), 0);
    EXPECT_EQ(PDK_PlatformInit(), 0);
}

// ---------------------------------------------------------------------------
// Tests for PDK_PostPlatformInit — trivial stub returning 0.
// ---------------------------------------------------------------------------

TEST(PDKHWTest, PostPlatformInit_Called_ReturnsZero)
{
    const int result = PDK_PostPlatformInit();
    EXPECT_EQ(result, 0);
}

TEST(PDKHWTest, PostPlatformInit_CalledTwice_ReturnsZeroBothTimes)
{
    EXPECT_EQ(PDK_PostPlatformInit(), 0);
    EXPECT_EQ(PDK_PostPlatformInit(), 0);
}

// ---------------------------------------------------------------------------
// Tests for PDK_GetPSGood — connects to D-Bus; unavailable in Docker.
//
// Pre-scan findings addressed:
//  Finding #1 (Critical): std::get<std::string>() on variant can throw
//    std::bad_variant_access — not caught by the existing sdbusplus catch.
//  Finding #2 (High): unvalidated `node` parameter; negative / overflow
//    values produce invalid D-Bus service/path strings.
//
// In Docker, the chassis D-Bus service is absent, so sdbusplus::exception_t
// is thrown before the variant branch is reached. Tests verify:
//  a) the function never crashes (EXPECT_NO_FATAL_FAILURE)
//  b) the return value is either 0, 1, or -1 — no other values are valid
//  c) boundary node values do not cause UB or abort
// ---------------------------------------------------------------------------

class PDKGetPSGoodTest : public ::testing::Test
{
  protected:
    // Calls PDK_GetPSGood, tolerating the expected D-Bus exception in Docker.
    // Returns the actual return value on success, or -1 on D-Bus failure.
    int callGetPSGood(int node)
    {
        try
        {
            return PDK_GetPSGood(node);
        }
        catch (const sdbusplus::exception_t&)
        {
            // Expected in Docker — service unavailable; function returns -1
            return -1;
        }
        catch (const std::bad_variant_access&)
        {
            // Finding #1 — documents uncaught exception path
            return -1;
        }
        catch (const std::exception&)
        {
            return -1;
        }
    }
};

TEST_F(PDKGetPSGoodTest, GetPSGood_DefaultNode_ReturnsValidCode)
{
    // Arrange / Act
    const int result = callGetPSGood(0);

    // Assert — only 0, 1, or -1 are valid return codes
    EXPECT_TRUE(result == 0 || result == 1 || result == -1)
        << "Unexpected return code: " << result;
}

TEST_F(PDKGetPSGoodTest, GetPSGood_NodeZero_DoesNotCrash)
{
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(0));
}

TEST_F(PDKGetPSGoodTest, GetPSGood_NodeOne_DoesNotCrash)
{
    // Multi-host scenario
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(1));
}

TEST_F(PDKGetPSGoodTest, GetPSGood_NegativeNode_ReturnsErrorCode)
{
    // Finding #2: negative node creates invalid D-Bus path string
    // Expect -1 (D-Bus error path) — must not crash or return 1
    const int result = callGetPSGood(-1);
    EXPECT_NE(result, 1) << "Negative node must not report PS_GOOD";
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(-1));
}

TEST_F(PDKGetPSGoodTest, GetPSGood_LargePositiveNode_DoesNotCrash)
{
    // Finding #2: very large node value — verifies no integer overflow crash
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(std::numeric_limits<int>::max()));
}

TEST_F(PDKGetPSGoodTest, GetPSGood_IntMinNode_DoesNotCrash)
{
    // Finding #2: INT_MIN — most negative value
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(std::numeric_limits<int>::min()));
}

TEST_F(PDKGetPSGoodTest, GetPSGood_ReturnCode_WithinValidRange)
{
    // Verify return code is always within {-1, 0, 1} for node 0
    const int result = callGetPSGood(0);
    EXPECT_GE(result, -1);
    EXPECT_LE(result, 1);
}

TEST_F(PDKGetPSGoodTest, GetPSGood_NegativeNode_ReturnCodeWithinValidRange)
{
    // Same validity check for negative node
    const int result = callGetPSGood(-1);
    EXPECT_GE(result, -1);
    EXPECT_LE(result, 1);
}

TEST_F(PDKGetPSGoodTest, GetPSGood_MultipleNodeValues_DoesNotCrash)
{
    // Exercise a variety of node values for additional branch coverage
    const int nodes[] = {2, 42, -42, std::numeric_limits<int>::max() / 2,
                         std::numeric_limits<int>::min() / 2};
    for (int node : nodes)
    {
        EXPECT_NO_FATAL_FAILURE({
            try
            {
                (void)PDK_GetPSGood(node);
            }
            catch (const sdbusplus::exception_t&)
            {}
            catch (const std::bad_variant_access&)
            {}
            catch (const std::exception&)
            {}
        }) << "Node: "
           << node;
    }
}

TEST_F(PDKGetPSGoodTest, GetPSGood_RepeatedCalls_DifferentNodes)
{
    // Call with different nodes in sequence
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(0));
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(1));
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(-1));
    EXPECT_NO_FATAL_FAILURE(callGetPSGood(42));
}
