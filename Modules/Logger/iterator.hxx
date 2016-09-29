#ifndef MODULE_LOGGER_ITERATOR_HXX
#define MODULE_LOGGER_ITERATOR_HXX

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

// STD includes
#include <iostream>

namespace SHA_Logger
{
  /// @class Iterator parameter
  ///
  class Iterator
  {
    typedef rapidjson::OStreamWrapper Stream_Type;
    typedef rapidjson::PrettyWriter<Stream_Type> Writer_type;

    public:
      // Assert correct JSON construction.
      ~Iterator() { assert(this->writer.IsComplete()); }

      static std::string GetType() { return "iterator"; }

      /// Build - Construct a json iterator log.
      ///
      /// @return stream reference filled up with Iterator object information,
      /// error information in case of failure.
      static std::ostream& Build(std::ostream& os,
                                 const std::string& parentId,
                                 const std::string& name,
                                 int index)
      {
        // Create Iterator logger
        auto parameter = Iterator(os);

        // Add Error Object log in case of failure
        // @todo temporary code: create generic error object
        if (parentId.empty() || name.empty()) {
          parameter.writer.StartObject();
          parameter.writer.Key("type");
          parameter.writer.String("error");
          parameter.writer.Key("fct");
          parameter.writer.String("iterator::build");
          parameter.writer.Key("message");
          parameter.writer.String("parameter parentId: '" + parentId + "' or name: '" + name + "' missing.");
          parameter.writer.EndObject();
          return os;
        }

        // Add iterator information
        parameter.writer.StartObject();
        parameter.writer.Key("type");
        parameter.writer.String("iterator");
        parameter.writer.Key("id");
        parameter.writer.String(parentId + "_" + name); // Create id model
        parameter.writer.Key("name");
        parameter.writer.String(name);
        parameter.writer.Key("ref");
        parameter.writer.String(parentId);
        parameter.writer.Key("data");
        parameter.writer.Int(index);
        parameter.writer.EndObject();

        return os;
      }

    private:
      Iterator(std::ostream& os) : stream(os), writer(this->stream) {}
      Iterator operator=(Iterator&) {} // Not Implemented

      rapidjson::OStreamWrapper stream; // Stream wrapper
      Writer_type writer;               // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_ITERATOR_HXX
