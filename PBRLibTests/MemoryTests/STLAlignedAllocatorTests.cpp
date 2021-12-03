//
//  STLAlignedAllocatorTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 05/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include "../../src/Memory/STLAlignedAllocator.hpp"

#include <vector>

using namespace pbrlib;
using namespace std;

TEST(MemorySTLAlignedAllocator, AllTests)
{
    vector<float, STLAlignedAllocator<float>> tvec(12);

    pbrlib::testing::utils::equality(12, tvec.size());

    tvec.reserve(1000);

    for (size_t i{0}; i < 1000; i++) {
        tvec.push_back(1.0f);
    }

    pbrlib::testing::utils::equality(1012, tvec.size());

    tvec.resize(tvec.size() * 10000);
    tvec.resize(1);

    pbrlib::testing::utils::equality(1, tvec.size());
}
