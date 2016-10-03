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
#ifndef MODULE_LOGGER_VALUE_HXX
#define MODULE_LOGGER_VALUE_HXX

#include <Logger/typedef.hxx>
#include <Logger/value_type.hxx>

namespace SHA_Logger
{
  /// @class Value parameter
  ///
  template <typename T>
  class Value
  {
    public:
      // Assert correct JSON construction.
      ~Value() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write value information.
      ///
      /// @return stream reference filled up with Value object information,
      ///         error information in case of failure.
      static std::ostream& Build(std::ostream& os, String_Type& name, const T& value)
      {
        auto parameter = Value(os);
        parameter.Write(name, value);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with Value object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer, String_Type& name, const T& value)
      {
        Write(writer, name, value);

        return writer;
      }

    private:
      Value(std::ostream& os) : stream(os), writer(this->stream) {}
      Value operator=(Value&) {}                                    // Not Implemented

      bool Write(String_Type& name, const T& value) { return Write(this->writer, name, value); }

      static bool Write(Writer_Type& writer, String_Type& name, const T& value)
      {
         // Add Error Object log in case of failure
        // @todo temporary code: create generic error object
        if (name.empty()) {
          writer.StartObject();
          writer.Key("type");
          writer.String("error");
          writer.Key("fct");
          writer.String("value::build");
          writer.Key("message");
          writer.String("parameter name is empty.");
          writer.EndObject();
          return false;
        }

        // Add iterator information
        writer.StartObject();
        writer.Key("type");
        writer.String("value");
        writer.Key("name");
        writer.String(name);
        writer.Key("data");
        ValueType::Build<T>(writer, value);
        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_VALUE_HXX
