#include <gtest/gtest.h>
#include <algorithm.hxx>

// STD includes
#include <string>

using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  typedef std::unique_ptr<Algo> Algo_Type;
}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestAlgo, build)
{
  // Empty Array - No BST should be built
  {
    auto algoLog = Algo::Build("test");
  }
}
