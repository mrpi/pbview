
#include <google/protobuf/descriptor.h>
#include <google/protobuf/compiler/importer.h>

#include <sstream>
#include <fstream>
#include <string_view>

#include <range/v3/view/take_while.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

using namespace std::literals;

class ExceptionErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
   void AddError(const std::string& filename, int line, int /*column*/, const std::string& message) override
   {
      std::ostringstream ss;
      ss << filename;
      if (line != -1)
         ss << ':' << line;
      ss << ": " << message;
      //std::cout << "Error: " << ss.str() << std::endl;
      throw std::runtime_error(ss.str());
   }
};

std::string packageToNamespace(std::string_view p)
{
   std::string res;
   res.reserve(p.size() + 3);

   for (char c : p)
   {
      if (c == '.')
         res += "::";
      else
         res += c;
   }

   return res;
}

std::string cppType(const google::protobuf::FieldDescriptor& field, std::string_view MessageNameSuffix)
{
   if (field.is_repeated())
      return "auto";

   using FD = google::protobuf::FieldDescriptor;

   switch(field.type())
   {
    case FD::TYPE_DOUBLE:
       return "double";
    case FD::TYPE_FLOAT:
       return "float";
    case FD::TYPE_INT64:
    case FD::TYPE_SFIXED64:
    case FD::TYPE_SINT64:
       return "std::int64_t";
    case FD::TYPE_UINT64:
    case FD::TYPE_FIXED64:
       return "std::uint64_t";
    case FD::TYPE_INT32:
    case FD::TYPE_SFIXED32:
    case FD::TYPE_SINT32:
       return "std::int32_t";
    case FD::TYPE_FIXED32:
    case FD::TYPE_UINT32:
       return "std::uint32_t";
    case FD::TYPE_BOOL:
       return "bool";
    case FD::TYPE_STRING:
    case FD::TYPE_BYTES:
       return "std::string_view";
    case FD::TYPE_MESSAGE:
       return field.message_type()->name() + std::string{MessageNameSuffix};
    case FD::TYPE_ENUM:
       return field.enum_type()->name();
   }

   return "";
}

enum class TypeFor
{
   SingleValue,
   RepeatedField
};

std::string pbviewType(const google::protobuf::FieldDescriptor& field, TypeFor typeFor)
{
   using FD = google::protobuf::FieldDescriptor;

   switch(field.type())
   {
    case FD::TYPE_DOUBLE:
       return "pbview::type::Double"s;
    case FD::TYPE_FLOAT:
       return "pbview::type::Float"s;
    case FD::TYPE_INT64:
       return "pbview::type::Int64"s;
    case FD::TYPE_SFIXED64:
       return "pbview::type::Sfixed64"s; 
    case FD::TYPE_SINT64:
       return "pbview::type::Sint64"s;
    case FD::TYPE_UINT64:
       return "pbview::type::Uint64"s;
    case FD::TYPE_FIXED64:
       return "pbview::type::Fixed64"s;
    case FD::TYPE_INT32:
       return "pbview::type::Int32"s;
    case FD::TYPE_SFIXED32:
       return "pbview::type::Sfixed32"s;
    case FD::TYPE_SINT32:
       return "pbview::type::Sint32"s;
    case FD::TYPE_FIXED32:
       return "pbview::type::Fixed32"s; 
    case FD::TYPE_UINT32:
       return "pbview::type::Uint32"s;
    case FD::TYPE_BOOL:
       return "pbview::type::Bool"s;
    case FD::TYPE_STRING:
       return "pbview::type::String"s;
    case FD::TYPE_BYTES:
       return "pbview::type::Bytes"s;
    case FD::TYPE_MESSAGE:
       return field.message_type()->name() + "ViewBase<BinView>";
    case FD::TYPE_ENUM:
       if (typeFor == TypeFor::RepeatedField)
          return "pbview::type::EnumUntyped";
       else
          return "pbview::type::Enum<"s + field.enum_type()->name() + ">";
   }

   return ""s;
}

