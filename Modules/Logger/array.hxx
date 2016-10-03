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
#ifndef MODULE_LOGGER_ARRAY_HXX
#define MODULE_LOGGER_ARRAY_HXX

#include <Logger/iterator.hxx>
#include <Logger/value_type.hxx>

namespace SHA_Logger
{
  /// @class Array parameter
  ///
  template <typename IteratorT>
  class Array
  {
    public:
      ~Array() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument and write array information.
      ///
      /// @return stream reference filled up with Iterator object information,
      ///         error information in case of failure.
      static std::ostream& Build(std::ostream& os, String_Type& id,
                                 String_Type& beginName, const IteratorT& begin,
                                 String_Type& endName, const IteratorT& end)
      {
        auto parameter = Array(os);
        parameter.Write(id, beginName, begin, endName, end);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with Array object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer, String_Type& id,
                                String_Type& beginName, const IteratorT& begin,
                                String_Type& endName, const IteratorT& end)
      {
        Write(writer, id, beginName, begin, endName, end);

        return writer;
      }

    private:
      Array(std::ostream& os) : stream(os), writer(this->stream) {}
      Array operator=(Array&) {}                                    // Not Implemented

      bool Write(String_Type& id,
                 String_Type& beginName, const IteratorT& begin,
                 String_Type& endName, const IteratorT& end)
      { return Write(this->writer, id, beginName, begin, endName, end); }

      static bool Write(Writer_Type& writer, String_Type& id,
                        String_Type& beginName, const IteratorT& begin,
                        String_Type& endName, const IteratorT& end)
      {
         // Add Error Object log in case of failure
        // @todo temporary code: create generic error object
        // PARAM: CreateError: std::map<string, string> for: if (empty --> error + return false)
        // Check if only macro usage retrieving fct name + line
        if (id.empty() || beginName.empty() || endName.empty()) {
          writer.StartObject();
          writer.Key("type");
          writer.String("error");
          writer.Key("fct");
          writer.String("array::build");
          writer.Key("message");
          writer.String("parameter id: '" + id + "' or name: '" + beginName + "'...missing.");
          writer.EndObject();

          return false;
        }

        // @todo create generic error object
        // 2*N for non random accessible iterator
        const int kdataSize = static_cast<int>(std::distance(begin, end));
        if (kdataSize < 1)
        {
          writer.StartObject();
          writer.Key("type");
          writer.String("error");
          writer.Key("fct");
          writer.String("array::build");
          writer.Key("message");
          writer.String("undefined or empty sequence.");
          writer.EndObject();

          return false;
        }

        // StartBuild - Main information
        writer.StartObject();
        writer.Key("type");
        writer.String("array");
        writer.Key("id");
        writer.String(id);

        // Add data
        writer.Key("data");
        ValueType::BuildArray<IteratorT>(writer, begin, end);

        // Add Iterators
        writer.Key("iterators");
        writer.StartArray();
        Iterator::Build(writer, id, beginName, 0);
        Iterator::Build(writer, id, endName, static_cast<int>(kdataSize) + 1);
        writer.EndArray();

        // Finish object
        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_ARRAY_HXX
