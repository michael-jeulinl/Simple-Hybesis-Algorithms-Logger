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
#ifndef MODULE_LOGGER_VALUE_TYPE_HXX
#define MODULE_LOGGER_VALUE_TYPE_HXX

#include <Logger/typedef.hxx>

namespace SHA_Logger
{
  /// @class ValueType
  ///
  class ValueType
  {
    public:
      // Assert correct JSON construction.
      ~ValueType() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write Value_Type information depending on type.
      ///
      /// @return stream reference filled up with Iterator object information,
      ///         error information in case of failure.
      template <typename T>
      static std::ostream& Build(std::ostream& os, const T& value)
      {
        // Create ValueType and add it to the logger
        auto logger = ValueType(os);
        logger.Write(value);

        return os;
      }

      /// Use json writer passed as parameter to write Value_Type information.
      ///
      /// @return stream reference filled up with Array object information depending on type,
      ///         error information in case of failure.
      template <typename T>
      static Writer_Type& Build(Writer_Type& writer, const T& value)
      {
        Write(writer, value);

        return writer;
      }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write array information depending on type.
      ///
      /// @return stream reference filled up with Iterator object information,
      ///         error information in case of failure.
      template <typename IteratorT>
      static std::ostream& BuildArray(std::ostream& os, const IteratorT& begin, const IteratorT& end)
      {
        // Create ValueType logger
        auto logger = ValueType(os);

        // @todo check & log error? --> time consuming
        logger.writer.StartArray();
        for (auto it = begin; it != end; ++it)
          logger.Write(*it);
        logger.writer.EndArray();

        return os;
      }

      /// Use json writer passed as parameter to write array of value_type information.
      ///
      /// @return stream reference filled up with Array object information depending on type,
      ///         error information in case of failure.
      template <typename IteratorT>
      static Writer_Type& BuildArray(Writer_Type& writer, const IteratorT& begin, const IteratorT& end)
      {
        // @todo check & log error? --> time consuming
        writer.StartArray();
        for (auto it = begin; it != end; ++it)
          Write(writer, *it);
        writer.EndArray();

        return writer;
      }

    private:
      ValueType(std::ostream& os) : stream(os), writer(this->stream) {}
      ValueType operator=(ValueType&) {}                                  // Not Implemented

      // Wrapper using internal writer
      template <typename T>
      bool Write(T value) { return this->Write(this->writer, value); }

      // Specifications
      static bool Write(Writer_Type& writer, char value) { return writer.String(std::string(1, value)); }
      static bool Write(Writer_Type& writer, bool value) { return writer.Bool(value); }
      static bool Write(Writer_Type& writer, double value) { return writer.Double(value); }
      static bool Write(Writer_Type& writer, int value) { return writer.Int(value); }
      static bool Write(Writer_Type& writer, int64_t value) { return writer.Int64(value); }
      static bool Write(Writer_Type& writer, const std::string& value) { return writer.String(value); }
      static bool Write(Writer_Type& writer, unsigned value) { return writer.Uint(value); }
      static bool Write(Writer_Type& writer, uint64_t value) { return writer.Uint64(value); }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_VALUE_TYPE_HXX
