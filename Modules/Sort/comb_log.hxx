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
#ifndef MODULE_SORT_COMB_LOG_HXX
#define MODULE_SORT_COMB_LOG_HXX

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
  /// @class CombLog
  ///
  template <typename IT, typename Compare = std::less<typename std::iterator_traits<IT>::value_type>>
  class CombLog
  {
    public:
      static const String GetName() { return "Comb Sort"; }

      // Assert correct JSON construction.
      ~CombLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with CombLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const IT& begin, const IT& end, VecStats& stats)
      {
        std::unique_ptr<CombLog> builder = std::unique_ptr<CombLog>(new CombLog(os));
        builder->Write(opts, begin, end, stats);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with CombLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const IT& begin, const IT& end, VecStats& stats)
      {
        Write(writer, opts, begin, end, stats);

        return writer;
      }

    private:
      CombLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                             writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      CombLog operator=(CombLog&) {} // Not Implemented

      bool Write(Options opts, const IT& begin, const IT& end, VecStats& stats)
      { return Write(*this->writer, opts, begin, end, stats); }

      static bool Write(Writer& writer, Options opts, const IT& begin, const IT& end, VecStats& stats)
      {
        // Do not write sequence if no data to be processed
        const int _seqSize = static_cast<int>(std::distance(begin, end));
        if (_seqSize < 2)
        {
          Comment::Build(writer, "Sequence size too small to be processed.", 0);
          Operation::Return<bool>(writer, true);
          return true;
        }

        writer.StartObject();

        Algo_Traits<CombLog>::Build(writer, opts);    // Write description
        WriteParameters(writer, opts, begin, end);    // Write parameters
        WriteComputation(writer, begin, end, stats);  // Write computation

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer& writer, Options opts, const IT& begin, const IT& end)
      {
        writer.Key("parameters");
        writer.StartArray();
        if (opts & OpIsSub)
        {
          const int _seqSize = static_cast<int>(std::distance(begin, end));
          Iterator::Build(writer, kSeqName, "begin", 0);
          Iterator::Build(writer, kSeqName, "end", _seqSize);
        }
        else
        {
          Array<IT>::Build(writer, kSeqName, "begin", begin, "end", end);
        }
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer& writer, const IT& begin, const IT& end, VecStats& stats)
      {
        const int ksize = static_cast<int>(std::distance(begin, end));

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        auto it = Iterator::BuildIt<IT>(writer, kSeqName, "it", 0, begin);
        Iterator::BuildIt<IT>(writer, kSeqName, "pivot", ksize - 1, end - 1);
        writer.EndArray();

        // Proceed sort
        writer.Key("logs");
        writer.StartArray();
        int gap = ksize;
        bool hasSwapped = true;
        Comment::Build(writer, "Swap smallest element between decreasing gap until sorted.");
        while (hasSwapped)
        {
          hasSwapped = false;

          // Compute new gap
          gap /= 1.25;
          if (gap > 1)
            hasSwapped = true;
          else
            gap = 1;
          Comment::Build(writer, "gap = " + ToString(gap) + ".", 1);

          int _itIdx = 0;
          Operation::Set<int>(writer, "it", _itIdx);
          Operation::Set<int>(writer, "pivot", _itIdx + gap);
          for (it = begin; it + gap < end; ++it, ++stats.nbIterations)
          {
            ++stats.nbComparisons;
            ++stats.nbOtherAccess;
            if (Compare()(*(it + gap), *it))
            {
              Comment::Build(writer, "it[" + ToString(_itIdx) + "]{" + ToString(*it) +
                             "} > next[" + ToString(_itIdx + gap) + "]{" + ToString(*(it + gap)) +
                             "} : Swap them.", 2);
              ++stats.nbSwaps;
              Operation::Swap(writer, "it", "pivot");
              std::swap(*it, *(it + gap));
              hasSwapped = true;
            }

            Comment::Build(writer, "it[" + ToString(_itIdx) + "]{" + ToString(*it) +
                           "} <= next[" + ToString(_itIdx + gap) + "]{" + ToString(*(it + gap)) +
                           "} : Ignore element.", 2);
            Operation::Set<int>(writer, "it", ++_itIdx);
            Operation::Set<int>(writer, "pivot", _itIdx + gap);
          }
        }

        Operation::Return<bool>(writer, true);
        writer.EndArray();

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

        return true;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SORT_COMB_LOG_HXXb
