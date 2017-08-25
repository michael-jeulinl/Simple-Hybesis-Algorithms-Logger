/*===========================================================================================================
 *
 * SHA-L - Simple Hybesis Algorithm Logger
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms-Logger/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#include <gtest/gtest.h>

#include <maze_sidewinder_log.hxx>

// STD includes
#include <fstream>

// Testing namespace
using namespace SHA_Logger;

#ifndef DOXYGEN_SKIP
namespace {}
#endif /* DOXYGEN_SKIP */

// Test TestAlgo Construction
TEST(TestMazeSidewinderLog, build)
{
  OFStream fileStream("maze_sidewinder_output.json");

  // Build Maze
  // @todo check if should return the maze object
  MazeSidewinderLog::Build(fileStream, OpGetAll, 30, 30);
}
