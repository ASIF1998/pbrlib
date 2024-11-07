#include "../utils.hpp"

#include <backend/memory/stl_aligned_allocator.hpp>

#include <vector>

using namespace pbrlib;

TEST(MemorySTLAlignedAllocator, AllTests)
{
    std::vector<float, STLAlignedAllocator<float>> vec(12);

    pbrlib::testing::utils::equality(12, vec.size());

    vec.reserve(1000);

    for (size_t i{0}; i < 1000; i++)
        vec.push_back(1.0f);

    pbrlib::testing::utils::equality(1012, vec.size());

    vec.resize(vec.size() * 10000);
    vec.resize(1);

    pbrlib::testing::utils::equality(1, vec.size());
}
