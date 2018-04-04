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
#ifndef MODULE_SEARCH_BINARY_LOG_HXX
#define MODULE_SEARCH_BINARY_LOG_HXX

#include <Logger/algorithm.hxx>
#include <Logger/array.hxx>
#include <Logger/comment.hxx>
#include <Logger/operation.hxx>
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>

namespace SHA_Logger
{
  /// @class BinaryLog
  ///
  template <typename IT, typename T, typename IsEqualT>
  class BinaryLog
  {
    public:
      static const String GetName() { return "Binary Search"; }

      // Assert correct JSON construction.
      ~BinaryLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with BinaryLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const IT& begin, const IT& end, const T& key)
      {
        std::unique_ptr<BinaryLog> builder = std::unique_ptr<BinaryLog>(new BinaryLog(os));
        builder->Write(opts, begin, end, key);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with BinaryLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const IT& begin, const IT& end, const T& key)
      {
        Write(writer, opts, begin, end, key);

        return writer;
      }

    private:
      BinaryLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                               writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      BinaryLog operator=(BinaryLog&) {} // Not Implemented

      bool Write(Options opts, const IT& begin, const IT& end, const T& key)
      { return Write(*this->writer, opts, begin, end, key); }

      static bool Write(Writer& writer, Options opts, const IT& begin, const IT& end, const T& key)
      {
        writer.StartObject();

        Algo_Traits<BinaryLog>::Build(writer, opts);  // Write description
        WriteParameters(writer, begin, end, key);     // Write parameters
        WriteComputation(writer, begin, end, key);    // Write computation

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer& writer, const IT& begin, const IT& end, const T& key)
      {
        writer.Key("parameters");
          writer.StartArray();
          Array<IT>::Build(writer, kSeqName, "begin", begin, "end", end);
          Value<T>::Build(writer, "key", key);
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer& writer, const IT& begin, const IT& end, const T& key)
      {
        // Not part of the logs
        const auto seqSize = static_cast<int>(std::distance(begin, end));
        auto lowIt = begin;
        auto highIt = end;
        int middleIdx = seqSize / 2;

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
          auto index = Value<int>::BuildValue(writer, "index", -1);
          auto middleIt =
            Iterator::BuildIt<IT>(writer, kSeqName, "it", middleIdx, lowIt + middleIdx);
        writer.EndArray();

        // Log algorithm operations
        writer.Key("logs");
        writer.StartArray();
        while (lowIt < highIt && index < 0)
        {
          middleIdx = static_cast<int>(std::distance(begin, middleIt));
          Operation::Set<int>(writer, "it", middleIdx);

          if (IsEqualT()(key, *middleIt))
          {
            index = middleIdx;

            Comment::Build(writer,
              "Key {" + std::to_string(key) + "} Found at index [" + std::to_string(index) + "]", 1);
            Operation::Set<int>(writer, "index", index);
            break;
          }
          else if (key > *middleIt)
          {
            lowIt = middleIt + 1;

            Comment::Build(writer, "Key{" + std::to_string(key) + "} > it[" +
              std::to_string(middleIdx) + "]{" + std::to_string(*middleIt) +
              "}: search within upper sequence.", 1);
          }
          else
          {
            highIt = middleIt;

            Comment::Build(writer, "Key{" + std::to_string(key) + "} < it[" +
              std::to_string(middleIdx) + "]{" + std::to_string(*middleIt) +
              "}: search within lower sequence.", 1);
          }

          /// LOG RANGE CHANGED
          /// @todo check new type of operation
          writer.StartObject();
            writer.Key("type");
            writer.String("operation");
            writer.Key("name");
            writer.String("setRange");
            writer.Key("range");
            writer.StartArray();
              writer.Int(static_cast<int>(std::distance(begin, lowIt)));
              writer.Int(static_cast<int>(std::distance(begin, highIt)));
            writer.EndArray();
          writer.EndObject();

          middleIt = lowIt + std::distance(lowIt, highIt) / 2;
          middleIdx = static_cast<int>(std::distance(begin, middleIt));
          Comment::Build(writer, "Select middle element[" + std::to_string(middleIdx) +
            "]{" + std::to_string(*middleIt) + "}.", 1);
          Operation::Set<int>(writer, "it", middleIdx);
        }

        // Logs failure
        if (index < 0)
          Comment::Build(writer, "Key {" + std::to_string(key) + "} was not found.", 0);

        Operation::Return<int>(writer, index);
        writer.EndArray();
        return true;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SEARCH_BINARY_LOG_HXX