std::string replaceProtoExtension(std::string_view protoFileName, std::string_view newExtension)
{
   constexpr auto ext = ".proto"sv;
   if (protoFileName.size() <= ext.size() || protoFileName.substr(protoFileName.size() - ext.size()) != ext)
      throw std::runtime_error{"Invalid filename '" + std::string{protoFileName} +"', has to be *.proto file"};

   protoFileName.remove_suffix(ext.size());
   return std::string{protoFileName}.append(newExtension);
}

std::string numberConstant(const google::protobuf::FieldDescriptor& field)
{
   auto camelName = field.camelcase_name();
   camelName[0] = std::toupper(camelName[0]);
   return "k" + camelName + "FieldNumber";   
}

struct VarImpl
{
   static constexpr auto TemplateArgs = "typename... Args"sv;
   static constexpr auto NameSuffix = "Var"sv;
   static constexpr auto LookupTemplate = "VariantFor"sv;
   static constexpr auto DataType = "std::variant<Args...>"sv;

   static std::string buildFullNameSuffix(const google::protobuf::FieldDescriptor& field)
   {
      std::ostringstream oss;
      oss << "Var<std::remove_reference_t<decltype(pbview::impl::wrap(pbview::impl::unwrap(std::declval<const Args>())." << field.name() << "()))>...>";
      return oss.str();
   }

   static void writeViewConstructors(std::ostream& os, const google::protobuf::Descriptor& desc)
   {
      os << "  template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, " << DataType << ">>>\n";
      os << "  " << desc.name() << NameSuffix << "(T&& t)\n";
      os << "    : mData(std::forward<T>(t))\n";
      os << "  {}\n";
   }

   static void writeViewHasGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      os << "  bool has_" << field.name() << "() const\n";
      os << "  {\n";
      os << "     return std::visit([](auto&& data) -> bool { return pbview::impl::unwrap(data).has_" << field.name() << "(); }, mData);\n";
      os << "  }\n";
   }

   static void writeViewOptGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      if (field.is_repeated())
         return;

      auto fullNameSuffix = buildFullNameSuffix(field);
      os << "  std::optional<" << cppType(field, fullNameSuffix) << "> opt_" << field.name() << "() const\n";
      os << "  {\n";
      os << "     return std::visit([](auto&& data) -> std::optional<" << cppType(field, fullNameSuffix) << "> {\n";
      os << "       if constexpr(std::is_base_of_v<google::protobuf::Message, std::decay_t<decltype(pbview::impl::unwrap(data))>>)\n";
      os << "       {\n";
      os << "          if (pbview::impl::unwrap(data).has_" << field.name() << "())\n";
      os << "             return pbview::impl::unwrap(data)." << field.name() << "();\n";
      os << "          return {};\n";
      os << "       }\n";
      os << "       else\n";
      os << "          return pbview::impl::unwrap(data).opt_" << field.name() << "();\n";
      os << "     }, mData);\n";
      os << "  }\n";
   }

   static void writeViewGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      if (field.is_repeated())
         return;

      auto fullNameSuffix = buildFullNameSuffix(field);
      os << "  " << cppType(field, fullNameSuffix) << " " << field.name() << "() const\n";
      os << "  {\n";
      os << "     return std::visit([](auto&& data) -> " << cppType(field, fullNameSuffix) << "\n";
      os << "       {\n";
      os << "          return pbview::impl::unwrap(data)." << field.name() << "();\n";
      os << "       }, mData);\n";
      os << "  }\n";
   }

   static void writeViewSizeGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
   }

   static void writeViewIndexGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
   }

   static void writeLookupTemplate(std::ostream& os, const google::protobuf::FileDescriptor& fileDesc, const google::protobuf::Descriptor& desc)
   {
      os << "template <>\n";
      os << "struct VariantFor<" << packageToNamespace(fileDesc.package()) << "::" << desc.name() << ">\n";
      os << "{\n";
      os << "  template <typename... Args>\n";
      os << "  using Type = " << packageToNamespace(fileDesc.package()) << "::" << desc.name() << NameSuffix << "<Args...>;\n";
      os << "};\n";
   }
};

