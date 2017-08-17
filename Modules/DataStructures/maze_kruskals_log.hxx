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
#ifndef MODULE_DS_MAZE_KRUSKALS_LOG_HXX
#define MODULE_DS_MAZE_KRUSKALS_LOG_HXX

#include <Logger/algorithm.hxx>
#include <Logger/array.hxx>
#include <Logger/comment.hxx>
#include <Logger/operation.hxx>
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>

// STD includes
#include <algorithm>
#include <chrono>
#include <memory>
#include <random>
#include <set>
#include <vector>

namespace SHA_Logger
{
  // Cell - Make it fully generic
  // CellInfo - Template parameter
  class Cell {
  public:
    Cell() : x(0), y(0), rootDistance(-1) {}
    Cell(unsigned int x, unsigned int y) : x(x), y(y) {}

    unsigned int GetX() const { return this->x; }
    unsigned int GetY() const { return this->y; }

    // Set Bucket connection cell
    void SetBucketId(unsigned int id) { this->bucketId = id; }

    // Get bucket cell owner
    unsigned int GetBucketId() const { return this->bucketId; }

    // @todo this information should not be part the cell info itself
    int GetRootDistance() const { return this->rootDistance; }
    void SetRootDistance(const int distance) { this->rootDistance = distance; }

    void Write(Writer& writer)
    {
      writer.StartObject();
        writer.Key("x");
        writer.Int(this->x);
        writer.Key("y");
        writer.Int(this->y);
      writer.EndObject();
    }

  private:
    unsigned int x;
    unsigned int y;

    int rootDistance;       // @todo cf. above
    unsigned int bucketId;  // @todo cf. above
  };

  /// @class MazeKruskalsLog
  ///
  class MazeKruskalsLog
  {
    public:
      /// eg https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
      static const String GetName() { return "Maze_Kruskals"; }

      /// Write datastructure information
      /// @todo Use string litteral for JSON description within c++ code
      static bool WriteInfo(Writer& writer) { return true; }

      /// Write datastructure decription
      /// @todo Use string litteral for JSON description within c++ code
      static bool WriteDoc(Writer& writer) { return true; }

      /// Write datastructure sources
      /// @todo Use string litteral for JSON description within c++ code
      static bool WriteSrc(Writer& writer) { return true; }

      // Assert correct JSON construction.
      ~MazeKruskalsLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with MazeKruskalsLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const unsigned int width, const unsigned int height)
      {
        std::unique_ptr<MazeKruskalsLog> builder = std::unique_ptr<MazeKruskalsLog>(new MazeKruskalsLog(os));
        builder->Write(opts, width, height);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with MazeKruskalsLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const unsigned int width, const unsigned int height)
      {
        Write(writer, opts, width, height);

        return writer;
      }

    private:
      MazeKruskalsLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                                      writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      MazeKruskalsLog operator=(MazeKruskalsLog&) {} // Not Implemented

      bool Write(Options opts, const unsigned int width, const unsigned int height)
      { return Write(*this->writer, opts, width, height); }

      static bool Write(Writer& writer, Options opts, const unsigned int width, const unsigned int height)
      {
        // Do not compute if data incorrect
        if (width < 1 || height < 1)
        {
          Comment::Build(writer, "Sequence size too small to be processed.", 0);
          Operation::Return<bool>(writer, true);
          return true;
        }

        writer.StartObject();

        // Write description
        //Algo_Traits<MazeKruskalsLog>::Build(writer, opts);
        writer.Key("type");
        writer.String("DataStructure");
        writer.Key("name");
        writer.String(GetName());

        // Write parameters
        WriteParameters(writer, opts, width, height);

        // Write computation
        WriteComputation(writer, width, height);

        writer.EndObject();

        return true;
      }

      ///
      static bool WriteParameters(Writer& writer, Options opts,
                                  const unsigned int width, const unsigned int height)
      {
        writer.Key("parameters");
        writer.StartArray();
          Value<unsigned int>::Build(writer, "width", width);
          Value<unsigned int>::Build(writer, "height", height);
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer& writer, const unsigned int width, const unsigned int height)
      {
        // Init Matrix
        std::vector<std::vector<std::shared_ptr<Cell>>> mazeMatrix;
        mazeMatrix.resize(width);

        for (unsigned int x = 0; x < width; ++x)
        {
          mazeMatrix[x].reserve(height);
          for (unsigned int y = 0; y < height; ++y)
            mazeMatrix[x].push_back(std::shared_ptr<Cell>(new Cell(x, y)));
        }

        // Set start cell
        mazeMatrix[0][0]->SetRootDistance(0);

        /// LOG MATRIX
        // @todo build matrix in Log (check to generalize array etc - no name / no iterator needed for raw)
        // @ build during initialization
        // the cell should be able to log itself to --> Cell::Build
        writer.Key("structure");
        writer.StartObject();
          writer.Key("type");
          writer.String("Matrix");
          writer.Key("dataType");
          writer.String("Cell");

          writer.Key("data");
          writer.StartArray();
            for (auto itX = mazeMatrix.begin(); itX != mazeMatrix.end(); ++itX)
            {
              writer.StartArray();
              for (auto itY = itX->begin(); itY != itX->end(); ++itY)
                (*itY)->Write(writer);
              writer.EndArray();
            }
          writer.EndArray();
        writer.EndObject();
        /// /LOG MATRIX

        // Initialize random generator based on Mersenne Twister algorithm
        // @todo use seed / random generator parameter (also usefull for testing purpose)
        std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());

