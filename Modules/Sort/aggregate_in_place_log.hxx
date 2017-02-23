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
      /// eg https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
      static const String GetName() { return "AggregateInPlace"; }

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
      ~AggregateInPlaceLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with AggregateInPlaceLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts,
                            const IT& begin, const IT& pivot, const IT& end,
                            const int offset = 0)
      {
        std::unique_ptr<AggregateInPlaceLog> builder =
          std::unique_ptr<AggregateInPlaceLog>(new AggregateInPlaceLog(os));
        builder->Write(opts, begin, pivot, end, offset);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with AggregateInPlaceLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts,
                           const IT& begin, const IT& pivot, const IT& end,
                           const int offset = 0)
      {
        Write(writer, opts, begin, pivot, end, offset);

        return writer;
      }

    private:
      AggregateInPlaceLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                                         writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      AggregateInPlaceLog operator=(AggregateInPlaceLog&) {} // Not Implemented

      bool Write(Options opts,
                 const IT& begin, const IT& pivot, const IT& end,
                 const int offset)
      { return Write(*this->writer, opts, begin, pivot, end, offset); }

      static bool Write(Writer& writer, Options opts,
                        const IT& begin, const IT& pivot, const IT& end,
                        const int offset)
      {
        // Do not write sequence if no data to be processed
        if (std::distance(begin, pivot) < 1 || std::distance(pivot, end) < 1)
        {
          Comment::Build(writer, "One of the sequence is too small to be processed.", 0);
          Operation::Return<bool>(writer, true);
          return true;
        }

        writer.StartObject();

        // Write description
        Algo_Traits<AggregateInPlaceLog>::Build(writer, opts);

        // Write parameters
        WriteParameters(writer, opts, begin, pivot, end, offset);

        // Write computation
        WriteComputation(writer, begin, pivot, end, offset);

        writer.EndObject();

        return true;
      }

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
      static bool WriteComputation(Writer& writer,
                                   const IT& begin, const IT& pivot, const IT& end,
                                   const int offset)
      {
        auto _beginIdx = offset;

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        auto it = Iterator::BuildIt<IT>(writer, kSeqName, "it", _beginIdx, begin, "Cur IT");
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();
        // Use first half as receiver
        for(auto curBegin = begin; curBegin < pivot; ++curBegin, ++_beginIdx)
        {
          Operation::Set<int>(writer, "begin", _beginIdx);

          if (Compare()(*curBegin, *pivot))
            continue;

          // Place the highest/lowest value it at the beginning of the first list
          std::swap(*curBegin, *pivot);
          Operation::Swap(writer, "begin", "pivot");

          // Displace the higher value in the right place of the second list by swapping
          it = pivot;
          auto _itIdx = offset + static_cast<int>(std::distance(begin, it));
          for (; it != end - 1; ++it, ++_itIdx)
          {
            Operation::Set<int>(writer, "it", _itIdx);

            if (Compare()(*it, *(it + 1)))
              break;

            std::swap(*it, *(it + 1));
            Operation::Swap(writer, "it", "it++");
          }
        }

        Operation::Return<bool>(writer, true);
        writer.EndArray();
        return true;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SORT_AGGREGATE_IN_PLACE_LOG_HXX
