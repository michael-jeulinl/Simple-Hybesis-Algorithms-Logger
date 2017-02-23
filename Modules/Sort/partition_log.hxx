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
  template <typename IT, typename Compare = std::less_equal<typename std::iterator_traits<IT>::value_type>>
  class PartitionLog
  {
    public:
    /// @todo Use string litteral for JSON description within c++ code
    /// eg https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
    static const String GetName() { return "Partition"; }

      /// Write algorithm information
      /// @todo Use string litteral for JSON description within c++ code --> partition_desc.json.hxx
      static bool WriteInfo(Writer& writer) { return true; }

      /// Write algorithm decription
      /// @todo Use string litteral for JSON description within c++ code --> partition_desc.json.hxx
      static bool WriteDoc(Writer& writer) { return true; }

      /// Write algorithm sources
      /// @todo Use string litteral for JSON description within c++ code --> partition_desc.json.hxx
      static bool WriteSrc(Writer& writer) { return true; }

      // Assert correct JSON construction.
      ~PartitionLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with PartitionLog object information,
      ///         error object information in case of failure.
      static IT Build(Ostream& os, Options opts,
                      const IT& begin, const IT& pivot, const IT& end, const int offSet = 0)
      {
        std::unique_ptr<PartitionLog> builder = std::unique_ptr<PartitionLog>(new PartitionLog(os));
        return builder->Write(opts, begin, pivot, end, offSet);
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with PartitionLog object information,
      ///         error information in case of failure.
      static IT Build(Writer& writer, Options opts,
                      const IT& begin, const IT& pivot, const IT& end, const int offSet = 0)
      {
        return Write(writer, opts, begin, pivot, end, offSet);
      }

    private:
      PartitionLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                                  writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      PartitionLog operator=(PartitionLog&) {} // Not Implemented

      IT Write(Options opts, const IT& begin, const IT& pivot, const IT& end, const int offset)
      { return Write(*this->writer, opts, begin, pivot, end, offset); }

      static IT Write(Writer& writer, Options opts,
                      const IT& begin, const IT& pivot, const IT& end,
                      const int offset)
      {
        writer.StartObject();

        // Do not write sequence if no data to be processed
        const int _seqSize = static_cast<int>(std::distance(begin, end));
        if (_seqSize < 2 || pivot == end)
        {
          Comment::Build(writer, "Sequence size too small to be processed.", 0);
          // @todo return pivot
          Operation::Return<bool>(writer, true);
          return pivot;
        }

        // Write description
        Algo_Traits<PartitionLog>::Build(writer, opts);

        // Write parameters
        WriteParameters(writer, opts, begin, pivot, end, offset);

        // Write computation
        auto newPivot = WriteComputation(writer, begin, pivot, end, offset);

        writer.EndObject();

        return newPivot;
      }

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
      static IT WriteComputation(Writer& writer,
                                 const IT& begin, const IT& pivot, const IT& end,
                                 const int offset)
      {
        // Not part of the logs
        auto _seqSize = static_cast<int>(std::distance(begin, end));
        int _itIdx = offset;
        int _storeIdx = offset;

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        auto it = Iterator::BuildIt<IT>(writer, kSeqName, "it", offset, begin, "Cur IT");
        Iterator::BuildIt<IT>(writer, kSeqName, "last", offset + _seqSize - 1, end - 1, "Lst IT");
        auto pivotValue = Value<typename std::iterator_traits<IT>::value_type>::
                          BuildValue(writer, "pivotValue", *pivot, "Keet pivot val.");
        auto store = Iterator::BuildIt<IT>(writer, kSeqName, "store", offset, begin, "Store IT");
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();
        Comment::Build(writer, "Start Partitinning", 0);

        Comment::Build(writer, "Put the pivot at the end for convenience", 0);
        Operation::Swap(writer, "pivot", "last");
        std::swap(*pivot, *(end - 1));

        Comment::Build(writer, "Swap each element with ", 0);
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

        Comment::Build(writer, "Replace the pivot at its good position and replace its value.", 0);
        Operation::Set<int>(writer, "pivot", _storeIdx);
        Operation::Swap(writer, "store", "last");
        std::swap(*(end - 1), *store);

        // @todo Return Iterator
        Operation::Return<bool>(writer, true);
        writer.EndArray();
        return store;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SORT_PARTITION_HXX
