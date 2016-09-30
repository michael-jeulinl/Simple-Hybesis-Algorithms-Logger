#ifndef MODULE_LOGGER_VALUE_TYPE_HXX
#define MODULE_LOGGER_VALUE_TYPE_HXX

#include <typedef.hxx>

namespace SHA_Logger
{
  /// @class ValueType
  ///
  class ValueType
  {
    public:
      // Assert correct JSON construction.
      ~ValueType() { assert(this->writer.IsComplete()); }

      /// Build - Construct a json ValueType log.
      ///
      /// @return stream reference filled up with ValueType object information,
      /// error information in case of failure.
      template <typename ValueT>
      static std::ostream& Build(std::ostream& os, const ValueT& value)
      {
        // Create ValueType and add it to the logger
        auto logger = ValueType(os);
        logger.Write(value);

        return os;
      }

      /// Build - Construct a json ValueType log.
      ///
      /// @return stream reference filled up with ValueType object information,
      /// error information in case of failure.
      template <typename IteratorT>
      static std::ostream& BuildArray(std::ostream& os, const IteratorT& begin, const IteratorT& end)
      {
        // Create ValueType logger
        auto logger = ValueType(os);

        // @todo check & log error? --> time consuming
        logger.writer.StartArray();
        for (auto it = begin; it != end; ++it)
          logger.Write(*it);
        logger.writer.EndArray();

        return os;
      }

      /// Build - Construct a json ValueType log.
      ///
      /// @return stream reference filled up with ValueType object information,
      /// error information in case of failure.
      template <typename IteratorT>
      static Writer_type& BuildArray(Writer_type& writer, const IteratorT& begin, const IteratorT& end)
      {
        // @todo check & log error? --> time consuming
        writer.StartArray();
        for (auto it = begin; it != end; ++it)
          Write(writer, *it);
        writer.EndArray();

        return writer;
      }

    private:
      ValueType(std::ostream& os) : stream(os), writer(this->stream) {}
      ValueType operator=(ValueType&) {}                                  // Not Implemented

      // Wrapper using internal writer
      template <typename ValueT>
      bool Write(ValueT value) { return this->Write(this->writer, value); }

      // Specifications
      // @todo add char specification.
      static bool Write(Writer_type& writer, bool value) { return writer.Bool(value); }
      static bool Write(Writer_type& writer, double value) { return writer.Double(value); }
      static bool Write(Writer_type& writer, int value) { return writer.Int(value); }
      static bool Write(Writer_type& writer, int64_t value) { return writer.Int64(value); }
      static bool Write(Writer_type& writer, const std::string& value) { return writer.String(value); }
      static bool Write(Writer_type& writer, unsigned value) { return writer.Uint(value); }
      static bool Write(Writer_type& writer, uint64_t value) { return writer.Uint64(value); }

      Stream_Type stream; // Stream wrapper
      Writer_type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_VALUE_TYPE_HXX
