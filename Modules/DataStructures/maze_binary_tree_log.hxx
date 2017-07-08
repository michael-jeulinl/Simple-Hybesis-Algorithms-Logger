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
#ifndef MODULE_SORT_MAZE_LOG_HXX
#define MODULE_SORT_MAZE_LOG_HXX

#include <Logger/algorithm.hxx>
#include <Logger/array.hxx>
#include <Logger/comment.hxx>
#include <Logger/operation.hxx>
#include <Logger/typedef.hxx>
#include <Logger/value.hxx>

// STD includes
#include <chrono>
#include <memory>
#include <random>
#include <stack>
#include <vector>

#include <iostream>

namespace SHA_Logger
{
  class CellBT {
  public:
    CellBT() : x(0), y(0), isVisited(false) {}
    CellBT(unsigned int x, unsigned int y, bool isVisited) :
      x(x), y(y), isVisited(isVisited) {}

    unsigned int GetX() const { return this->x; }
    unsigned int GetY() const { return this->y; }

    // @todo this information should not be part the CellBT info itself
    static const std::shared_ptr<CellBT>& Visite(const std::shared_ptr<CellBT>& CellBT)
    {
      CellBT->isVisited = true;
      return CellBT;
    }
    bool IsVisited() const { return this->isVisited; }

    // Add a connection
    //
    void AddCellConnection(std::shared_ptr<CellBT> CellBT)
    { connectedCells.push_back(CellBT); }

    // @todo this information should not be part the CellBT info itself
    int GetRootDistance() const { return this->rootDistance; }
    void SetRootDistance(const int distance) { this->rootDistance = distance; }

    void Write(Writer& writer)
    {
      writer.StartObject();
        writer.Key("x");
        writer.Int(this->x);
        writer.Key("y");
        writer.Int(this->y);
        writer.Key("connectedCellBTs");
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

    std::vector<std::shared_ptr<CellBT>> connectedCells;
  };

  /// @class MazeBinaryTreeLog
  ///
  class MazeBinaryTreeLog
  {
    public:
      /// eg https://cs.chromium.org/chromium/src/gpu/config/software_rendering_list_json.cc
      static const String GetName() { return "Binary_Tree_Maze"; }

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
      ~MazeBinaryTreeLog() { assert(this->writer->IsComplete()); }

      /// Instantiate a new json writer using the stream passed as
      /// argument, run and write algorithm computation information.
      ///
      /// @return stream reference filled up with MazeBinaryTreeLog object information,
      ///         error object information in case of failure.
      static Ostream& Build(Ostream& os, Options opts, const unsigned int width, const unsigned int height)
      {
        auto builder = std::unique_ptr<MazeBinaryTreeLog>(new MazeBinaryTreeLog(os));
        builder->Write(opts, width, height);

        return os;
      }

      /// Use json writer passed as parameter to write iterator information.
      ///
      /// @return stream reference filled up with MazeBinaryTreeLog object information,
      ///         error information in case of failure.
      static Writer& Build(Writer& writer, Options opts, const unsigned int width, const unsigned int height)
      {
        Write(writer, opts, width, height);

        return writer;
      }

    private:
      MazeBinaryTreeLog(Ostream& os) : stream(std::unique_ptr<Stream>(new Stream(os))),
                                       writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}
      MazeBinaryTreeLog operator=(MazeBinaryTreeLog&) = delete; // Not Implemented

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
          // Seed
          // Direction
        writer.EndArray();

        return true;
      }

      ///
      static bool WriteComputation(Writer& writer, const unsigned int width, const unsigned int height)
      {
        // Init Matrix
        std::vector<std::vector<std::shared_ptr<CellBT>>> mazeMatrix;
        mazeMatrix.resize(width);
        for (unsigned int x = 0; x < width; ++x)
        {
          mazeMatrix[x].reserve(height);
          for (unsigned int y = 0; y < height; ++y)
            mazeMatrix[x].push_back(std::shared_ptr<CellBT>(new CellBT(x, y, false)));
        }

        /// LOG MATRIX
        // @todo build matrix in Log (check to generalize array etc - no name / no iterator needed for raw)
        // @ build during initialization
        // the cell should be able to log itself to --> CellBT::Build
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

        // Create Completely connected Maze using Binary Tree
        mazeMatrix[0][0]->SetRootDistance(0);

        /// LOG LOCALS
        ///
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
            writer.Key("rootDistance");
            writer.Int(mazeMatrix[0][0]->GetRootDistance());
          writer.EndObject();
        writer.EndArray();

        // While there is node to be handled
        writer.Key("logs");
        writer.StartArray();
        int maxDistance = 0;

        /// LOG COMPUTATION
        for (unsigned int y = 0; y < height; ++y)
        {
          for (unsigned int x = 0; x < width; ++x)
          {
            // Get available neighbours
            auto curCell = CellBT::Visite(mazeMatrix[x][y]);
            auto curNeighbours = GetAvailableNeighbours(mazeMatrix, *curCell.get());

            if (curNeighbours.empty()) {
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
                  writer.Int(curCell->GetX());
                  writer.Int(curCell->GetY());
                writer.EndArray();
                writer.Key("rootDistance");
                writer.Int(curCell->GetRootDistance());
              writer.EndObject();

              continue;

            }

            // Randomly select a node to be processed
            auto randIdx = mt() % curNeighbours.size();
            curCell->AddCellConnection(curNeighbours[randIdx]);
            curCell->SetRootDistance(curNeighbours[randIdx]->GetRootDistance() + 1);
            maxDistance = std::max(maxDistance, curCell->GetRootDistance());

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
                writer.Int(curCell->GetX());
                writer.Int(curCell->GetY());
              writer.EndArray();
              writer.Key("rootDistance");
              writer.Int(curCell->GetRootDistance());
            writer.EndObject();

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
                writer.Int(curNeighbours[randIdx]->GetX());
                writer.Int(curNeighbours[randIdx]->GetY());
              writer.EndArray();
            writer.EndObject();
          }
        }

        /// LOG CLOSING
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
      static std::vector<std::shared_ptr<CellBT>> GetAvailableNeighbours(
            const std::vector<std::vector<std::shared_ptr<CellBT>>>& mazeMatrix,
            const CellBT& cell)
      {
        std::vector<std::shared_ptr<CellBT>> neighbours;

        const auto curX = cell.GetX();
        const auto curY = cell.GetY();

        // North West Direction
        // Push right if available
        // if (curX + 1 < mazeMatrix.size())
        if (curX > 0)
          neighbours.push_back(mazeMatrix[curX - 1][curY]);
        // Push bottom if available
        // if (curY + 1 < mazeMatrix.size())
        if (curY > 0)
          neighbours.push_back(mazeMatrix[curX][curY - 1]);

        return neighbours;
      }

      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_SORT_MAZE_LOG_HXX
