//
//  STLAlignedAllocatorTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 05/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/Memory/STLAlignedAllocator.hpp"

#include <vector>

using namespace testing;
using namespace pbrlib;
using namespace std;

TEST(MemorySTLAlignedAllocator, AllTests)
{
    vector<float, STLAlignedAllocator<float>> tvec(12);

    EXPECT_EQ(12, tvec.size());

    tvec.reserve(1000);

    for (size_t i{0}; i < 1000; i++) {
        tvec.push_back(1.0f);
    }

    EXPECT_EQ(1012, tvec.size());

    tvec.resize(tvec.size() * 10000);
    tvec.resize(1);

    EXPECT_EQ(1, tvec.size());
}
