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
set(MODULE_DATA_STRCTURES_SRCS TestBinarySearchTree.cxx)

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
cxx_gtest(TestModuleDataStructures "${MODULE_DATA_STRCTURES_SRCS}" ${SHA_SRCS})
