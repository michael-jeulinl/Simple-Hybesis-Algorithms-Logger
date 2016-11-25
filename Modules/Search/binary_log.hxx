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

namespace SHA_Search
{
  namespace
  {
    using namespace SHA_Logger;

    static const std::string kSeqName = "sequence"; // Name used as id for Array build from iterators.
  }

  /// @class BinaryLog
  ///
  template <typename IT, typename T, typename IsEqualT>
  class BinaryLog
  {
    public:
      /// Write algorithm information
      /// @TODO Use string litteral for JSON description within c++ code --> binary_desc.json.hxx
      static bool WriteInfo(Writer_Type& writer) { return true; }

      /// Write algorithm decription
      /// @TODO Use string litteral for JSON description within c++ code --> binary_desc.json.hxx
      static bool WriteDoc(Writer_Type& writer) { return true; }

      /// Write algorithm sources
      /// @TODO Use string litteral for JSON description within c++ code --> binary_desc.json.hxx
      static bool WriteSrc(Writer_Type& writer) { return true; }

      // Assert correct JSON construction.
      ~BinaryLog() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with BinaryLog object information,
      ///         error object information in case of failure.
      static Ostream_T& Build(Ostream_T& os, Options opts, const IT& begin, const IT& end, const T& key)
      {
        auto parameter = BinaryLog(os);
        parameter.Write(opts, begin, end, key);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with BinaryLog object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer, Options opts,
                                const IT& begin, const IT& end, const T& key)
      {
        Write(writer, opts, begin, end, key);

        return writer;
      }

    private:
      BinaryLog(std::ostream& os) : stream(os), writer(this->stream) {}
      BinaryLog operator=(BinaryLog&) {}                                  // Not Implemented

      bool Write(Options opts, const IT& begin, const IT& end, const T& key)
      { return Write(this->writer, opts, begin, end, key); }

      static bool Write(Writer_Type& writer, Options opts, const IT& begin, const IT& end, const T& key)
      {
        writer.StartObject();

        // Write description
        Algo_Traits<BinaryLog>::Build(writer, opts);

        // Write parameters
        WriteParameters(writer, begin, end, key);

        // Write computation
        WriteComputation(writer, begin, end, key);

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer_Type& writer, const IT& begin, const IT& end, const T& key)
      {
        writer.Key("parameters");
        writer.StartArray();
        Array<IT>::Build(writer, kSeqName, "begin", begin, "end", end);
        Value<T>::Build(writer, "key", key);
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer_Type& writer, const IT& begin, const IT& end, const T& key)
      {
        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        Value<int>::Build(writer, "index", -1);
        Value<int>::Build(writer, "seqSize", -1);
        Iterator::Build(writer, kSeqName, "lowIt", -1);
        Iterator::Build(writer, kSeqName, "highIt", -1);
        Iterator::Build(writer, kSeqName, "middleIt", -1);
        writer.EndArray();

        // Setting local variable
        int index = -1;
        auto lowIt = begin;
        auto highIt = end;
        int seqSize = static_cast<int>(std::distance(lowIt, highIt));
        auto middleIt = lowIt + seqSize / 2;

        // Not part of the logs
        int _lowIdx = 0;

        // Log algorithm operations
        writer.Key("logs");
        writer.StartArray();

        // Log local variable settings
        Operation::Set<int>(writer, "index", index);
        Operation::Set<int>(writer, "seqSize", seqSize);
        Operation::Set<int>(writer, "lowIt", 0);
        Operation::Set<int>(writer, "highIt", seqSize + 1);
        Operation::Set<int>(writer, "middleIt", seqSize / 2);

        Comment::Build(writer, "Start Binary Search", 0);
        while (lowIt < highIt && index < 0)
        {
          if (IsEqualT()(key, *middleIt))
          {
            index = static_cast<int>(std::distance(begin, middleIt));

            Comment::Build(writer, "Found!", 1);
            Operation::Set<int>(writer, "index", index);
            break;
          }
          else if (key > *middleIt)
          {
            lowIt = middleIt + 1;
            _lowIdx += seqSize / 2 + 1;
            seqSize -= seqSize / 2 + 1;

            Comment::Build(writer, "(key > current): search upper sequence.", 1);
            Operation::Set<int>(writer, "lowIt", _lowIdx);
          }
          else
          {
            highIt = middleIt;

            Comment::Build(writer, "(key < current): search lower sequence.", 1);
            Operation::Set<int>(writer, "highIt", seqSize / 2);
            seqSize -= seqSize / 2;
          }

          middleIt = lowIt + seqSize / 2;
          Operation::Set<int>(writer, "middleIt", _lowIdx + seqSize / 2);
        }

        // Logs failure
        if (index < 0)
          Comment::Build(writer, "Key not found.", 0);

        // Log return value
        Operation::Return<int>(writer, index);

        writer.EndArray();
        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_SEARCH_BINARY_LOG_HXX
