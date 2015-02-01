// Copyright (C) 2015 FourC AS, http://www.fourc.eu/
// All Rights Reserved.
//
// This file is part of PngChecker.
//
// PngChecker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// PngChecker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with PngChecker.  If not, see <http://www.gnu.org/licenses/>.

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
