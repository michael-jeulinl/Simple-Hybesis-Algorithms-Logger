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
#ifndef MODULE_LOGGER_TYPEDEF_HXX
#define MODULE_LOGGER_TYPEDEF_HXX

// JSON lib includes
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

// STD includes
#include <string>

namespace SHA_Logger
{
  typedef rapidjson::OStreamWrapper Stream_Type;
  typedef rapidjson::PrettyWriter<Stream_Type> Writer_Type;
  typedef const std::string String_Type;
};

#endif() // MODULE_LOGGER_TYPEDEF_HXX
