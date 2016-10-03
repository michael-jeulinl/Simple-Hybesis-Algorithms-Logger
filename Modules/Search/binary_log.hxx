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
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>

namespace SHA_Search
{
  using namespace SHA_Logger;

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
        /// @return The vector index of the first found key, -1 otherwise.";
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
        Array<IteratorT>::Build(writer, "p_0", "begin", begin, "end", end);
        Value<T>::Build(writer, "key", key);
        writer.EndArray();

        // Build computation


        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_SEARCH_BINARY_LOG_HXX
