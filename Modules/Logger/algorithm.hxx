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
#ifndef MODULE_LOGGER_ALGORITHM_HXX
#define MODULE_LOGGER_ALGORITHM_HXX

#include <typedef.hxx>

namespace SHA_Logger
{
  /// @class Algo_Traits
  ///
  template <typename Algo>
  class Algo_Traits
  {
    public:
      /// Instantiate a new json writer using the stream passed as
      /// argument and write algorithm information.
      ///
      /// @return stream reference filled up with Algo object information,
      ///         error information in case of failure.
      static std::ostream& Build(std::ostream& os)
      {
        auto algo = Algo_Traits(os);
        algo.Write();

        return os;
      }

      /// Use json writer passed as parameter to write Algo information.
      ///
      /// @return stream reference filled up with Algo object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer)
      {
        Write(writer);

        return writer;
      }

    private:
      Algo_Traits(std::ostream& os) : stream(os), writer(this->stream) {}
      Algo_Traits operator=(Algo_Traits&) {}                              // Not Implemented

      bool Write() { return Write(this->writer); }

      static bool Write(Writer_Type& writer)
      {
        writer.StartObject();
        writer.Key("type");
        writer.String(GetType());
        writer.Key("version");
        writer.String(Algo::GetVersion());
        writer.Key("author");
        writer.String(Algo::GetAuthor());
        writer.Key("name");
        writer.String(Algo::GetName());
        writer.Key("module");
        writer.String(Algo::GetModule());
        writer.EndObject();

        return true;
      }

      static const std::string GetType() { return "algorithm"; }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_ALGORITHM_HXX
