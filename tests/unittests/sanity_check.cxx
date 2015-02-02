/**
 * Verifies that the testing system gives some sensible results
 */

#include <gtest/gtest.h>
 
TEST(SanityCheck, SanityCheck) // Declares a test named "sanity_check"
{
    EXPECT_TRUE(true); // We certainly hope that true is true
    EXPECT_EQ(2, 1 + 1); // The value 1+1 should equal 2
    
    double a = 1.51;
    double b = 1.52;
    EXPECT_NEAR(a, b, 0.1); // These equal within 0.1
}
