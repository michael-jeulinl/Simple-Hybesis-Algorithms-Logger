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
#ifndef MODULE_LOGGER_ITERATOR_HXX
#define MODULE_LOGGER_ITERATOR_HXX

#include <Logger/typedef.hxx>

namespace SHA_Logger
{
  /// @class Iterator parameter
  ///
  class Iterator
  {
    public:
      // Assert correct JSON construction.
      ~Iterator() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write iterator information.
      ///
      /// @return stream reference filled up with Iterator object information,
      ///         error information in case of failure.
      static std::ostream& Build(std::ostream& os, String_Type& parentId, String_Type& name, int index)
      {
        auto parameter = Iterator(os);
        parameter.Write(parentId, name, index);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with Iterator object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer, String_Type& parentId, String_Type& name, int index)
      {
        Write(writer, parentId, name, index);

        return writer;
      }

    private:
      Iterator(std::ostream& os) : stream(os), writer(this->stream) {}
      Iterator operator=(Iterator&) {}                                  // Not Implemented

      bool Write(String_Type& parentId, String_Type& name, int index)
      { return Write(this->writer, parentId, name, index); }

      static bool Write(Writer_Type& writer, String_Type& parentId, String_Type& name, int index)
      {
         // Add Error Object log in case of failure
        // @todo temporary code: create generic error object
        if (parentId.empty() || name.empty()) {
          writer.StartObject();
          writer.Key("type");
          writer.String("error");
          writer.Key("fct");
          writer.String("iterator::build");
          writer.Key("message");
          writer.String("parameter parentId: '" + parentId + "' or name: '" + name + "' missing.");
          writer.EndObject();
          return false;
        }

        // Add iterator information
        writer.StartObject();
        writer.Key("type");
        writer.String("iterator");
        writer.Key("id");
        writer.String(parentId + "_" + name); // Create id model
        writer.Key("name");
        writer.String(name);
        writer.Key("ref");
        writer.String(parentId);
        writer.Key("data");
        writer.Int(index);
        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_ITERATOR_HXX
