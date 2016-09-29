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
set(MODULE_LOGGER_SRCS #TestAlgorithm.cxx
                       #TestArray.cxx
                       TestIterator.cxx
                       #TestParameter.cxx
                       )

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
include_directories(${LIB_DIR})
cxx_gtest(TestModuleLogger "${MODULE_LOGGER_SRCS}" ${SHA_SRCS})