struct ViewImpl
{
   static void writeViewConstructors(std::ostream& os, const google::protobuf::Descriptor& desc)
   {
      os << "  using ViewType = BinView;\n";
      os << "\n";
      os << "  " << desc.name() << NameSuffix << "()\n";
      os << "    : mData(DataSpan{})\n";
      os << "  {}\n";
      os << "\n";
      os << "  explicit " << desc.name() << NameSuffix << "(DataSpan binMessage)\n";
      os << "    : mData(binMessage)\n";
      os << "  {}\n";
      os << "\n";
      os << "  static " << desc.name() << NameSuffix << " fromBytesString(std::string_view sv)\n";
      os << "  {\n";
      os << "     return " << desc.name() << NameSuffix << "{pbview::DataSpan{reinterpret_cast<const std::byte *>(sv.data()), sv.size()}};\n";
      os << "  }\n";
   }

   static void writeViewHasGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      os << "  bool has_" << field.name() << "() const\n";
      os << "  {\n";
      os << "     return mData.has(" << numberConstant(field) << ");\n";
      os << "  }\n";
   }

   static void writeViewOptGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      os << "  // more efficient than successive calls to has_" << field.name() << "() + " << field.name() << "()\n";
      os << "  std::optional<" << cppType(field, NameSuffixFull) << "> opt_" << field.name() << "() const\n";
      os << "  {\n";
      os << "     return mData.template get<" 
         << pbviewType(field, TypeFor::SingleValue) << ">(" << numberConstant(field) << ");\n";
      os << "  }\n";
   }

   static void writeViewGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      os << "  " << cppType(field, NameSuffixFull) << " " << field.name() << "() const\n";
      os << "  {\n";

      if (field.is_repeated())
      {
         const auto modifier = field.is_packed() ? "Packed"sv : ""sv;
         os << "     return mData.template get" << modifier << "Repeated<" 
            << pbviewType(field, TypeFor::RepeatedField) << ">(" << numberConstant(field) << ");\n";
      }
      else
      {
         os << "     if (auto val = opt_" << field.name() << "())\n";
         os << "       return *val;\n";
         if (field.message_type())
            os << "     return " << cppType(field, NameSuffixFull) << "{};\n";
         else
            os << "     return Message::default_instance()." << field.name() << "();\n";
      }

      os << "  }\n";
   }

   static void writeViewSizeGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      os << "  int " << field.name() << "_size() const\n";
      os << "  {\n";
      os << "     // Don't use this as the base of a for-loop!\n";
      os << "     // For best performance use a range-based-for-loop over " << field.name() << "()\n";
      os << "     return ranges::distance(" << field.name() << "());\n";
      os << "  }\n";
   }

   static void writeViewIndexGetter(std::ostream& os, const google::protobuf::FieldDescriptor& field)
   {
      os << "  " << cppType(field, NameSuffixFull) << " " << field.name() << "(int idx) const\n";
      os << "  {\n";
      os << "     // Don't use this as the base of a for-loop!\n";
      os << "     // For best performance use a range-based-for-loop over " << field.name() << "()\n";
      os << "     return ranges::front(" << field.name() << "() | ranges::view::drop_exactly(idx));\n";
      os << "  }\n";
   }

   static constexpr auto TemplateArgs = "typename BinView"sv;
   static constexpr auto NameSuffix = "ViewBase"sv;
   static constexpr auto NameSuffixFull = "ViewBase<BinView>"sv;
   static constexpr auto DataType = "BinView"sv;

   static void writeLookupTemplate(std::ostream& os, const google::protobuf::FileDescriptor& fileDesc, const google::protobuf::Descriptor& desc)
   {
      os << "template <" << TemplateArgs << ">\n";
      os << "struct ViewFor<" << packageToNamespace(fileDesc.package()) << "::" << desc.name() << ", " << DataType << ">\n";
      os << "{\n";
      os << "  using Type = " << packageToNamespace(fileDesc.package()) << "::" << desc.name() << NameSuffix << "<" << DataType << ">;\n";
      os << "};\n";
   }
};

