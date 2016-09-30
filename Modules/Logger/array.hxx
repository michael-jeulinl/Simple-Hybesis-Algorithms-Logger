#ifndef MODULE_LOGGER_ARRAY_HXX
#define MODULE_LOGGER_ARRAY_HXX

#include <iterator.hxx>
#include <value_type.hxx>

namespace SHA_Logger
{
  /// @class Array parameter
  ///
  template <typename IteratorT>
  class Array
  {
    public:
      // @todo generic class
      ~Array() { assert(this->writer.IsComplete()); }

      /// Build array parameter within stream
      ///
      /// @return Parameter logger unique pointer to be owned, nullptr if construction failed.
      static std::ostream& Build(std::ostream& os, String_Type& id,
                                 String_Type& beginName, const IteratorT& begin,
                                 String_Type& endName, const IteratorT& end)
      {
        // Create Iterator logger
        auto parameter = Array(os);
        parameter.Write(id, beginName, begin, endName, end);

        return os;
      }

    private:
      Array(std::ostream& os) : stream(os), writer(this->stream) {}
      Array operator=(Array&) {}                                    // Not Implemented

      bool Write(String_Type& id,
                 String_Type& beginName, const IteratorT& begin,
                 String_Type& endName, const IteratorT& end)
      { return Write(this->writer, id, beginName, begin, endName, end); }

      static bool Write(Writer_type& writer, String_Type& id,
                        String_Type& beginName, const IteratorT& begin,
                        String_Type& endName, const IteratorT& end)
      {
         // Add Error Object log in case of failure
        // @todo temporary code: create generic error object
        // PARAM: CreateError: std::map<string, string> for: if (empty --> error + return false)
        // Check if only macro usage retrieving fct name + line
        if (id.empty() || beginName.empty() || endName.empty()) {
          writer.StartObject();
          writer.Key("type");
          writer.String("error");
          writer.Key("fct");
          writer.String("array::build");
          writer.Key("message");
          writer.String("parameter id: '" + id + "' or name: '" + beginName + "'...missing.");
          writer.EndObject();

          return false;
        }

        // @todo create generic error object
        // 2*N for non random accessible iterator
        const int kdataSize = static_cast<int>(std::distance(begin, end));
        if (kdataSize < 1)
        {
          writer.StartObject();
          writer.Key("type");
          writer.String("error");
          writer.Key("fct");
          writer.String("array::build");
          writer.Key("message");
          writer.String("undefined or empty sequence.");
          writer.EndObject();

          return false;
        }

        // StartBuild - Main information
        writer.StartObject();
        writer.Key("type");
        writer.String("array");
        writer.Key("id");
        writer.String(id);

        // Add data
        writer.Key("data");
        ValueType::BuildArray<IteratorT>(writer, begin, end);

        // Add Iterators
        writer.Key("iterators");
        writer.StartArray();
        Iterator::Build(writer, id, beginName, 0);
        Iterator::Build(writer, id, endName, static_cast<int>(kdataSize) + 1);
        writer.EndArray();

        // Finish object
        writer.EndObject();

        return true;
      }

      Stream_Type stream; // Stream wrapper
      Writer_type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_ARRAY_HXX
