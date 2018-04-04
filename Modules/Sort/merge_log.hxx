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
    typedef MergeLog<IT, Compare, Aggregator> Merge;

    public:
      static const String GetName() { return "Merge Sort"; }

      // Assert correct JSON construction.
      ~MergeLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with MergeLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const IT& begin, const IT& end,
                            VecStats& stats, const int offset = 0)
      {
        std::unique_ptr<MergeLog> builder = std::unique_ptr<MergeLog>(new MergeLog(os));
        builder->Write(opts, begin, end, stats, offset);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with MergeLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const IT& begin, const IT& end,
                           VecStats& stats, const int offset = 0)
      {
        Write(writer, opts, begin, end, stats, offset);

        return writer;
      }

    private:
      MergeLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                              writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      MergeLog operator=(MergeLog&) {} // Not Implemented

      ///
      /// \brief Write
      /// \param opts
      /// \param begin
      /// \param end
      /// \param offset
      /// \return
      ///
      bool Write(Options opts, const IT& begin, const IT& end, VecStats& stats,  const int offset)
      { return Write(*this->writer, opts, begin, end, stats, offset); }

      ///
      /// \brief Write
      /// \param writer
      /// \param opts
      /// \param begin
      /// \param end
      /// \param offset
      /// \return
      ///
      static bool Write(Writer& writer, Options opts, const IT& begin, const IT& end,
                        VecStats& stats, const int offset)
      {
        // Do not write sequence if no data to be processed
        if (static_cast<int>(std::distance(begin, end)) < 2)
        {
          Operation::Return<bool>(writer, true);
          return true;
        }

        writer.StartObject();

        Algo_Traits<MergeLog>::Build(writer, opts);                 // Write description
        WriteParameters(writer, opts, begin, end, offset);          // Write parameters
        WriteComputation(writer, opts, begin, end, stats, offset);  // Write computation

        writer.EndObject();

        return true;
      }

      ///
      /// \brief WriteParameters
      /// \param writer
      /// \param opts
      /// \param begin
      /// \param end
      /// \param offset
      /// \return
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
      /// \brief WriteComputation
      /// \param writer
      /// \param begin
      /// \param end
      /// \param offset
      /// \return
      ///
      static bool WriteComputation(Writer& writer, Options opts,
                                   const IT& begin, const IT& end,
                                   VecStats& stats, const int offset)
      {
        const auto _seqSize = static_cast<const int>(std::distance(begin, end));
        const auto _pIdx = _seqSize / 2;
        const std::string comment = "Pick Pivot at the middle of  [" + ToString(offset) + ", " +
                                    ToString(offset + _seqSize - 1) + "] --> [" +
                                    ToString(offset + _pIdx) + "]{" +
                                    ToString(*(begin + _pIdx)) + "}";

        writer.Key("locals");
        writer.StartArray();
        ++stats.nbOtherAccess;
        auto pivot = Iterator::BuildIt<IT>(writer, kSeqName, "pivot", offset + _pIdx, begin + _pIdx, comment);
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();

        Merge::Build(writer, OpIsSub, begin, pivot, stats, offset);           // Recurse on first partition
        Merge::Build(writer, OpIsSub, pivot, end, stats, offset + _pIdx);     // Recurse on second partition
        Aggregator::Build(writer, OpIsSub, begin, pivot, end, stats, offset); // Merge the two pieces

        Operation::Return<bool>(writer, true);
        writer.EndArray();

        if (!(opts & OpIsSub))
        {
          // Add Statistical informations
          writer.Key("stats");
          writer.StartObject();
            writer.Key("nbComparisons");
            writer.Int(stats.nbComparisons);
            writer.Key("nbIterations");
            writer.Int(stats.nbIterations);
            writer.Key("nbOtherAccess");
            writer.Int(stats.nbOtherAccess);
            writer.Key("nbSwaps");
            writer.Int(stats.nbSwaps);
          writer.EndObject();
        }

        return true;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SORT_MERGE_HXX
