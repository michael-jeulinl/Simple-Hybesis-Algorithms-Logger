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
#ifndef MODULE_DS_MAZE_PRIMS_LOG_HXX
#define MODULE_DS_MAZE_PRIMS_LOG_HXX

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
  class Cell {
  public:
    Cell() : x(0), y(0), isVisited(false) {}
    Cell(unsigned int x, unsigned int y, bool isVisited) : x(x), y(y), isVisited(isVisited) {}

    unsigned int GetX() const { return this->x; }
    unsigned int GetY() const { return this->y; }

    // @todo this information should not be part the cell info itself
    static const std::shared_ptr<Cell>& Visite(const std::shared_ptr<Cell>& cell)
    {
      cell->isVisited = true;
      return cell;
    }
    bool IsVisited() const { return this->isVisited; }

    // Add a connection
    //
    void AddCellConnection(std::shared_ptr<Cell> cell)
    { connectedCells.push_back(cell); }

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
        writer.Key("connectedCells");
        writer.StartArray();
          //for (auto it = )
        writer.EndArray();
      writer.EndObject();
    }

  private:
    unsigned int x;
    unsigned int y;

    bool isVisited; // @todo cf. above
    int rootDistance; // @todo cf. above

    std::vector<std::shared_ptr<Cell>> connectedCells;
  };

  /// @class MazePrimsLog
  ///
  class MazePrimsLog
  {
    public:
      /// eg https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
      static const String GetName() { return "Maze_Prims"; }

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
      ~MazePrimsLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with MazePrimsLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const unsigned int width, const unsigned int height)
      {
        std::unique_ptr<MazePrimsLog> builder = std::unique_ptr<MazePrimsLog>(new MazePrimsLog(os));
        builder->Write(opts, width, height);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with MazePrimsLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const unsigned int width, const unsigned int height)
      {
        Write(writer, opts, width, height);

        return writer;
      }

    private:
      MazePrimsLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                             writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      MazePrimsLog operator=(MazePrimsLog&) {} // Not Implemented

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
        //Algo_Traits<MazePrimsLog>::Build(writer, opts);
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
            mazeMatrix[x].push_back(std::shared_ptr<Cell>(new Cell(x, y, false)));
        }

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

        // Initialize random generator based on Mersenne Twister algorithm
        // @todo use seed / random generator parameter (also usefull for testing purpose)
        std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());

        // Create Completely connected Maze using Prims
        // @todo use generic generator for RawArray, Matrix, Stack...
        std::set<std::shared_ptr<Cell>> pathSet;
        mazeMatrix[0][0]->SetRootDistance(0);
        pathSet.insert(Cell::Visite(mazeMatrix[0][0]));

        /// LOG LOCALS
        writer.Key("locals");
        writer.StartArray();
          /// LOG Set
          writer.StartObject();
            writer.Key("name");
            writer.String("pathSet");
            writer.Key("type");
            writer.String("Set");
            writer.Key("dataType");
            writer.String("Cell");
            writer.Key("dataIndexes");
            writer.StartArray();
              writer.StartArray();
                writer.Int(mazeMatrix[0][0]->GetX());
                writer.Int(mazeMatrix[0][0]->GetY());
              writer.EndArray();
            writer.EndArray();
          writer.EndObject();

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
        int maxDistance = 0;

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
            writer.Int(mazeMatrix[0][0]->GetX());
            writer.Int(mazeMatrix[0][0]->GetY());
          writer.EndArray();
          writer.Key("rootDistance");
          writer.Int(mazeMatrix[0][0]->GetRootDistance());
        writer.EndObject();

        while (!pathSet.empty())
        {
          // Select randomly a path to extend
          auto curCellIt = pathSet.begin();
          std::advance(curCellIt, mt() % pathSet.size());
          Cell::Visite(*curCellIt);

          // Randomly connect it to a previous visited cell
          auto curNeighboursPath = GetAvailableNeighbours(mazeMatrix, *(*curCellIt).get(), true);
          if (!curNeighboursPath.empty())
          {
            auto randIdx = mt() % curNeighboursPath.size();

            /// LOG CONNECT
            (*curCellIt)->SetRootDistance(curNeighboursPath[randIdx]->GetRootDistance() + 1);
            maxDistance = std::max(maxDistance, (*curCellIt)->GetRootDistance());

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

            (*curCellIt)->AddCellConnection(curNeighboursPath[randIdx]);
            writer.StartObject();
              writer.Key("type");
              writer.String("operation");
              writer.Key("name");
              writer.String("Connect");
              writer.Key("ref");
              writer.String("curNode");
              writer.Key("indexes");
              writer.StartArray();
                writer.Int(curNeighboursPath[randIdx]->GetX());
                writer.Int(curNeighboursPath[randIdx]->GetY());
              writer.EndArray();
            writer.EndObject();
          }

          // Get available neighbours
          auto curNeighbours = GetAvailableNeighbours(mazeMatrix, *(*curCellIt).get(), false);

          // If there is avaible node to process deeper
          if (!curNeighbours.empty())
          {
            // Randomly select a node to be processed
            auto randIdx = mt() % curNeighbours.size();

            // For each available node push it into the set to be processed
            // Only the choosen is connected
            for (unsigned int i = 0; i < curNeighbours.size(); ++i)
            {
              pathSet.insert(curNeighbours[i]);

              /// LOG PUSH
              writer.StartObject();
                writer.Key("type");
                writer.String("operation");
                writer.Key("name");
                writer.String("Push");
                writer.Key("ref");
                writer.String("pathSet");
                writer.Key("indexes");
                writer.StartArray();
                  writer.Int(curNeighbours[i]->GetX());
                  writer.Int(curNeighbours[i]->GetY());
                writer.EndArray();
              writer.EndObject();
            }
          }

          // Remove from the set the processed cell
          pathSet.erase(curCellIt);
        }

        // Write distances information
        int maxDistance = WriteDistances(writer, mazeMatrix, *(mazeMatrix[0][0].get()));

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
      /// @return neighbours that has not been visited yet.
      static std::vector<std::shared_ptr<Cell>> GetAvailableNeighbours(
            const std::vector<std::vector<std::shared_ptr<Cell>>>& mazeMatrix,
            const Cell& cell,
            const bool visited)
      {
        std::vector<std::shared_ptr<Cell>> neighbour;

        const auto curX = cell.GetX();
        const auto curY = cell.GetY();

        // Push left if available
        if (static_cast<int>(curX - 1) >= 0 &&
            ((visited) ? mazeMatrix[curX - 1][curY]->IsVisited() : !mazeMatrix[curX - 1][curY]->IsVisited()))
          neighbour.push_back(mazeMatrix[curX - 1][curY]);
        // Push bottom if available
        if (static_cast<int>(curY - 1) >= 0 &&
            ((visited) ? mazeMatrix[curX][curY - 1]->IsVisited() : !mazeMatrix[curX][curY - 1]->IsVisited()))
          neighbour.push_back(mazeMatrix[curX][curY - 1]);
        // Push top if available
        if (curX + 1 < mazeMatrix.size() &&
            ((visited) ? mazeMatrix[curX + 1][curY]->IsVisited() : !mazeMatrix[curX + 1][curY]->IsVisited()))
          neighbour.push_back(mazeMatrix[curX + 1][curY]);
        // Push right if available
        if (curY + 1 < mazeMatrix[curX].size() &&
            ((visited) ? mazeMatrix[curX][curY + 1]->IsVisited() : !mazeMatrix[curX][curY + 1]->IsVisited()))
          neighbour.push_back(mazeMatrix[curX][curY + 1]);

        return neighbour;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_DS_MAZE_PRIMS_LOG_HXX
