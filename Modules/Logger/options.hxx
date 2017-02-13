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
#ifndef MODULE_LOGGER_OPTIONS_HXX
#define MODULE_LOGGER_OPTIONS_HXX

namespace SHA_Logger
{
  enum Options
  {
    OpGetMin = 0x00,
    OpGetDoc = 0x01,
    OpGetInfo = 0x02,
    OpGetSrc = 0x04,
    OpGetAll = OpGetDoc | OpGetInfo | OpGetSrc
  };
}

#endif // MODULE_LOGGER_OPTIONS_HXX
