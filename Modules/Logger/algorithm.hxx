#ifndef MODULE_LOGGER_ALGO_HXX
#define MODULE_LOGGER_ALGO_HXX

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

// STD includes
#include <memory>

#include <iostream>

namespace SHA_Logger
{
  /// @class Logger
  ///
  /// @todo may be basic class of specified logger (JSON/XML...)
  typedef rapidjson::StringBuffer Buffer_Type;
  typedef rapidjson::PrettyWriter<Buffer_Type> Writer_type;
  class Algo
  {
    public:
      /// Build - Construct in a naive way a Binary Search Tree given an unordered sequence of elements.
      ///
      /// @return Algo logger unique pointer to be owned, nullptr if construction failed.
      static std::unique_ptr<Algo> Build(const std::string& name)
      {
        // Create algorithm logger
        auto algo = std::unique_ptr<Algo>(new Algo());

        return algo;
      }
      
      // @todo temporary code
      ~Algo()
      {
        std::cout << strBuffer.GetString() << std::endl;
      }

      //static std::unique_ptr<Algo> BuildFromJson(const std::string& name, ostream& stream){}

    private:
      Algo() : strBuffer(), writer(std::unique_ptr<Writer_type>(new Writer_type(this->strBuffer))) 
      {
        // @todo temporary code for poc
        writer->StartObject();
        writer->Key("type");
        writer->String("algorithm");
        writer->Key("version");
        writer->Double(0.1);
        writer->EndObject();
      }

      Algo(Algo&) {}           // Not Implemented
      Algo operator=(Algo&) {} // Not Implemented

      Buffer_Type strBuffer;               // Temporary Variable
      std::unique_ptr<Writer_type> writer; // Temporary Variable
  };
};

#endif() // MODULE_LOGGER_ALGO_HXX
