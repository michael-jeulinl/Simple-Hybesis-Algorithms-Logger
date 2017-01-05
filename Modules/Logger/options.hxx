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
  // @TODO add option to specify if locals and/or parameter used for recursive/other function call
  // @TODO will be used to optimize logs on such calls.
  enum Options
  {
    OpGetMin = 0x00,                            // Retrieve only logs
    OpGetName = 0x01,                           // Retrieve the name
    OpGetDoc = 0x02,                            // Retrieve the technical documentation
    OpGetInfo = 0x04,                           // Retrieve diverse information
    OpGetSrc = 0x08,                            // Retrieve the original source code
    OpIsSub = 0x16 | OpGetName,                 //! SubRoutine Call - Do not build parameter but only indexes
    OpGetAll = OpGetDoc | OpGetInfo | OpGetSrc  // Retrieve all information
  };
};

#endif() // MODULE_LOGGER_OPTIONS_HXX
