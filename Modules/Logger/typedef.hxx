#ifndef MODULE_LOGGER_TYPEDEF_HXX
#define MODULE_LOGGER_TYPEDEF_HXX

// JSON lib includes
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

// STD includes
#include <string>

namespace SHA_Logger
{
  typedef rapidjson::OStreamWrapper Stream_Type;
  typedef rapidjson::PrettyWriter<Stream_Type> Writer_Type;
  typedef const std::string String_Type;
};

#endif() // MODULE_LOGGER_TYPEDEF_HXX
