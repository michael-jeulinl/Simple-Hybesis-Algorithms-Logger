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
#ifndef MODULE_SORT_QUICK_LOG_HXX
#define MODULE_SORT_QUICK_LOG_HXX

#include <Logger/algorithm.hxx>
#include <Logger/array.hxx>
#include <Logger/comment.hxx>
#include <Logger/operation.hxx>
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>
#include <partition_log.hxx>

// STD includes
#include <iterator>
#include <random>

namespace SHA_Logger
{
  template <typename IT>
  class PickFirst
  {
    public:
      std::pair<uint16_t, std::string> operator()
      (const IT& begin, const IT& end, const int offset)
      {
        // Get first element
        const auto lenght = static_cast<const int>(std::distance(begin, end));
        const uint16_t pivotPick = 0;

        // Comment
        const std::string comment = "Pick first item as Pivot within [" + ToString(offset) + ", " +
                                    ToString(offset + lenght - 1) + "] --> [" +
                                    ToString(offset + pivotPick) + "]{" +
                                    ToString(*(begin + pivotPick)) + "}";

        return make_pair(pivotPick, comment);
      }
  };

  template <typename IT>
  class PickLast
  {
    public:
      std::pair<uint16_t, std::string> operator()
      (const IT& begin, const IT& end, const int offset)
      {
        // Get first element
        const auto lenght = static_cast<const int>(std::distance(begin, end));
        const uint16_t pivotPick = lenght - 1;

        // Comment
        const std::string comment = "Pick last item as Pivot within [" + ToString(offset) + ", " +
                                    ToString(offset + lenght - 1) + "] --> [" +
                                    ToString(offset + pivotPick) + "]{" +
                                    ToString(*(begin + pivotPick)) + "}";

        return make_pair(pivotPick, comment);
      }
  };

  template <typename IT>
  class PickMiddle
  {
    public:
      std::pair<uint16_t, std::string> operator()
      (const IT& begin, const IT& end, const int offset)
      {
        // Get first element
        const auto lenght = static_cast<const int>(std::distance(begin, end));
        const uint16_t pivotPick = lenght / 2;

        // Comment
        const std::string comment = "Pick middle item as Pivot within [" + ToString(offset) + ", " +
                                    ToString(offset + lenght - 1) + "] --> [" +
                                    ToString(offset + pivotPick) + "]{" +
                                    ToString(*(begin + pivotPick)) + "}";

        return make_pair(pivotPick, comment);
      }
  };

  template <typename IT>
  class PickThreeMedian
  {
    public:
      std::pair<uint16_t, std::string> operator()
      (const IT& begin, const IT& end, const int offset)
      {
        // Get middle element
        const auto lenght = static_cast<const int>(std::distance(begin, end));
        uint16_t pivotPick = lenght / 2;
        const uint16_t middleValue = *(begin + pivotPick);

        // Compute median
        int x = *begin - middleValue;
        int y = middleValue - *(end - 1);
        int z = *begin - *(end - 1);
        if (x * y > 0) pivotPick = pivotPick;
        else if (x * z > 0) pivotPick = lenght - 1;
        else  pivotPick = 0;

        // Comment
        const std::string comment = "Pick median (of three values) as Pivot within [" +
                                    ToString(offset) + ", " +
                                    ToString(offset + lenght - 1) + "] --> [" +
                                    ToString(offset + pivotPick) + "]{" +
                                    ToString(*(begin + pivotPick)) + "}";

        return make_pair(pivotPick, comment);
      }
  };

  template <typename IT>
  class PickRandom
  {
    public:
      PickRandom(const int seed = 130888) : random(seed) {}
      std::pair<uint16_t, std::string> operator()
      (const IT& begin, const IT& end, const int offset)
      {
        // Get first element
        const auto lenght = static_cast<const int>(std::distance(begin, end));
        const uint16_t pivotPick = random() % lenght;

        // Comment
        const std::string comment = "Pick random item as Pivot within [" + ToString(offset) + ", " +
                                    ToString(offset + lenght - 1) + "] --> [" +
                                    ToString(offset + pivotPick) + "]{" +
                                    ToString(*(begin + pivotPick)) + "}";

        return make_pair(pivotPick, comment);
      }

    private:
      std::mt19937 random;
  };

  /// @class QuickLog
  ///
  template <typename IT,
            typename Compare = std::less_equal<typename std::iterator_traits<IT>::value_type>,
            typename Picker = PickThreeMedian<IT>>
  class QuickLog
  {
    typedef PartitionLog<IT, Compare> Partition;
    typedef QuickLog<IT, Compare, Picker> QuickSort;

    public:
      static const String GetName() { return "Quick Sort"; }

      // Assert correct JSON construction.
      ~QuickLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with QuickLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const IT& begin, const IT& end,
                            VecStats& stats, const int offset = 0)
      {
        std::unique_ptr<QuickLog> builder = std::unique_ptr<QuickLog>(new QuickLog(os));
        builder->Write(opts, begin, end, stats, offset);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with QuickLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const IT& begin, const IT& end,
                           VecStats& stats, const int offset = 0)
      {
        Write(writer, opts, begin, end, stats, offset);

        return writer;
      }

    private:
      QuickLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                              writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      QuickLog operator=(QuickLog&) {} // Not Implemented

      ///
      /// \brief Write
      /// \param opts
      /// \param begin
      /// \param end
      /// \param offset
      /// \return
      ///
      bool Write(Options opts, const IT& begin, const IT& end, VecStats& stats, const int offset)
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

        Algo_Traits<QuickLog>::Build(writer, opts);                 // Write description
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
          Iterator::Build(writer, kSeqName, "begin", offset);
          Iterator::Build(writer, kSeqName, "end", offset + static_cast<int>(std::distance(begin, end)));
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
        // Local logged variables
        writer.Key("locals");
        writer.StartArray();

        const auto _pivot = Picker()(begin, end, offset);
        ++stats.nbOtherAccess;

        auto pivot = Iterator::BuildIt<IT>(writer, kSeqName, "pivot",
                                           offset + _pivot.first, begin + _pivot.first, _pivot.second);
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();

        // Proceed partition
        auto newPivot = Partition::Build(writer, OpIsSub, begin, pivot, end, stats, offset);
        auto newOffset = offset + std::distance(begin, newPivot);

        Comment::Build(writer, "Recurse on left-side partition");
        QuickSort::Build(writer, OpIsSub, begin, newPivot, stats, offset);
        Comment::Build(writer, "Recurse on right-side partition");
        ++stats.nbOtherAccess;
        QuickSort::Build(writer, OpIsSub, newPivot + 1, end, stats, newOffset + 1);

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

#endif // MODULE_SORT_QUICK_HXX
