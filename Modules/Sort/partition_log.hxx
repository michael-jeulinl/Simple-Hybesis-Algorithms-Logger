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
#ifndef MODULE_SORT_PARTITION_LOG_HXX
#define MODULE_SORT_PARTITION_LOG_HXX

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
  /// @class PartitionLog
  ///
  template <typename IT, typename Compare = std::less_equal<typename std::iterator_traits<IT>::value_type>>
  class PartitionLog
  {
    typedef Value<typename std::iterator_traits<IT>::value_type> Value_Type;

    public:
      static const String GetName() { return "Partition"; }

      // Assert correct JSON construction.
      ~PartitionLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with PartitionLog object information,
      ///         error object information in case of failure.
      static IT Build(Ostream& os, Options opts,
                      const IT& begin, const IT& pivot, const IT& end,
                      VecStats& stats, const int offSet = 0)
      {
        std::unique_ptr<PartitionLog> builder = std::unique_ptr<PartitionLog>(new PartitionLog(os));
        return builder->Write(opts, begin, pivot, end, stats, offSet);
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with PartitionLog object information,
      ///         error information in case of failure.
      static IT Build(Writer& writer, Options opts,
                      const IT& begin, const IT& pivot, const IT& end,
                      VecStats& stats, const int offSet = 0)
      {
        return Write(writer, opts, begin, pivot, end, stats, offSet);
      }

    private:
      PartitionLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                                  writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      PartitionLog operator=(PartitionLog&) {} // Not Implemented

      IT Write(Options opts, const IT& begin, const IT& pivot, const IT& end,
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
      static IT Write(Writer& writer, Options opts,
                      const IT& begin, const IT& pivot, const IT& end,
                      VecStats& stats, const int offset)
      {
        writer.StartObject();

        // Do not write sequence if no data to be processed
        if (static_cast<int>(std::distance(begin, end)) < 2 || pivot == end)
        {
          Operation::Return<bool>(writer, true);
          return pivot;
        }

        Algo_Traits<PartitionLog>::Build(writer, opts);                                   // Write description
        WriteParameters(writer, opts, begin, pivot, end, offset);                         // Write parameters
        auto newPivot = WriteComputation(writer, opts, begin, pivot, end, stats, offset); // Write computation

        writer.EndObject();

        return newPivot;
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
        const int _pivIdx = static_cast<int>(std::distance(begin,pivot));
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

        writer.EndArray();

        return true;
      }

      ///
      static IT WriteComputation(Writer& writer, Options opts,
                                 const IT& begin, const IT& pivot, const IT& end,
                                 VecStats& stats, const int offset)
      {
        auto _seqSize = static_cast<int>(std::distance(begin, end));
        const int _pivotIdx = static_cast<int>(std::distance(begin, pivot)) + offset;
        int _itIdx = offset;
        int _storeIdx = offset;

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();

        auto it = Iterator::BuildIt<IT>(writer, kSeqName, "it", offset, begin);
        ++stats.nbOtherAccess;
        Iterator::BuildIt<IT>(writer, kSeqName, "last", offset + _seqSize - 1, end - 1);
        auto pivotValue = Value_Type::BuildValue(writer, "pivotValue", *pivot);
        auto store = Iterator::BuildIt<IT>(writer, kSeqName, "store", offset, begin,
          "Set a store iterator delimiting both partition at the beginning of the current sequence: [" +
          ToString(offset) + "]{" + ToString(*begin) + "}");

        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();

        if (_pivotIdx - offset != _seqSize - 1)
        {
          Comment::Build(writer,
                         "Put the pivot[" + ToString(_pivotIdx) + "]{" +
                         ToString(pivotValue) + "} at the end for convenience.");

          ++stats.nbSwaps;
          Operation::Swap(writer, "pivot", "last");
          std::swap(*pivot, *(end - 1));
        }

        Comment::Build(writer, "Put each value <= pivot on the left side of the store pointer:");
        for (it = begin; it != end - 1; ++it, Operation::Set<int>(writer, "it", ++_itIdx), ++stats.nbIterations)
        {
          ++stats.nbComparisons;
          if (Compare()(*it, pivotValue))
          {
            // Swap if needed
            if (_itIdx != _storeIdx)
            {
              Comment::Build(writer, "it[" + ToString(_itIdx) + "]{" + ToString(*it) +
                                     "} <= pivot{" + ToString(pivotValue) + "}" +
                                     ": swap it with the store and increment store pointer[" +
                                     ToString(_storeIdx + 1) + "]", 1);

              Operation::Swap(writer, "store", "it");
              std::swap(*store, *it);
              ++stats.nbSwaps;
            }

            Operation::Set<int>(writer, "store", ++_storeIdx);
            ++stats.nbOtherAccess;
            ++store;
          }
          else
          {
            Comment::Build(writer, "it[" + ToString(_itIdx) + "]{" + ToString(*it) +
                           "} > pivot{" + ToString(pivotValue) + "} : Ignore element.", 1);
          }
        }

        if (_storeIdx != (offset + _seqSize - 1) && Compare()(pivotValue, *store))
        {
          Comment::Build(writer, "Restore pivot[" + ToString(offset + _seqSize - 1) +
                         "]{" + ToString(*(end - 1)) + "} at the current store[" +
                         ToString(_storeIdx) + "]{" + ToString(*store) + "}.", 0);
          Operation::Set<int>(writer, "pivot", _storeIdx);

          ++stats.nbSwaps;
          ++stats.nbOtherAccess;
          Operation::Swap(writer, "store", "last");
          std::swap(*(end - 1), *store);
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

        return store;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SORT_PARTITION_HXX
