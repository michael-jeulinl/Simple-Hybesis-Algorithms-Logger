#include <gtest/gtest.h>
#include <algorithm.hxx>

using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {
  class Algo_Quick
  {
    public:
      static const std::string GetAuthor() { return "Michael Jeulin-L"; }
      static const std::string GetDoc() { return "Documentation."; }
      static const std::string GetModule() { return "Sort"; }
      static const std::string GetName() { return "QuickSort"; }
      static const std::string GetVersion() { return "1.0"; }
  };
}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestAlgo_Traits, build)
{
  // @todo Passing arguement as: Algo_Quick::Build(std::cout, x1, x2...)
  // { Algo_Traits<Algo_Quick>::Build() };
  Algo_Traits<Algo_Quick>::Build(std::cout);
}
