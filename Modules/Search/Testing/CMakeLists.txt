###########################################################################
#
# Project: Simple Hybesis Algorithm Logger
#
# Copyright (c) Michael Jeulin-Lagarrigue.
# All rights reserved.
#
# Author: Michael Jeulin-Lagarrigue.
#
###########################################################################

set(SHA ${PROJECT_NAME})

# Source files
set(MODULE_SEARCH_SRCS TestBinary.cxx
                       TestKthMaxElement.cxx
                       TestMaxDistance.cxx
                       TestMaxMElements.cxx
                       TestMaxSubSequence.cxx)

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
include_directories(${MODULES_DIR})
cxx_gtest(TestModuleSearch "${MODULE_SEARCH_SRCS}" ${SHA_SRCS})
