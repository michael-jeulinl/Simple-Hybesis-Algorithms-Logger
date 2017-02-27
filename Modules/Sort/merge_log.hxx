/*===========================================================================================================
 *
 * SHA - Simple Hybesis Algorithms
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#ifndef MODULE_SORT_MERGE_LOG_HXX
#define MODULE_SORT_MERGE_LOG_HXX

#include <Logger/algorithm.hxx>
#include <Logger/array.hxx>
#include <Logger/comment.hxx>
#include <Logger/operation.hxx>
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>
#include <aggregate_in_place_log.hxx>

// STD includes
#include <iterator>

namespace SHA_Logger
{
  /// @class MergeLog
  ///
  template <typename IT,
            typename Compare = std::less<typename std::iterator_traits<IT>::value_type>,
            typename Aggregator = AggregateInPlaceLog<IT, Compare>
            >
  class MergeLog
  {
    public:
      /// eg https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
      static const String GetName() { return "Merge_Sort"; }

      /// Write algorithm information
      /// @todo Use string litteral for JSON description within c++ code
      static bool WriteInfo(Writer& writer) { return true; }

      /// Write algorithm decription
      /// @todo Use string litteral for JSON description within c++ code
      static bool WriteDoc(Writer& writer) { return true; }

      /// Write algorithm sources
      /// @todo Use string litteral for JSON description within c++ code
      static bool WriteSrc(Writer& writer) { return true; }

      // Assert correct JSON construction.
      ~MergeLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with MergeLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const IT& begin, const IT& end, const int offset = 0)
      {
        std::unique_ptr<MergeLog> builder = std::unique_ptr<MergeLog>(new MergeLog(os));
        builder->Write(opts, begin, end, offset);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with MergeLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const IT& begin, const IT& end, const int offset = 0)
      {
        Write(writer, opts, begin, end, offset);

        return writer;
      }

    private:
      MergeLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                              writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      MergeLog operator=(MergeLog&) {} // Not Implemented

      bool Write(Options opts, const IT& begin, const IT& end, const int offset)
      { return Write(*this->writer, opts, begin, end, offset); }

      static bool Write(Writer& writer, Options opts, const IT& begin, const IT& end, const int offset)
      {
        // Do not write sequence if no data to be processed
        const auto _seqSize = static_cast<int>(std::distance(begin, end));
        if (_seqSize < 2)
        {
          Comment::Build(writer, "Sequence size too small to be processed.", 0);
          Operation::Return<bool>(writer, true);
          return true;
        }

        writer.StartObject();

        // Write description
        Algo_Traits<MergeLog>::Build(writer, opts);

        // Write parameters
        WriteParameters(writer, opts, begin, end, offset);

        // Write computation
        WriteComputation(writer, begin, end, offset);

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer& writer, Options opts,
                                  const IT& begin, const IT& end, const int offset)
      {
        writer.Key("parameters");
        writer.StartArray();
        if (opts & OpIsSub)
        {
          const int _seqSize = static_cast<int>(std::distance(begin, end));
          Iterator::Build(writer, kSeqName, "begin", offset);
          Iterator::Build(writer, kSeqName, "end", offset + _seqSize);
        }
        else
        {
          Array<IT>::Build(writer, kSeqName, "begin", begin, "end", end);
        }
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer& writer, const IT& begin, const IT& end, const int offset)
      {
        const auto _pivotIdx = static_cast<const int>(std::distance(begin, end)) / 2;

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        auto pivot = Iterator::BuildIt<IT>(writer, kSeqName, "pivot", offset + _pivotIdx, begin + _pivotIdx);
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();
        // Recurse on first partition
        MergeLog<IT, Compare, Aggregator>::Build(writer, OpIsSub, begin, pivot, offset);
        // Recurse on second partition
        MergeLog<IT, Compare, Aggregator>::Build(writer, OpIsSub, pivot, end, offset + _pivotIdx);

        // Merge the two pieces
        Aggregator::Build(writer, OpIsSub, begin, pivot, end, offset);

        writer.EndArray();

        return true;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SORT_MERGE_HXX
