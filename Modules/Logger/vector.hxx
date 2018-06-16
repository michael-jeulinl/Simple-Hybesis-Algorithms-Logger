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
#ifndef MODULE_LOGGER_VECTOR_HXX
#define MODULE_LOGGER_VECTOR_HXX

#include <Logger/iterator.hxx>
#include <Logger/value_type.hxx>
#include <Logger/typedef.hxx>

// STD includes
#include <iterator>
#include <memory>
#include <vector>

namespace SHA_Logger
{
  // Should be its own class
  struct Logger
  {
    Logger(Ostream& os) :
      currentLevel(-1),
      stream(std::unique_ptr<Stream>(new Stream(os))),
      writer(std::unique_ptr<Writer>(new Writer(*this->stream))) {}

    ~Logger() { assert(this->writer->IsComplete()); }

    void AddEntry(const String& key, const String& value)
    {
      writer->Key(key);
      writer->String(value);
    }

    void AddEntry(const String& key, const int& value)
    {
      writer->Key(key);
      writer->Int(value);
    }

    template <typename T>
    void AddObject(const T& object, const String& key = "")
    {
      if (!key.empty()) writer->Key(key);
      object.Log();
    }

    template <typename IT>
    void AddData(const IT& begin, const IT& end, const String& key = "")
    {
      if (!key.empty()) writer->Key(key);

      StarArray();
        for (auto it = begin; it != end; ++it) Add(*it);
      EndArray();
    }

    template <typename IT>
    void AddStats(const IT& it, bool logOwner = false)
    {
      if (logOwner)
        it.LogOwnerStats();
      it.LogStats();
    }

    template <typename IT, typename Comp>
    static bool Compare(IT& first, IT& second)
    {
      // first.increment
      // second.increment

      return true;//compare(*first, *second);
    }

    template <typename T>
    void Return(const T& value)
    {
      StartObject();
        AddEntry("type", "operation");
        AddEntry("name", "Return");
        AddEntry("data", value);
      EndObject();
    }

    template <typename IT>
    void AddDataDetails(const IT& begin, const IT& end, const String& key = "")
    {
      if (!key.empty()) writer->Key(key);

      /// @todo use raw

      StartObject();
        AddEntry("type", begin.GetOwnerType());
        AddEntry("name", begin.GetOwnerRef());
        AddData(begin, end, "data");

        StartArray("iterators");
          AddObject(begin);
          AddObject(end);
        EndArray();
      EndObject();
    }

    void Start()
    {
      ++currentLevel;
      writer->StartObject();
    }

    void StartLoop(const String& comment = "")
    {
      if (!comment.empty()) Comment(comment);
      ++currentLevel;
    }

    void EndLoop(const String& comment = "")
    {
      --currentLevel;
      if (!comment.empty()) Comment(comment);
    }

    void End()
    {
      --currentLevel;
      writer->EndObject();
    }

    /// @todo make it tuple
    template<class Pair>
    void SetRange(const Pair& range)
    {
      StartObject();
        AddEntry("type", "operation");
        AddEntry("name", "setRange");
        StartArray("range");
          writer->Int(range.first);
          writer->Int(range.second);
        EndArray();
      EndObject();
    }



    void StartArray(const String& key)
    {
      writer->Key(key);
      writer->StartArray();
    }
    void StarArray() { writer->StartArray(); }
    void EndArray() { writer->EndArray(); }

    void StartObject(const String& key = "")
    {
      if (!key.empty()) writer->Key(key);
      writer->StartObject();
    }
    void EndObject() { writer->EndObject(); }


    void Comment(const String& message, const String& extent = "")
    {
      writer->StartObject();

      AddEntry("type", "comment");
      AddEntry("message", message);
      if (currentLevel != 0) { writer->Key("level"); writer->Int(currentLevel); }
      if (extent != "") { AddEntry("extent", extent); }

      writer->EndObject();
    }

    int GetCurrentLevel() const { return currentLevel; }

    // Specifications
    void Add(bool value)          { writer->Bool(value); }
    void Add(double value)        { writer->Double(value); }
    void Add(char value)          { writer->String(String(1, value)); }
    void Add(int value)           { writer->Int(value); }
    void Add(int64_t value)       { writer->Int64(value); }
    void Add(const String& value) { writer->String(value); }
    void Add(unsigned value)      { writer->Uint(value); }
    void Add(uint64_t value)      { writer->Uint64(value); }

    private:
      int currentLevel;
      std::unique_ptr<Stream> stream; // Stream wrapper
      std::unique_ptr<Writer> writer; // Writer used to fill the stream
  };

  /// @class Vector
  /// Wrap std::vector adding log operations writing and statisticals informations
  ///
  template <typename T>
  class Vector
  {
    public:
    static const String GetType() { return "array"; }
    static const String GetVersion() { return "1.0.0"; }

