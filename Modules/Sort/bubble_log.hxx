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
#ifndef MODULE_SORT_BUBBLE_LOG_HXX
#define MODULE_SORT_BUBBLE_LOG_HXX

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
  class Swap
  {
  public:
    template <typename IT>
    void operator()(Logger& logger, const IT& first, const IT& second)
    {
      const bool sharedOwner = (first.GetOwnerRef() == second.GetOwnerRef());

      logger.StartObject();
        logger.AddEntry("type", "operation");
        logger.AddEntry("name", "Swap");

        // Owner reference(s)
        if (sharedOwner) logger.AddEntry("ref", first.GetOwnerRef());
        else
        {
          logger.StartArray("refs");
            logger.Add(first.GetOwnerRef());
            logger.Add(second.GetOwnerRef());
          logger.EndArray();
        }

        logger.StartArray("indexes");
          logger.Add(first.GetIndex());
          logger.Add(second.GetIndex());
        logger.EndArray();
      logger.EndObject();

      std::swap(*first, *second);
      first.AddSwap(true);
      second.AddSwap((sharedOwner) ? false : true);
    }
  };

  // A container will log only once the comparisons on two of its iterator
  // e.g. vector would not be able to say which iterator it owns or have owned.
  // necessaary to keep statistic reliable.
  template <typename IT, typename CompareT>
  class CompareWrap
  {
  public:
    bool operator()(const IT& first, const IT& second)
    {
      const bool sharedOwner = (first.GetOwnerRef() == second.GetOwnerRef());

      first.AddCompare(true);
      second.AddCompare((sharedOwner) ? false : true);

      return CompareT()(*first, *second);
    }
  };

  /// @class Bubble
  ///
  ///
  ///
  template <typename IT, typename Compare = std::greater<typename std::iterator_traits<IT>::value_type>>
  class Bubble
  {
    // Specification to get h_iterator (if normal iterator subBuild bubble using h_iterator as template)
    typedef CompareWrap<IT, Compare> CompareF;

    public:
      static const String GetName() { return "Bubble Sort"; }
      static const String GetVersion() { return "1.0.0"; }
      static const String GetType() { return "algorithm"; }

      // @TODO both specialization for hurna iterator that log themself otherwise create temporary
      // hurna vector to run computation with logging capability (allows to log from classic iterators)

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with AggregateInPlaceLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, const IT& begin, const IT& end)
      {
        std::unique_ptr<Bubble> builder =
          std::unique_ptr<Bubble>(new Bubble(os));
        builder->Write(begin, end);

        return os;
      }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return true in case of success, false otherwise.
      //void operator()
      static void Build(Logger& logger, const IT& begin, const IT& end)
      { Write(logger, begin, end); }

    private:
      Bubble(Ostream& os) : logger(std::unique_ptr<Logger>(new Logger(os))) {}
      Bubble operator=(Bubble&) {} // Not Implemented

      bool Write(const IT& begin, const IT& end) { return Write(*this->logger, begin, end); }

      ///
      static void Write(Logger& logger, const IT& begin, const IT& end)
      {
        logger.Start();                        // Start Logging Procedure

        Algo_Traits<Bubble>::Build(logger);    // Write description
        WriteParameters(logger, begin, end);   // Write parameters
        WriteComputation(logger, begin, end);  // Write computation

        logger.End();                          // Close Logging Procedure

      }

      ///
      static void WriteParameters(Logger& logger, const IT& begin, const IT& end)
      {
        logger.StartArray("parameters");
        if (logger.GetCurrentLevel() > 0)           // Write only iterators
        {
          logger.AddObject(begin);
          logger.AddObject(end);
        }
        else { logger.AddDataDetails(begin, end); } // Write all data
        logger.EndArray();
      }

      ///
      static void WriteComputation(Logger& logger, const IT& begin, const IT& end)
      {
        // No data to be processed
        const auto size = static_cast<const int>(std::distance(begin, end));
        if (size < 2)
        {
          logger.Comment("Sequence too small to be procesed: already sorted.");
          logger.Return("void");
          return;
        }

        // Locals
        logger.StartArray("locals");
          int endIdx = -1;
          bool hasSwapped;
          auto curIt = IT(begin, "current", true);
          auto nextIt = IT(curIt + 1, "next", true);
        logger.EndArray();

        // Computation
        logger.StartArray("logs");
          logger.StartLoop("Bubble biggest element at the end and restart to [end-i] until sorted:");
          for (auto it = begin; it < end - 1; ++it, --endIdx)
          {
            hasSwapped = false;

            // Notify new search space
            auto range = std::make_pair(0, size + endIdx);
            logger.SetRange(range);
            logger.StartLoop("Bubble up biggest value within " + RangeToString(range) + ":");
            for (curIt = begin; curIt < end + endIdx; ++curIt)
            {
              nextIt = IT(curIt + 1);
              if (CompareF()(curIt, nextIt))
              {
                logger.Comment(curIt.String() + " > " + nextIt.String() + " : Bubble up.");
                Swap()(logger, curIt, nextIt);
                hasSwapped = true;
              }
              else { logger.Comment(curIt.String() + " <= " + nextIt.String() + " : Ignore element."); }
            }
            logger.EndLoop();

            if (!hasSwapped)
            {
              logger.Comment("No swap occured: sequence is sorted.");
              break;
            }
          }
          logger.EndLoop();

          logger.Return("void");
        logger.EndArray();

        // Add Statistical informations for the main computation
        if (logger.GetCurrentLevel() == 0)
        {
          logger.StartArray("stats");
            logger.AddStats(curIt, true);
            logger.AddStats(nextIt);
          logger.EndArray();
        }
      }

      // Unique as created only at execution as a RAII ressource
      std::unique_ptr<Logger> logger; // Logger used to fill the stream
  };
}

#endif // MODULE_SORT_BUBBLE_LOG_HXX
