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
  template <typename IteratorT, typename T, typename IsEqualT>
  class BinaryLog
  {
    public:
      static const std::string GetAuthor() { return "Michael Jeulin-L"; }
      static const std::string GetDoc()
      {
        return
        "/// Binary Search\
        /// Iteratively proceed a dichotomic search within a sorted collection on the first occurence\
        /// of the key passed as parameter.\
        ///\
        /// @complexity O(log(N)).\
        ///\
        /// @tparam Iterator type using to go through the collection.\
        /// @param begin,end iterators to the initial and final positions of\
        /// the sequence to be sorted. The range used is [first,last), which contains all the elements\
        /// between first and last, including the element pointed by first but\
        /// not the element pointed by last.\
        /// @param key the key value to be searched.\
        ///\
        /// @return sequence index of the first found key, -1 otherwise.";
      }
      static const std::string GetModule() { return "Search"; }
      static const std::string GetName() { return "Binary"; }
      static const std::string GetVersion() { return "1.0"; }

      // Assert correct JSON construction.
      ~BinaryLog() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with BinaryLog object information,
      ///         error object information in case of failure.
      static std::ostream& Build(std::ostream& os, Options opts,
                                 const IteratorT& begin, const IteratorT& end, const T& key)
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
                                const IteratorT& begin, const IteratorT& end, const T& key)
      {
        Write(writer, opts, begin, end, key);

        return writer;
      }

    private:
      BinaryLog(std::ostream& os) : stream(os), writer(this->stream) {}
      BinaryLog operator=(BinaryLog&) {}                                  // Not Implemented

      bool Write(Options opts, const IteratorT& begin, const IteratorT& end, const T& key)
      { return Write(this->writer, opts, begin, end, key); }

      static bool Write(Writer_Type& writer, Options opts,
                        const IteratorT& begin, const IteratorT& end, const T& key)
      {
        writer.StartObject();

        // Build general information
        writer.Key("info");
        Algo_Traits<BinaryLog>::Build(writer, opts);

        // Add parameters
        writer.Key("parameters");
        writer.StartArray();
        Array<IteratorT>::Build(writer, kSeqName, "begin", begin, "end", end);
        Value<T>::Build(writer, "key", key);
        writer.EndArray();

        // Add return value
        // @todo add GetReturn() method and push it within Algo_Trait build
        writer.Key("return");
        writer.StartObject();
        writer.Key("type");
        writer.String("int");
        writer.EndObject();

        // Build computation
        WriteComputation(writer, begin, end, key);

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteComputation(Writer_Type& writer,
                                   const IteratorT& begin, const IteratorT& end, const T& key)
      {
        int index = -1;
        auto lowIt = begin;
        auto highIt = end;
        const int _kSeqSize = static_cast<int>(std::distance(lowIt, highIt)); // Not part of the logs
        auto middleIt = lowIt + _kSeqSize / 2;

        // Log local variables
        writer.Key("locals");
        writer.StartArray();
        Value<int>::Build(writer, "index", index);
        Iterator::Build(writer, kSeqName, "lowIt", 0);
        Iterator::Build(writer, kSeqName, "highIt", _kSeqSize + 1);
        Iterator::Build(writer, kSeqName, "middleIt", _kSeqSize / 2);
        writer.EndArray();

        // Log algorithm operations
        writer.Key("logs");
        writer.StartArray();
        Comment::Build(writer,
          "Search while there is still objects between the two iterators and no object has been found yet");
        while(lowIt < highIt && index < 0)
        {
          if (IsEqualT()(key, *middleIt))
          {
            Comment::Build(writer, "Object found: set index computed from initial begin iterator.", 1);
            index = static_cast<int>(std::distance(begin, middleIt));
          }
          else if (key > *middleIt)
          {
            Comment::Build(writer,
              "Object with higher value than the middle: search key within upper collection.", 1);
            lowIt = middleIt + 1;
          }
          else
          {
            Comment::Build(writer,
              "Object with lower value than the middle: search key within lower collection.", 1);
            highIt = middleIt;
          }

          middleIt = lowIt + std::distance(lowIt, highIt) / 2;
        }

        // Add return value
        // return index;
        writer.StartObject();
        writer.Key("return");
        writer.Int(index);
        writer.EndObject();

        writer.EndArray();
        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_SEARCH_BINARY_LOG_HXX
