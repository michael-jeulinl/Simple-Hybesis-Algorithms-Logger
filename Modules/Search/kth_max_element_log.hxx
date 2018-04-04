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
#ifndef MODULE_SEARCH_KTH_ELEMENT_LOG_HXX
#define MODULE_SEARCH_KTH_ELEMENT_LOG_HXX

#include <Logger/algorithm.hxx>
#include <Logger/array.hxx>
#include <Logger/comment.hxx>
#include <Logger/operation.hxx>
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>

#include <Sort/partition_log.hxx>

namespace SHA_Logger
{
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

  /// @class KthElementLog
  ///
  template <typename IT,
            typename Compare = std::less_equal<typename std::iterator_traits<IT>::value_type>,
            typename Picker = PickLast<IT>>
  class KthElementLog
  {
    typedef PartitionLog<IT, Compare> Partition;
    typedef KthElementLog<IT, Compare> KthElement;

    public:
      static const String GetName() { return "Kth Element Search"; }

      // Assert correct JSON construction.
      ~KthElementLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with KthElementLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts,
                            const IT& begin, const IT& end, unsigned int k,
                            VecStats& stats, const int offset = 0)
      {
        std::unique_ptr<KthElementLog> builder = std::unique_ptr<KthElementLog>(new KthElementLog(os));
        builder->Write(opts, begin, end, k, stats, offset);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with KthElementLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts,
                           const IT& begin, const IT& end, unsigned int k,
                           VecStats& stats, const int offset = 0)
      {
        Write(writer, opts, begin, end, k, stats, offset);

        return writer;
      }

    private:
      KthElementLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                                   writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      KthElementLog operator=(KthElementLog&) {} // Not Implemented

      bool Write(Options opts, const IT& begin, const IT& end, unsigned int k,
                 VecStats& stats, const int offset)
      { return Write(*this->writer, opts, begin, end, k, stats, offset); }

      static bool Write(Writer& writer, Options opts,
                        const IT& begin, const IT& end, unsigned int k,
                        VecStats& stats, const int offset)
      {
        // Do not write sequence if no data to be processed
        if (k >= std::distance(begin, end))
        {
          Operation::Return<bool>(writer, false);
          return false;
        }

        writer.StartObject();

        Algo_Traits<KthElement>::Build(writer, opts);         // Write description
        WriteParameters(writer, opts, begin, end, k, offset); // Write parameters
        WriteComputation(writer, opts, begin, end, k, stats, offset);      // Write computation

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer& writer, Options opts,
                                  const IT& begin, const IT& end, unsigned int k, const int offset)
      {
        writer.Key("parameters");
          writer.StartArray();
          if (opts & OpIsSub)
          {
            Iterator::Build(writer, kSeqName, "begin", offset);
            Iterator::Build(writer, kSeqName, "end", offset + static_cast<int>(std::distance(begin, end)));
            Value<unsigned int>::Build(writer, "kth", k);
          }
          else
          {
            Array<IT>::Build(writer, kSeqName, "begin", begin, "end", end);
            Value<unsigned int>::Build(writer, "kth", k);
          }
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer& writer, Options opts,
                                   const IT& begin, const IT& end, unsigned int k,
                                   VecStats& stats, const int offset)
      {
        // Pick pivot
        const auto _pivot = Picker()(begin, end, offset);

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        ++stats.nbOtherAccess;
        auto pivot = Iterator::BuildIt<IT>(writer, kSeqName, "pivot",
                                           offset + _pivot.first, begin + _pivot.first, _pivot.second);
        writer.EndArray();


        writer.Key("logs");
        writer.StartArray();

        // Proceed partition
        auto newPivot = Partition::Build(writer, OpIsSub, begin, pivot, end, stats, offset);
        const auto kPivotIndex = std::distance(begin, newPivot);

        // Return if at the kth position
        if (kPivotIndex == k) {
          Comment::Build(writer, "The new pivot index is equal to k, K'th element found: [" +
                         std::to_string(offset + k) + "]{" + std::to_string(*newPivot) + "}.", 0);

          // @TODO remove currently to trick visualization (local parameter should be better handled on sub)
          Operation::Set<int>(writer, "pivot", offset + k);
          Operation::Set<int>(writer, "it", offset + k);

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

        if (kPivotIndex > k)
        {
          Comment::Build(writer, "Index of the new pivot (the i'th smallest/biggest value) [" +
                         std::to_string(offset + kPivotIndex) + "] > k [" + std::to_string(offset + k)
                         + "] : Recurse on left-side partition.", 0);
          KthElement::Build(writer, OpIsSub, begin, newPivot, k, stats, offset);
        }
        else
        {
          Comment::Build(writer, "Index of the new pivot (the i'th smallest/biggest value) [" +
                         std::to_string(offset + kPivotIndex) + "] < k [" + std::to_string(offset + k)
                         + "] : Recurse on right-side partition.", 0);
          KthElement::Build(writer, OpIsSub, newPivot, end, k - kPivotIndex, stats, offset + kPivotIndex);
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

#endif // MODULE_SEARCH_KTH_ELEMENT_LOG_HXX
