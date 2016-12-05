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
#ifndef MODULE_LOGGER_COMMENT_HXX
#define MODULE_LOGGER_COMMENT_HXX

#include <Logger/typedef.hxx>

namespace SHA_Logger
{
  /// @class Comment parameter
  ///
  class Comment
  {
    public:
      // Assert correct JSON construction.
      ~Comment() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write value information.
      ///
      /// @return stream reference filled up with Comment object information,
      ///         error information in case of failure.
      ///
      /// @note todo pass extent as an enumered type.
      static std::ostream& Build(std::ostream& os,
                                 String_Type& message, int level = 0, String_Type extent = "normal")
      {
        auto parameter = Comment(os);
        parameter.Write(message, level, extent);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with Comment object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer,
                                String_Type& message, int level = 0, String_Type extent = "normal")
      {
        Write(writer, message, level, extent);

        return writer;
      }

    private:
      Comment(std::ostream& os) : stream(os), writer(this->stream) {}
      Comment operator=(Comment&) {}                                    // Not Implemented

      bool Write(String_Type& message, int level, String_Type& extent)
      { return Write(this->writer, message, level, extent); }

      static bool Write(Writer_Type& writer, String_Type& message, int level, String_Type& extent)
      {
        writer.StartObject();
        writer.Key("type");
        writer.String("comment");
        writer.Key("message");
        writer.String(message);
        writer.Key("level");
        writer.Int(level);
        writer.Key("extent");
        writer.String(extent);
        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_COMMENT_HXX
