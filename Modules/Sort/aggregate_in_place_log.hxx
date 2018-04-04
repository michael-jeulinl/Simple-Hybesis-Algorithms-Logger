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
#ifndef MODULE_SORT_AGGREGATE_IN_PLACE_LOG_HXX
#define MODULE_SORT_AGGREGATE_IN_PLACE_LOG_HXX

#include <Logger/algorithm.hxx>
#include <Logger/array.hxx>
#include <Logger/comment.hxx>
#include <Logger/operation.hxx>
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>
#include <Logger/vector.hxx>


// STD includes
#include <iterator>

namespace SHA_Logger
{
  /// @class AggregateInPlaceLog
  ///
  template <typename IT, typename Compare = std::less<typename std::iterator_traits<IT>::value_type>>
  class AggregateInPlaceLog
  {
    public:
      static const String GetName() { return "Aggregate In Place"; }

      // Assert correct JSON construction.
      ~AggregateInPlaceLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with AggregateInPlaceLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts,
                            const IT& begin, const IT& pivot, const IT& end,
                            VecStats& stats, const int offset = 0)
      {
        std::unique_ptr<AggregateInPlaceLog> builder =
          std::unique_ptr<AggregateInPlaceLog>(new AggregateInPlaceLog(os));
        builder->Write(opts, begin, pivot, end, stats, offset);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with AggregateInPlaceLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts,
                           const IT& begin, const IT& pivot, const IT& end,
                           VecStats& stats, const int offset = 0)
      {
        Write(writer, opts, begin, pivot, end, stats, offset);

        return writer;
      }

    private:
      AggregateInPlaceLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                                         writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      AggregateInPlaceLog operator=(AggregateInPlaceLog&) {} // Not Implemented

      ///
      /// \brief Write
      /// \param opts
      /// \param begin
      /// \param pivot
      /// \param end
      /// \param offset
      /// \return
      ///
      bool Write(Options opts,
                 const IT& begin, const IT& pivot, const IT& end,
                 VecStats& stats, const int offset)
      { return Write(*this->writer, opts, begin, pivot, end, stats, offset); }

      ///
      /// \brief Write
      /// \param writer
      /// \param opts
      /// \param begin
      /// \param pivot
      /// \param end
      /// \param offset
      /// \return
      ///
      static bool Write(Writer& writer, Options opts,
                        const IT& begin, const IT& pivot, const IT& end,
                        VecStats& stats, const int offset)
      {
        // Do not write sequence if no data to be processed
        if (std::distance(begin, pivot) < 1 || std::distance(pivot, end) < 1)
        {
          Operation::Return<bool>(writer, true);
          return true;
        }

        writer.StartObject();

        Algo_Traits<AggregateInPlaceLog>::Build(writer, opts);            // Write description
        WriteParameters(writer, opts, begin, pivot, end, offset);         // Write parameters
        WriteComputation(writer, opts, begin, pivot, end, stats, offset); // Write computation

        writer.EndObject();

        return true;
      }

      ///
      /// \brief WriteParameters
      /// \param writer
      /// \param opts
      /// \param begin
      /// \param pivot
      /// \param end
      /// \param offset
      /// \return
      ///
      static bool WriteParameters(Writer& writer, Options opts,
                                  const IT& begin, const IT& pivot, const IT& end,
                                  const int offset)
      {
        const int _pivIdx = static_cast<int>(std::distance(begin, pivot));
        writer.Key("parameters");
        writer.StartArray();
        if (opts & OpIsSub)
        {
          const int _seqSize = static_cast<int>(std::distance(begin, end));
          Iterator::Build(writer, kSeqName, "begin", offset);
          Iterator::Build(writer, kSeqName, "pivot", offset + _pivIdx);
          Iterator::Build(writer, kSeqName, "end", offset + _seqSize);
        }
        else
        {
          Array<IT>::Build(writer, kSeqName, "begin", begin, "end", end);
          Iterator::Build(writer, kSeqName, "pivot", _pivIdx);
        }
        writer.EndArray();;

        return true;
      }

      ///
      /// \brief WriteComputation
      /// \param writer
      /// \param begin
      /// \param pivot
      /// \param end
      /// \param offset
      /// \return
      ///
      static bool WriteComputation(Writer& writer, Options opts,
                                   const IT& begin, const IT& pivot, const IT& end,
                                   VecStats& stats, const int offset)
      {
        auto _beginIdx = offset;
        const auto _pIdx = offset + static_cast<const int>(std::distance(begin, pivot));

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        auto it = Iterator::BuildIt<IT>(writer, kSeqName, "it", _beginIdx, begin);
        ++stats.nbOtherAccess;
        Iterator::BuildIt<IT>(writer, kSeqName, "it_increment", _beginIdx + 1, begin + 1);
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();

        Comment::Build(writer, static_cast<std::string>(
                       "Swap if smaller, second part first element with first part current element") +
                       " and place the new first at its right position:");
        for (auto curBegin = begin; curBegin < pivot; ++curBegin, ++_beginIdx, ++stats.nbIterations)
        {
          Operation::Set<int>(writer, "begin", _beginIdx);
          ++stats.nbComparisons;
          if (Compare()(*curBegin, *pivot))
          {
            Comment::Build(writer, "it[" + ToString(_beginIdx) + "]{" + ToString(*curBegin) +
                           "} < pivot[" + ToString(_pIdx) + "]{" + ToString(*pivot) +
                           "} : Ignore element.", 1);
            continue;
          }

          Comment::Build(writer, "it[" + ToString(_beginIdx) + "]{" + ToString(*curBegin) +
                         "} >= pivot[" + ToString(_pIdx) + "]{" + ToString(*pivot) +
                         "} : Swap pivot with the current it.", 1);
          ++stats.nbSwaps;
          Operation::Swap(writer, "begin", "pivot");
          std::swap(*curBegin, *pivot);

          it = pivot;
          auto _itIdx = offset + static_cast<int>(std::distance(begin, it));
          Comment::Build(writer, "Displace new pivot value in the right place by bubbling:", 1);
          for (; it != end - 1; ++it, ++_itIdx, ++stats.nbIterations)
          {
            Operation::Set<int>(writer, "it", _itIdx);
            Operation::Set<int>(writer, "it_increment", _itIdx + 1);

            ++stats.nbComparisons;
            ++stats.nbOtherAccess;
            if (!Compare()(*(it + 1), *it))
            {
              Comment::Build(writer, "it[" + ToString(_itIdx) + "]{" + ToString(*it) +
                             "} <= next[" + ToString(_itIdx + 1) + "]{" + ToString(*(it + 1)) +
                             "} : Element at its right place, break.", 2);
              break;
            }

            Comment::Build(writer, "it[" + ToString(_itIdx) + "]{" + ToString(*it) +
                           "} > next[" + ToString(_itIdx + 1) + "]{" + ToString(*(it + 1)) +
                           "} : Bubble element.", 2);

            ++stats.nbSwaps;
            Operation::Swap(writer, "it", "it_increment");
            std::swap(*it, *(it + 1));
          }
        }

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

#endif // MODULE_SORT_AGGREGATE_IN_PLACE_LOG_HXX