template<typename T>
void writeMessage(std::ostream& os, const google::protobuf::FileDescriptor& fileDesc, const google::protobuf::Descriptor& desc)
{
   if (!fileDesc.package().empty())
   {
      os << "\n";
      os << "namespace " << packageToNamespace(fileDesc.package()) << '\n';
      os << "{\n";
   }
   os << "template <" << T::TemplateArgs << ">\n";
   os << "class " << desc.name() << T::NameSuffix << "\n";
   os << "{\n";
   os << "public:\n";
   os << "  using Message = " << desc.name() << ";\n";
   os << "\n";
   os << "private:\n";
   os << "  " << T::DataType << " mData;\n";
   os << "\n";
   os << "public:\n";
   os << "  using CppType = " << desc.name() << T::NameSuffix << ";\n";
   os << "  static constexpr auto serialization = pbview::Serialization::LengthDelimited;\n";
   os << "\n";
   T::writeViewConstructors(os, desc);

   for (int i=0; i < desc.field_count(); i++)
   {
      auto& field = *desc.field(i);
      os << "\n";
      os << "  static const int " << numberConstant(field) << " = " << field.number() << ";\n";
      if (field.is_repeated())
      {
         T::writeViewSizeGetter(os, field);
         T::writeViewIndexGetter(os, field);
      }
      else
      {
         T::writeViewHasGetter(os, field);
         T::writeViewOptGetter(os, field);
      }
      T::writeViewGetter(os, field);
   }
   os << "};\n";

   if (!fileDesc.package().empty())
      os << "}\n";
   os << "\n";
   os << "namespace pbview\n";
   os << "{\n";
   T::writeLookupTemplate(os, fileDesc, desc);
   os << "}\n";
}

constexpr auto viewHeaderHeader = R"(#pragma once

#include <string_view>
#include <range/v3/view/drop_exactly.hpp>
#include <range/v3/front.hpp>

#include <pbview/binmessageview.hpp>

)"sv;

void writeVarHeader(std::ostream& os, const google::protobuf::FileDescriptor& fileDesc)
{
   os << viewHeaderHeader;
   os << "#include <variant>\n";

   os << "#include \"" << replaceProtoExtension(fileDesc.name(), ".pbview.h") << "\"\n";
   os << '\n';

   for (int i=0; i < fileDesc.message_type_count(); i++)
      writeMessage<VarImpl>(os, fileDesc, *fileDesc.message_type(i));
}

void writeViewHeader(std::ostream& os, const google::protobuf::FileDescriptor& fileDesc)
{
   os << viewHeaderHeader;
   
   os << "#include \"" << replaceProtoExtension(fileDesc.name(), ".pb.h") << "\"\n";
   os << '\n';

   for (int i=0; i < fileDesc.message_type_count(); i++)
      writeMessage<ViewImpl>(os, fileDesc, *fileDesc.message_type(i));
}

std::string_view parentDir(std::string_view path)
{
#ifdef _WIN32
   auto pos = path.find_last_of("/\\");
#else
   auto pos = path.find_last_of("/");
#endif
   if (pos == std::string::npos)
      return "";
   return path.substr(0, pos);
}

std::string_view fileName(std::string_view path)
{
#ifdef _WIN32
   auto pos = path.find_last_of("/\\");
#else
   auto pos = path.find_last_of("/");
#endif
   if (pos == std::string::npos)
      return path;
   return path.substr(pos);
}

