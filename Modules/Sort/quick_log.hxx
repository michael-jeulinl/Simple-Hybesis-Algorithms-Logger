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

namespace SHA_Logger
{
  /// @class QuickLog
  ///
  template <typename IT, typename Compare>
  class QuickLog
  {
    public:
      /// Write algorithm information
      /// @TODO Use string litteral for JSON description within c++ code
      /// @e.g https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
      static std::string GetName() { return "Quick Sort"; }

      /// Write algorithm information
      /// @TODO Use string litteral for JSON description within c++ code
      static bool WriteInfo(Writer_Type& writer) { return true; }

      /// Write algorithm decription
      /// @TODO Use string litteral for JSON description within c++ code
      static bool WriteDoc(Writer_Type& writer) { return true; }

      /// Write algorithm sources
      /// @TODO Use string litteral for JSON description within c++ code
      static bool WriteSrc(Writer_Type& writer) { return true; }

      // Assert correct JSON construction.
      ~QuickLog() { assert(this->writer.IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with QuickLog object information,
      ///         error object information in case of failure.
      static Ostream_T& Build(Ostream_T& os, Options opts, IT& begin, IT& end)
      {
        auto parameter = QuickLog(os);
        parameter.Write(opts, begin, end);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with QuickLog object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer, Options opts, IT& begin, IT& end)
      {
        Write(writer, opts, begin, end);

        return writer;
      }

    private:
      QuickLog(std::ostream& os) : stream(os), writer(this->stream) {}
      QuickLog operator=(QuickLog&) {}                                  // Not Implemented

      bool Write(Options opts, IT& begin, IT& end)
      { return Write(this->writer, opts, begin, end); }

      static bool Write(Writer_Type& writer, Options opts, IT& begin, IT& end)
      {
        // Do not write sequence if no data to be processed
        const auto distance = static_cast<const int>(std::distance(begin, end));
        if (distance < 2)
        {
          Comment::Build(writer, "Sequence size too small to be processed.", 0);
          Operation::Return<bool>(writer, true);
          return true;
        }

        writer.StartObject();

        // Write description
        Algo_Traits<QuickLog>::Build(writer, opts);

        // Write parameters
        WriteParameters(writer, begin, end);

        // Write computation
        WriteComputation(writer, begin, end);

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer_Type& writer, IT& begin, IT& end)
      {
        writer.Key("parameters");
        writer.StartArray();
        Array<IT>::Build(writer, kSeqName, "begin", begin, "end", end);
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer_Type& writer, IT& begin, IT& end)
      {
        const auto _pivotIdx = static_cast<const int>(rand() % (end - begin));

        // Local logged variables
        writer.Key("locals");
        writer.StartArray();
        auto pivot = Iterator::BuildIt<IT>
          (writer, kSeqName, "pivot", _pivotIdx, begin + _pivotIdx, "Pick Random Pivot within [begin, end]");
        writer.EndArray();

        writer.Key("logs");
        writer.StartArray();
        PartitionLog<IT, Compare>::Build(writer, OpIsSub, begin, pivot, end); // Proceed partition
        QuickLog<IT, Compare>::Build(writer, OpIsSub, begin, pivot);          // Recurse on first partition
        QuickLog<IT, Compare>::Build(writer, OpIsSub, pivot + 1, end);        // Recurse on second partition
        writer.EndArray();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_SORT_QUICK_HXX
