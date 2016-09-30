#include <gtest/gtest.h>
#include <array.hxx>

// STD includes
#include <string>

using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
}
#endif /* DOXYGEN_SKIP */

// Test Array
TEST(TestArray, build)
{
  // Simple Iterator
  {
    std::vector<int> sequence;
    sequence.push_back(2);
    sequence.push_back(-1);
    sequence.push_back(5);
    sequence.push_back(7);
    sequence.push_back(0);
    Array<std::vector<int>::iterator>::Build(std::cout, "p_0", "begin", sequence.begin(), "end", sequence.end());
  }
}