    // Vector and Vector::h_iterator Statistics
    struct Stats
    {
      Stats() : nbAccess(0), nbCompares(0), nbIterations(0), nbSwaps(0) {}
      int nbAccess;
      int nbCompares;
      int nbIterations;
      int nbSwaps;
    };

    explicit Vector(Ostream& os, std::initializer_list<T> init = {}) :
      logger(std::shared_ptr<Logger>(new Logger(os))),
      data(init) {}

    explicit Vector(std::shared_ptr<Logger> logger, std::initializer_list<T> init) :
      logger(logger),
      data(init) {}

      class h_iterator : public std::iterator<std::random_access_iterator_tag, T>
      {
        public:
          ///
          /// \brief h_iterator
          /// \param it
          /// \param owner
          /// \param index
          /// \param name
          ///
          h_iterator(typename std::vector<T>::iterator it,
                     Vector<T>* owner,
                     const int index,
                     const std::string& name) :
            it(it), owner(owner), index(index), name(name), comment(""), logOperations(false), stats() {}

          ///
          /// \brief h_iterator
          /// \param it
          /// \param name
          /// \param logOperations
          ///
          h_iterator(const h_iterator& it, const std::string& name="", bool logOperations = false) : it(it.it)
          {
            // Owner and index
            this->owner = it.owner;
            this->index = it.index;

            // Not copied fields
            this->name = name;
            this->logOperations = logOperations;

            // Log
            if (logOperations) this->Log();
          }

          ///
          /// \brief operator *
          /// \return
          ///
          T& operator *() const
          {
            // Stats
            ++this->stats.nbAccess;
            if (this->logOperations) this->owner->AddAccess();

            return *this->it;
          }

          ///
          /// \brief operator ++
          /// \return
          ///
          const h_iterator &operator ++()
          {
            // Increment
            ++this->it;
            ++this->index;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations)
            {
              this->owner->AddIteration();
              this->LogNewIndex();
            }

            return *this;
          }

          ///
          /// \brief operator ++
          /// \return
          ///
          h_iterator operator ++(int)
          {
            // Copy and Increment
            h_iterator copy(*this);
            ++this->it;
            ++this->index;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations)
            {
              this->owner->AddIteration();
              this->LogNewIndex();
            }

            return copy;
          }

          ///
          /// \brief operator --
          /// \return
          ///
          const h_iterator &operator --()
          {
            // Increment
            --this->it;
            --this->index;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations)
            {
              this->owner->AddIteration();
              this->LogNewIndex();
            }