        // Keep track of maximum path distance
        int maxDistance = 0;

        // Fill the set with the self containing cells
        std::set<std::shared_ptr<Cell>> mazeRemainingCells;
        std::vector<std::vector<std::shared_ptr<Cell>>> bucketCells;
        bucketCells.resize(height * width);
        {
          unsigned int nodeId = 0;
          for (auto itX = mazeMatrix.begin(); itX != mazeMatrix.end(); ++itX)
            for (auto itY = itX->begin(); itY != itX->end(); ++itY, ++nodeId)
            {
              (*itY)->SetBucketId(nodeId);
              mazeRemainingCells.insert(*itY);
              bucketCells[nodeId].push_back(*itY);
            }
        }

        /// LOG LOCALS
        writer.Key("locals");
        writer.StartArray();
          /// LOG CURRENT NODE
          writer.StartObject();
            writer.Key("name");
            writer.String("curNode");
            writer.Key("indexes");
            writer.StartArray();
              writer.Int(mazeMatrix[0][0]->GetX());
              writer.Int(mazeMatrix[0][0]->GetY());
            writer.EndArray();
          writer.EndObject();
        writer.EndArray();

        // While there is node to be handled
        writer.Key("logs");
        writer.StartArray();

        while (!mazeRemainingCells.empty())
        {
          // Select randomly a cell to extend
          auto curCellIt = mazeRemainingCells.begin();
          std::advance(curCellIt, mt() % mazeRemainingCells.size());

          /// LOG SET
          writer.StartObject();
            writer.Key("type");
            writer.String("operation");
            writer.Key("name");
            writer.String("Set");
            writer.Key("ref");
            writer.String("curNode");
            writer.Key("indexes");
            writer.StartArray();
              writer.Int((*curCellIt)->GetX());
              writer.Int((*curCellIt)->GetY());
            writer.EndArray();
            writer.Key("rootDistance");
            writer.Int((*curCellIt)->GetRootDistance());
          writer.EndObject();

          // Randomly get an edge cell at random
          auto neigbours = GetAvailableNeighbours(mazeMatrix, *(*curCellIt).get());
          if (!neigbours.empty())
          {
            auto randIdx = mt() % neigbours.size();

            /// LOG CONNECT
            writer.StartObject();
              writer.Key("type");
              writer.String("operation");
              writer.Key("name");
              writer.String("Connect");
              writer.Key("ref");
              writer.String("curNode");
              writer.Key("indexes");
              writer.StartArray();
                writer.Int(neigbours[randIdx]->GetX());
                writer.Int(neigbours[randIdx]->GetY());
              writer.EndArray();
            writer.EndObject();

            // Merge the sets
            MergeBucket(bucketCells, (*curCellIt)->GetBucketId(), neigbours[randIdx]->GetBucketId());
          }

          // Remove computed cell from the set
          mazeRemainingCells.erase(curCellIt);
        }

        Operation::Return<bool>(writer, true);
        writer.EndArray();

        // Add Statistical informations
        writer.Key("stats");
        writer.StartObject();
          writer.Key("maxDistance");
          writer.Int(maxDistance);
        writer.EndObject();

        return true;
      }


      ///
      /// @return available neighbours that is not already part of the same set.
      static std::vector<std::shared_ptr<Cell>> GetAvailableNeighbours(
            const std::vector<std::vector<std::shared_ptr<Cell>>>& mazeMatrix,
            const Cell& cell)
      {
        std::vector<std::shared_ptr<Cell>> neighbour;

        const auto curX = cell.GetX();
        const auto curY = cell.GetY();

        // Push left if available
        if (curX > 0 && cell.GetBucketId() != mazeMatrix[curX - 1][curY]->GetBucketId())
          neighbour.push_back(mazeMatrix[curX - 1][curY]);
        // Push bottom if available
        if (curY > 0 && cell.GetBucketId() != mazeMatrix[curX][curY - 1]->GetBucketId())
          neighbour.push_back(mazeMatrix[curX][curY - 1]);
        // Push top if available
        if (curX + 1 < mazeMatrix.size() &&
            cell.GetBucketId() != mazeMatrix[curX + 1][curY]->GetBucketId())
          neighbour.push_back(mazeMatrix[curX + 1][curY]);
        // Push right if available
        if (curY + 1 < mazeMatrix[curX].size() &&
            cell.GetBucketId() != mazeMatrix[curX][curY + 1]->GetBucketId())
          neighbour.push_back(mazeMatrix[curX][curY + 1]);

        return neighbour;
      }

      /// Merge two buckets of node together and update node bucket Id.
      ///
      /// @brief MergeBucket
      /// @param buckets
      /// @param bucketIdA
      /// @param bucketIdB
      static void MergeBucket(std::vector<std::vector<std::shared_ptr<Cell>>>& buckets,
                              unsigned int bucketIdA, unsigned int bucketIdB)
      {
        assert(bucketIdA != bucketIdB);

        // Set new bucket id for each melding cell
        for (auto it = buckets[bucketIdB].begin(); it != buckets[bucketIdB].end(); ++it)
        {
          (*it)->SetBucketId(bucketIdA);
          buckets[bucketIdA].push_back(*it);
        }

        buckets[bucketIdB].clear();
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_DS_MAZE_KRUSKALS_LOG_HXX
