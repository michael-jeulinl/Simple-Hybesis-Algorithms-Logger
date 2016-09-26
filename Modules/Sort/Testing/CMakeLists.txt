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
set(MODULE_SORT_SRCS TestMerge.cxx
                     TestPartition.cxx
                     TestQuick.cxx
                     TestRaddix.cxx)

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
cxx_gtest(TestModuleSort "${MODULE_SORT_SRCS}" ${SHA_SRCS})
