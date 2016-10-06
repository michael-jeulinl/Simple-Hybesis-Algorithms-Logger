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
#ifndef MODULE_LOGGER_OPERATION_HXX
#define MODULE_LOGGER_OPERATION_HXX

#include <Logger/typedef.hxx>
#include <Logger/value_type.hxx>

namespace SHA_Logger
{
  namespace
  {
    static const std::string kTypeName = "operation"; // Name used as type for all operations name
  }

  /// @class Operation
  ///
  class Operation
  {
    public:
      // Assert correct JSON construction.
      ~Operation() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write value information.
      ///
      /// @return stream reference filled up with Operation object information,
      ///         error information in case of failure.
      ///
      /// @todo pass extent as an enumered type.
      template <typename T>
      static std::ostream& Set(std::ostream& os, String_Type& name, const T& value)
      {
        auto operation = Operation(os);
        operation.WriteSet(name, value);

        return os;
      }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write value information.
      ///
      /// @return stream reference filled up with Operation object information,
      ///         error information in case of failure.
      ///
      /// @todo pass extent as an enumered type.
      template <typename T>
      static std::ostream& OffSet(std::ostream& os, String_Type& name, const T& offset)
      {
        auto operation = Operation(os);
        operation.WriteOffSet(name, offset);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with Operation object information,
      ///         error information in case of failure.
      template <typename T>
      static Writer_Type& Set(Writer_Type& writer, String_Type& name, const T& value)
      {
        WriteSet(writer, name, value);

        return writer;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with Operation object information,
      ///         error information in case of failure.
      template <typename T>
      static Writer_Type& OffSet(Writer_Type& writer, String_Type& name, const T& offset)
      {
        WriteOffSet(writer, name, offset);

        return writer;
      }

    private:
      Operation(std::ostream& os) : stream(os), writer(this->stream) {}
      Operation operator=(Operation&) {}                                    // Not Implemented

      template <typename T>
      bool WriteSet(String_Type& name, const T& value)
      { return WriteSet(this->writer, name, value); }

      template <typename T>
      bool WriteOffSet(String_Type& name, const T& offset)
      { return WriteOffSet(this->writer, name, offset); }

      template <typename T>
      static bool WriteSet(Writer_Type& writer, String_Type& name, const T& value)
      {
        writer.StartObject();
        writer.Key("type");
        writer.String(kTypeName);
        writer.Key("name");
        writer.String("Set");
        writer.Key("var");
        writer.String(name);
        writer.Key("data");
        ValueType::Build<T>(writer, value);
        writer.EndObject();

        return true;
      }

      template <typename T>
      static bool WriteOffSet(Writer_Type& writer, String_Type& name, const T& offset)
      {
        writer.StartObject();
        writer.Key("type");
        writer.String(kTypeName);
        writer.Key("name");
        writer.String("OffSet");
        writer.Key("var");
        writer.String(name);
        writer.Key("data");
        ValueType::Build<T>(writer, offset);
        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_OPERATION_HXX
