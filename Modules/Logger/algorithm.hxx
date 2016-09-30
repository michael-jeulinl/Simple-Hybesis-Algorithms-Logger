#ifndef MODULE_LOGGER_ALGORITHM_HXX
#define MODULE_LOGGER_ALGORITHM_HXX

#include <typedef.hxx>

namespace SHA_Logger
{
  /// @class Algo_Traits
  ///
  template <typename Algo>
  class Algo_Traits
  {
    public:
      /// Instantiate a new json writer using the stream passed as
      /// argument and write algorithm information.
      ///
      /// @return stream reference filled up with Algo object information,
      ///         error information in case of failure.
      static std::ostream& Build(std::ostream& os)
      {
        auto algo = Algo_Traits(os);
        algo.Write();

        return os;
      }

      /// Use json writer passed as parameter to write Algo information.
      ///
      /// @return stream reference filled up with Algo object information,
      ///         error information in case of failure.
      static Writer_Type& Build(Writer_Type& writer)
      {
        Write(writer);

        return writer;
      }

    private:
      Algo_Traits(std::ostream& os) : stream(os), writer(this->stream) {}
      Algo_Traits operator=(Algo_Traits&) {}                              // Not Implemented

      bool Write() { return Write(this->writer); }

      static bool Write(Writer_Type& writer)
      {
        writer.StartObject();
        writer.Key("type");
        writer.String(GetType());
        writer.Key("version");
        writer.String(Algo::GetVersion());
        writer.Key("author");
        writer.String(Algo::GetAuthor());
        writer.Key("name");
        writer.String(Algo::GetName());
        writer.Key("module");
        writer.String(Algo::GetModule());
        writer.EndObject();

        return true;
      }

      static const std::string GetType() { return "algorithm"; }

      Stream_Type stream; // Stream wrapper
      Writer_Type writer; // Writer used to fill the stream
  };
};

#endif() // MODULE_LOGGER_ALGORITHM_HXX