bool startsWith(std::string_view val, std::string_view part)
{
   if (val.size() < part.size())
      return false;

   return val.substr(0, part.size()) == part;
}

auto startsWith(std::string_view part)
{
   return [part](std::string_view in){ return startsWith(in, part); };
}

template <typename... Args>
auto startsWithOneOf(Args... parts)
{
   return [parts...](std::string_view in) {
      for (auto&& p : {parts...}) 
      {
         if(startsWith(in, p))
            return true;
      }
      return false; 
   };
}

template <typename Rng>
std::optional<std::string_view> optionalParameter(Rng&& params, std::string_view name)
{
   for (auto&& p : params)
   {
      if (startsWith(p, name))
         return p.substr(name.size());
   }

   return {};
}

template <typename Rng>
std::string_view requiredParameter(Rng&& params, std::string_view name)
{
   if (auto p = optionalParameter(params, name))
      return *p;

   throw std::runtime_error("The option '" + std::string{name} + "' is missing!");
} 

std::string_view paramValue(std::string_view param)
{
   if (startsWith(param, "--"))
   {
      auto pos = param.find('=');
      if (pos == std::string::npos)
         throw std::runtime_error{"Can't extract value from parameter '" + std::string{param} + "'"};
      return param.substr(pos+1);
   }
   else
   {
      if(param.size() < 2)
         throw std::runtime_error{"Can't extract value from parameter '" + std::string{param} + "'"};
      return param.substr(2);
   }
}

template<typename Rng>
void setupProtoPaths(google::protobuf::compiler::DiskSourceTree& sourceTree, Rng&& opts)
{
   auto protoPaths = opts //
                     | ranges::view::filter(startsWithOneOf("-I", "--proto_path="))
                     | ranges::view::transform(&paramValue);
   bool dirsFound = false;
   for (auto&& path : protoPaths)
   {
      sourceTree.MapPath("", std::string{path});
      dirsFound = true;
   }
   if (!dirsFound)
      sourceTree.MapPath("", ".");
}

int main(int argc, char* argsCStr[])
{
	try
	{
      auto isOption = [](std::string_view str){ return str[0] == '-'; };

      std::vector<std::string_view> args{argsCStr+1, argsCStr+argc};
      auto opts = args | ranges::view::take_while(isOption);
      auto files = args | ranges::view::drop_while(isOption);

      auto outDir = requiredParameter(opts, "--cpp_out=");
      if (outDir.back() == '/')
        outDir.remove_suffix(1);

	   auto sourceTree = std::make_unique<google::protobuf::compiler::DiskSourceTree>();
      setupProtoPaths(*sourceTree, opts);

	   auto errorCollector = std::make_unique<ExceptionErrorCollector>();
	   auto importer = std::make_unique<google::protobuf::compiler::Importer>(sourceTree.get(), errorCollector.get());

      for (auto&& file : files)
      {
    	   auto fileDesc = importer->pool()->FindFileByName(std::string{file});

         {         
            std::ofstream viewFile{std::string{outDir} + "/" + replaceProtoExtension(file, ".pbview.h")};
            writeViewHeader(viewFile, *fileDesc);
         }
         {
            std::ofstream varFile{std::string{outDir} + "/" + replaceProtoExtension(file, ".pbvar.h")};
            writeVarHeader(varFile, *fileDesc);            
         }
      }

	   return 0;
	}
	catch(std::exception& e)
	{
      std::cerr << R"(Usage: pbviewc [OPTION] PROTO_FILES
  -IPATH, --proto_path=PATH   Specify the directory in which to search for
                              imports.  May be specified multiple times;
                              directories will be searched in order.  If not
                              given, the current working directory is used.
  --cpp_out=OUT_DIR           Generate C++ header and source.
      )" << std::endl;
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}
