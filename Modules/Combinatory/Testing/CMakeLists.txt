###########################################################################
#
# Project: Simple Hybesis Algorithm
#
# Copyright (c) Michael Jeulin-Lagarrigue.
# All rights reserved.
#
# Author: Michael Jeulin-Lagarrigue.
#
###########################################################################

set(SHA ${PROJECT_NAME})

# Source files
set(MODULE_COMBINATORY_SRCS TestCombinations.cxx
                            TestIntersection.cxx
                            TestIsInterleaved.cxx
                            TestPermutations.cxx)

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
cxx_gtest(TestModuleCombinatory "${MODULE_COMBINATORY_SRCS}" ${SHA_SRCS})
