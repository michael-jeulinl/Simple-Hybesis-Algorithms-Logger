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

project(SHAModuleLogger)

# Source files
set(SHA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}")

# Testing
if(BUILD_TESTING_LOG)
  add_subdirectory(TestingLog)
endif()
