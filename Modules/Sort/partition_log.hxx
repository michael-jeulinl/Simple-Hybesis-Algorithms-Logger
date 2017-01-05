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

// STD includes
#include <iterator>

namespace SHA_Logger
{
  /// @class PartitionLog
  ///
  template <typename IT, typename Compare>
  class PartitionLog
  {
    public:
      /// Write algorithm information
      /// @TODO Use string litteral for JSON description within c++ code
      /// @e.g https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
      static std::string GetName() { return "Patition"; }

      /// Write algorithm information
      /// @TODO Use string litteral for JSON description within c++ code --> partition_desc.json.hxx
      static bool WriteInfo(Writer_Type& writer) { return true; }

      /// Write algorithm decription
      /// @TODO Use string litteral for JSON description within c++ code --> partition_desc.json.hxx
      static bool WriteDoc(Writer_Type& writer) { return true; }

      /// Write algorithm sources
      /// @TODO Use string litteral for JSON description within c++ code --> partition_desc.json.hxx
      static bool WriteSrc(Writer_Type& writer) { return true; }

      // Assert correct JSON construction.
      ~PartitionLog() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with PartitionLog object information,
      ///         error object information in case of failure.
      static Ostream_T& Build(Ostream_T& os, Options opts, const IT& begin, IT& pivot, const IT& end,
                              const int beginIdx = 0)
      {
        auto parameter = PartitionLog(os);
        parameter.Write(opts, begin, pivot, end, beginIdx);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with PartitionLog object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer, Options opts, const IT& begin, IT& pivot, const IT& end,
                                const int beginIdx = 0)
      {
        Write(writer, opts, begin, pivot, end, beginIdx);

        return writer;
      }

    private:
      PartitionLog(std::ostream& os) : stream(os), writer(this->stream) {}
      PartitionLog operator=(PartitionLog&) {}                                  // Not Implemented

      bool Write(Options opts, const IT& begin, IT& pivot, const IT& end, const int beginIdx)
      { return Write(this->writer, opts, begin, pivot, end, beginIdx); }

      static bool Write(Writer_Type& writer, Options opts, const IT& begin, IT& pivot, const IT& end,
                        const int beginIdx)
      {
        writer.StartObject();

        // Do not write sequence if no data to be processed
        const int _seqSize = static_cast<int>(std::distance(begin, end));
        if (_seqSize < 2 || pivot == end)
        {
          Comment::Build(writer, "Sequence size too small to be processed.", 0);
          Operation::Return<bool>(writer, true);
          return true;
        }

        // Write description
        Algo_Traits<PartitionLog>::Build(writer, opts);

        // Write parameters
        WriteParameters(writer, opts, begin, pivot, end, beginIdx);

        // Write computation
        WriteComputation(writer, begin, pivot, end, beginIdx);

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer_Type& writer, Options opts,
                                  const IT& begin, IT& pivot, const IT& end, const int beginIdx)
      {
        // Do not write sequence if no data to be processed
        const int _pivIdx = static_cast<int>(std::distance(begin,pivot));
        writer.Key("parameters");
        writer.StartArray();
        if (opts & OpIsSub)
        {
          const int _seqSize = static_cast<int>(std::distance(begin, end));
          Iterator::Build(writer, kSeqName, "begin", beginIdx);
          Iterator::Build(writer, kSeqName, "pivot", beginIdx + _pivIdx);
          Iterator::Build(writer, kSeqName, "end", beginIdx + _seqSize);
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
      static bool WriteComputation(Writer_Type& writer, const IT& begin, IT& pivot, const IT& end,
                                   const int beginIdx)
      {
        // Not part of the logs
        auto _seqSize = static_cast<int>(std::distance(begin, end));
        int _itIdx = beginIdx;
        int _storeIdx = beginIdx;

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        auto it = Iterator::BuildIt<IT>(writer, kSeqName, "it", beginIdx, begin, "Current element iterator.");
        auto last = Iterator::BuildIt<IT>(writer, kSeqName, "last", beginIdx + _seqSize - 1, end - 1, "Last element.");
        auto pivotValue = Value<IT::value_type>::BuildValue(writer, "pivotValue", *pivot, "Keet pivot val.");
        auto store = Iterator::BuildIt<IT>(writer, kSeqName, "store", beginIdx, begin, "Current store iterator.");
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();
        Comment::Build(writer, "Start Partitinning");

        Comment::Build(writer, "Put the pivot at the end for convenience");
        Operation::Swap(writer, "pivot", "last");
        std::swap(*pivot, *(end - 1));

        Comment::Build(writer, "Swap each element with ");
        for (it = begin; it != end - 1; ++it, Operation::Set<int>(writer, "it", ++_itIdx))
        {
          if (Compare()(*it, pivotValue))
          {
            Comment::Build(writer, "it <= pivot: swap(it, store).", 1);
            Operation::Swap(writer, "store", "it");
            Operation::Set<int>(writer, "store", ++_storeIdx);
            std::swap(*store, *it);
            ++store;
          } else
            Comment::Build(writer, "pivot > it : Ignore element.", 1);
        }

        Comment::Build(writer, "Replace the pivot at its good position and replace its value.");
        Operation::Set<int>(writer, "pivot", _storeIdx);
        pivot = store;
        Operation::Swap(writer, "pivot", "last");
        std::swap(*(end - 1), *pivot);

        Operation::Return<bool>(writer, true);
        writer.EndArray();
        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_SORT_PARTITION_HXX
