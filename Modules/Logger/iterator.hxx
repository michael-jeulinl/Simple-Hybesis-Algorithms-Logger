#ifndef MODULE_LOGGER_ITERATOR_HXX
#define MODULE_LOGGER_ITERATOR_HXX

#include <typedef.hxx>

namespace SHA_Logger
{
  /// @class Iterator parameter
  ///
  class Iterator
  {
    public:
      // @todo generic class
      // Assert correct JSON construction.
      ~Iterator() { assert(this->writer.IsComplete()); }

      /// Build - Construct a json iterator log.
      ///
      /// Instantiate a writer using the stream and write into it.
      ///
      /// @return stream reference filled up with Iterator object information,
      ///         error information in case of failure.
      static std::ostream& Build(std::ostream& os, String_Type& parentId, String_Type& name, int index)
      {
        auto parameter = Iterator(os);
        parameter.Write(parentId, name, index);

        return os;
      }

      /// Build - Construct a json iterator log.
      ///
      /// Instantiate a writer using the stream and write into it.
      ///
      /// @return stream reference filled up with Iterator object information,
      ///         error information in case of failure.
      static Writer_type& Build(Writer_type& writer, String_Type& parentId, String_Type& name, int index)
      {
        // Create Iterator logger
        Write(writer, parentId, name, index);

        return writer;
      }

    private:
      Iterator(std::ostream& os) : stream(os), writer(this->stream) {}
      Iterator operator=(Iterator&) {} // Not Implemented

      bool Write(String_Type& parentId, String_Type& name, int index)
      { return Write(this->writer, parentId, name, index); }

      static bool Write(Writer_type& writer, String_Type& parentId, String_Type& name, int index)
      {
         // Add Error Object log in case of failure
        // @todo temporary code: create generic error object
        if (parentId.empty() || name.empty()) {
          writer.StartObject();
          writer.Key("type");
          writer.String("error");
          writer.Key("fct");
          writer.String("iterator::build");
          writer.Key("message");
          writer.String("parameter parentId: '" + parentId + "' or name: '" + name + "' missing.");
          writer.EndObject();
          return false;
        }

        // Add iterator information
        writer.StartObject();
        writer.Key("type");
        writer.String("iterator");
        writer.Key("id");
        writer.String(parentId + "_" + name); // Create id model
        writer.Key("name");
        writer.String(name);
        writer.Key("ref");
        writer.String(parentId);
        writer.Key("data");
        writer.Int(index);
        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_ITERATOR_HXX
