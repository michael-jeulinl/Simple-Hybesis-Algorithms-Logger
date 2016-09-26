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

project(SHAModuleDataStructures)

# Source files
set(SHA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}")

# Testing
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()