            return *this;
          }

          ///
          /// \brief operator --
          /// \return
          ///
          h_iterator operator --(int)
          {
            // Copy and Increment
            h_iterator copy(*this);
            --this->it;
            --this->index;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations)
            {
              this->owner->AddIteration();
              this->LogNewIndex();
            }

            return copy;
          }

          ///
          /// \brief operator =
          /// \param other
          /// \return
          ///
          h_iterator& operator =(const h_iterator& other)
          {
            // Copy
            this->it = other.it;
            this->index = other.index;
            this->owner = other.owner;

            // Log
            if (this->logOperations) this->LogNewIndex();

            return *this;
          }

          ///
          /// \brief operator +
          /// \param delta
          /// \return
          ///
          h_iterator operator +(const long int& delta) const
          {
            // Copy
            h_iterator copy(*this);
            copy.it += delta;
            copy.index += delta;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations) this->owner->AddIteration();

            return copy;
          }

          ///
          /// \brief operator +=
          /// \param delta
          /// \return
          ///
          h_iterator& operator +=(const long int& delta)
          {
            // Increment
            this->it += delta;
            this->index += delta;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations)
            {
              this->owner->AddIteration();
              this->LogNewIndex();
            }

            return *this;
          }

          ///
          /// \brief operator -
          /// \param delta
          /// \return
          ///
          h_iterator operator -(const long int& delta) const
          {
            // Copy
            h_iterator copy(*this);
            copy.it -= delta;
            copy.index -= delta;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations) this->owner->AddIteration();

            return copy;
          }

          ///
          /// \brief operator -=
          /// \param delta
          /// \return
          ///
          h_iterator& operator -=(const long int& delta)
          {
            // Decrement
            this->it -= delta;
            this->index -= delta;

            // Stats & Log
            ++this->stats.nbIterations;
            if (this->logOperations)
            {
              this->owner->AddIteration();
              this->LogNewIndex();
            }

            return *this;
          }

          // Do not involve logging operation
          long int operator -(const h_iterator& delta) const { return it - delta.it; } // std::distance
          bool operator ==(const h_iterator &other) const { return it == other.it; }
          bool operator !=(const h_iterator &other) const { return it != other.it; }
          bool operator  <(const h_iterator &other) const { return it  < other.it; }
          bool operator  >(const h_iterator &other) const {  return it  > other.it; }
          bool operator <=(const h_iterator &other) const { return it <= other.it; }
          bool operator >=(const h_iterator &other) const { return it >= other.it; }
          T operator [](const long int& delta) const { return *(it + delta); }


          // Accessor
          int GetIndex() const { return index; }
          int GetName() const { return name; }

          // Owner Access
          std::string GetOwnerRef() const { return owner->GetRef(); }
          std::string GetOwnerType() const { return Vector::GetType(); }


          // Log itself
          void Log() const
          {
            auto logger = this->owner->GetLogger();
            logger->StartObject();

            // Add iterator information
            logger->AddEntry("type", "iterator");
            logger->AddEntry("name", this->name);
            logger->AddEntry("ref", this->GetOwnerRef());
            logger->AddEntry("data", this->index);
            if (!this->comment.empty()) logger->AddEntry("comment", comment);

            logger->EndObject();
          }

          void LogNewIndex() const
          {
            if (!this->logOperations)
              return;

            auto logger = this->owner->GetLogger();
            logger->StartObject();

            // Add iterator information
            logger->AddEntry("type", "operation");
            logger->AddEntry("name", "Set");
            logger->AddEntry("ref", this->name);
            logger->AddEntry("data", this->index);

            logger->EndObject();
          }

          void LogOwnerStats() const { this->owner->LogStats(); }

          void LogStats() const
          {
            auto logger = this->owner->GetLogger();

            // StartBuild - Main information
            logger->StartObject();
            logger->AddEntry("type", "iterator");
            logger->AddEntry("name", this->name);
            logger->AddEntry("ref", this->GetOwnerRef());
            if (!this->comment.empty()) logger->AddEntry("comment", this->comment);

            // Stats
            logger->AddEntry("nbAccess", this->stats.nbAccess);
            logger->AddEntry("nbCompares", this->stats.nbCompares);
            logger->AddEntry("nbIterations", this->stats.nbIterations);
            logger->AddEntry("nbSwaps", this->stats.nbSwaps);

            // Finish object
            logger->EndObject();
          }

          void LogOperations(bool logOperations) { this->logOperations = logOperations; }

          std::string String() const { return name + "[" + ToString(index) + "]{" + ToString(*it) + "}"; }


          // Statistics
          void AddCompare(bool propagateOwner) const
          {
            ++this->stats.nbCompares;
            if (propagateOwner) this->owner->AddCompare();
          }


          void AddSwap(bool propagateOwner) const
          {
            ++this->stats.nbSwaps;
            if (propagateOwner) this->owner->AddSwap();
          }

        private:
          typename std::vector<T>::iterator it;  // Iterator
          Vector<T>* owner;                      // Cannot be null as long as the iterator is valid
          int index;                             // Iterator position
          std::string name;                      // Iterator name
          std::string comment;                   // Add a comment to the variable (may be useful as caption)
          bool logOperations;                    // Whether or not log occuring operation (default to false)

          // Stats
          mutable Stats stats;
      };

      // Preserve normal iterator accesses
      typename std::vector<T>::iterator begin() { return data.begin(); }
      typename std::vector<T>::iterator end() { return data.end(); }

      const typename std::vector<T>::iterator cbegin() const { return data.cbegin(); }
      const typename std::vector<T>::iterator cend() const { return data.cend(); }

      // Observale iterator
      h_iterator h_begin() { return h_iterator(data.begin(), this, static_cast<int>(0), "begin"); }
      h_iterator h_end() { return h_iterator(data.end(), this, static_cast<int>(data.size()), "end"); }

      // @TODO Implement Meta-Language functions (swap/etc)
      void Log() const
      {
        // StartBuild - Main information
        logger->StartObject();
          logger->AddEntry("type", GetType());
          logger->AddEntry("name", this->GetRef());

          // Add data
          logger->StartArray("data");
            for (auto it = begin; it != end; ++it) logger->Add(*it);
          logger->EndArray();

        // Finish object
        logger->EndObject();
      }

      void LogStats() const
      {
        // StartBuild - Main information
        logger->StartObject();
          logger->AddEntry("type", GetType());
          logger->AddEntry("name", this->GetRef());

          // Stats
          logger->AddEntry("nbAccess", stats.nbAccess);
          logger->AddEntry("nbCompares", stats.nbCompares);
          logger->AddEntry("nbIterations", stats.nbIterations);
          logger->AddEntry("nbSwaps", stats.nbSwaps);

        // Finish object
        logger->EndObject();
      }

      void AddAccess() const { ++stats.nbAccess; }
      void AddCompare() const { ++stats.nbCompares; }
      void AddIteration() const { ++stats.nbIterations; }
      void AddSwap() const { ++stats.nbSwaps; }

      // Accessors
      std::shared_ptr<Logger> GetLogger() { return logger; }
      std::string GetRef() const { return "sequence"; }

    private:
      Vector operator=(Vector&) {}    // Not Implemented
      std::shared_ptr<Logger> logger; // Logger
      std::vector<T> data;            // Vector wrapper
      mutable Stats stats;            // Computation statistics
  };
}

#endif // MODULE_LOGGER_VECTOR_HXX
