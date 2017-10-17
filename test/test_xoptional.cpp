/***************************************************************************
* Copyright (c) 2016, Johan Mabille, Sylvain Corlay and Wolf Vollprecht    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "xtl/xoptional.hpp"
#include "xtl/xoptional_sequence.hpp"

namespace xtl
{
    TEST(xoptional, scalar_tests)
    {
        // Test uninitialized == missing
        xoptional<double, bool> v0;
        ASSERT_FALSE(v0.has_value());

        // Test initialization from value
        xoptional<double, bool> v1(1.0);
        ASSERT_TRUE(v1.has_value());
        ASSERT_EQ(v1.value(), 1.0);

        // Test lvalue closure types
        double value1 = 3.0;
        int there = 0;
        auto opt1 = optional(value1, there);
        ASSERT_FALSE(opt1.has_value());
        opt1 = 1.0;
        ASSERT_TRUE(opt1.has_value());
        ASSERT_EQ(value1, 1.0);

        // Test rvalue closure type for boolean
        double value2 = 3.0;
        auto opt2 = optional(value2, true);
        opt2 = 2.0;
        ASSERT_TRUE(opt2.has_value());
        ASSERT_EQ(value2, 2.0);

        auto ptr_opt2 = &opt2;
        EXPECT_TRUE(ptr_opt2->has_value());
        EXPECT_EQ(ptr_opt2->value(), 2.0);
    }

    TEST(xoptional, string)
    {
        xoptional<std::string, bool> opt1;
        opt1 = "foo";
        ASSERT_TRUE(opt1.has_value());

        xoptional<std::string, bool> opt2 = "bar";
        ASSERT_TRUE(opt2.has_value());
    }

    TEST(xoptional, vector)
    {
        xoptional_vector<double> v(3, 2.0);
        ASSERT_TRUE(v.front().has_value());
        ASSERT_TRUE(v[0].has_value());
        ASSERT_EQ(v[0].value(), 2.0);
        v[1] = missing<double>();
        ASSERT_FALSE(v[1].has_value());
    }

    TEST(xoptional, vector_iteration)
    {
        xoptional_vector<double> v(4, 2.0);
        v[0] = missing<double>();
        std::vector<double> res;
        for (auto it = v.cbegin(); it != v.cend(); ++it)
        {
            //res.push_back((*it).value_or(0.0));
            res.push_back(it->value_or(0.0));
        }
        std::vector<double> expect = {0.0, 2.0, 2.0, 2.0};
        ASSERT_TRUE(std::equal(res.begin(), res.end(), expect.begin()));
    }

    TEST(xoptional, comparison)
    {
        ASSERT_TRUE(optional(1.0, true) == 1.0);
        ASSERT_TRUE(optional(1.0, false) == missing<double>());
        ASSERT_FALSE(missing<double>() == 1.0);
        ASSERT_TRUE(missing<double>() != 1.0);
    }

    TEST(xoptional, vector_comparison)
    {
        xoptional_vector<double> v1(4, 2.0);
        v1[0] = missing<double>();

        xoptional_vector<double> v2(4, 1.0);
        v2[0] = missing<double>();

        EXPECT_TRUE(v1 == v1);
        EXPECT_FALSE(v1 == v2);
        EXPECT_TRUE(v1 != v2);
        EXPECT_FALSE(v1 != v1);
        EXPECT_TRUE(v2 < v1);
        EXPECT_FALSE(v1 < v1);
        EXPECT_TRUE(v1 <= v1);
        EXPECT_FALSE(v1 <= v2);
        EXPECT_TRUE(v1 > v2);
        EXPECT_FALSE(v2 > v1);
        EXPECT_TRUE(v1 >= v1);
        EXPECT_FALSE(v2 >= v1);    
    }

    TEST(xoptional, io)
    {
        std::ostringstream oss;
        oss << missing<int>();
        ASSERT_EQ(oss.str(), std::string("N/A"));
    }
}
