#include <gtest/gtest.h>
#include <iterator.hxx>

// STD includes
#include <string>

using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {}
#endif /* DOXYGEN_SKIP */

// Test TestIterator
TEST(TestIterator, build)
{
  // Simple Iterator
  {
    Iterator::Build(std::cout, "p_0", "it1", 0);
  }
}
