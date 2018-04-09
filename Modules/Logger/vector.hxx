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

// STD includes
#include <iterator>
#include <memory>
#include <vector>

namespace SHA_Logger
{
  /// @class Vector
  /// Wrap std::vector adding log operations writing and statisticals informations
  ///
  template <typename T>
  class Vector
  {
    public:
      explicit Vector(std::initializer_list<T> init, Ostream& os) :
        data(init),
        stream(std::shared_ptr<Stream>(new Stream(os))),
        writer(std::shared_ptr<Writer>(new Writer(*this->stream))) {}
      ~Vector() { assert(this->writer->IsComplete()); }

      class h_iterator : public std::iterator<std::random_access_iterator_tag, T>
      {
        public:
          h_iterator(typename std::vector<T>::iterator it,
                     Vector<T>* owner,
                     const int index,
                     const std::string& name) :
            it(it), owner(owner), index(index), name(name) {}

          T& operator *() const { return *it; }
          const h_iterator &operator ++() { ++it; return *this; }
          h_iterator operator ++(int) { h_iterator copy(*this); ++it; return copy; }
          const h_iterator &operator --() { --it; return *this; }
          h_iterator operator --(int) { h_iterator copy(*this); --it; return copy; }
          h_iterator& operator =(const h_iterator& other){ this->it =other.it; return *this;}

          bool operator ==(const h_iterator &other) const { return it == other.it; }
          bool operator !=(const h_iterator &other) const { return it != other.it; }
          bool operator  <(const h_iterator &other) const { return it  < other.it; }
          bool operator  >(const h_iterator &other) const { return it  > other.it; }
          bool operator <=(const h_iterator &other) const { return it <= other.it; }
          bool operator >=(const h_iterator &other) const { return it >= other.it; }

          h_iterator operator +(const long int& delta) const
          {
            h_iterator copy(*this);
            copy.it += delta;
            return copy;
          }

          h_iterator& operator +=(const long int& delta)
          {
            it = it + delta;
            return *this;
          }

          h_iterator operator -(const long int& delta) const
          {
            h_iterator copy(*this);
            copy.it -= delta;
            return copy;
          }

          // std::distance requierment
          long int operator -(const h_iterator& delta) const {
            return it - delta.it;
          }

          h_iterator& operator -=(const long int& delta)
          {
            it = it - delta;
            return *this;
          }

          T operator [](const long int& delta) const { return *(it + delta); }


          std::shared_ptr<Writer> GetWriter() const { return owner->GetWriter(); }

        private:
          typename std::vector<T>::iterator it;  // Iterator
          Vector<T>* owner;                      // Cannot be null as long as the iterator is valid
          int index;                             // Iterator position
          std::string name;                      // Iterator name
      };

      // Preserve normal iterator accesses
      typename std::vector<T>::iterator begin() { return data.begin(); }
      typename std::vector<T>::iterator end() { return data.end(); }

      const typename std::vector<T>::iterator cbegin() const { return data.cbegin(); }
      const typename std::vector<T>::iterator cend() const { return data.cend(); }

      // Observale iterator
      h_iterator h_begin() { return h_iterator(data.begin(), this, static_cast<int>(0), "begin"); }
      h_iterator h_end() { return h_iterator(data.end(), this, static_cast<int>(data.size() - 1), "end"); }

      // Implement Meta-Language functions

      // Statistics
      struct Stats
      {
        Stats() : nbComparisons(0), nbIterations(0), nbOtherAccess(0), nbSwaps(0) {}
        int nbComparisons;
        int nbIterations;
        int nbOtherAccess;
        int nbSwaps;
      };

      std::shared_ptr<Writer> GetWriter() const { return writer; }

    private:
      Vector(Ostream& os) : stream(std::shared_ptr<Stream>(new Stream(os))),
                            writer(std::shared_ptr<Writer>(new Writer(*this->stream))) {}
      Vector operator=(Vector&) {} // Not Implemented

      std::vector<T> data;            // Vector wrapper
      Stats stats;                    // Computation statistics
      std::shared_ptr<Stream> stream; // Stream wrapper
      std::shared_ptr<Writer> writer; // Writer used to fill the stream
  };
}

#endif // MODULE_LOGGER_VECTOR_HXX
