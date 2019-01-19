#include <catch2/catch.hpp>
#include <optional>

#include <range/v3/to_container.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/indices.hpp>

namespace Catch {
    template<typename T>
    struct StringMaker<std::optional<T>> {
        static std::string convert(const std::optional<T>& value) {
            if (value)
               return StringMaker<T>::convert(*value);
            return "std::nullopt";
        }
    };
}

#include <test/samples-pb2.pb.h>

#include <pbview/binmessageview.hpp>

using namespace std::literals;

TEST_CASE("BinMessageView on message with simple values")
{
    pbview::samples::AllTypes allTypes;

    allTypes.set_double_field(3.1415926); 
    allTypes.set_float_field(3.14);
    allTypes.set_int32_field(142);
    allTypes.set_int64_field(242);
    allTypes.set_uint32_field(342);
    allTypes.set_uint64_field(442);
    allTypes.set_sint32_field(542);
    allTypes.set_sint64_field(642);
    allTypes.set_fixed32_field(742);
    allTypes.set_fixed64_field(842);
    allTypes.set_sfixed32_field(942);
    allTypes.set_sfixed64_field(1042);
    allTypes.set_bool_field(true);
    allTypes.set_string_field("Lorem ipsum");
    allTypes.set_bytes_field("Lorem\0ipsum"s);
 
    allTypes.set_myenum_field(pbview::samples::MyEnumVal2);
 
    allTypes.mutable_mysubmsg_field()->set_id(314); 
    allTypes.mutable_mysubmsg_field()->set_value("asdf");

    auto binStr = allTypes.SerializeAsString();
    auto msg = pbview::BinMessageView<>::fromBytesString(binStr);

    REQUIRE(allTypes.double_field() == msg.get<pbview::type::Double>(pbview::samples::AllTypes::kDoubleFieldFieldNumber));
    REQUIRE(allTypes.float_field() == msg.get<pbview::type::Float>(pbview::samples::AllTypes::kFloatFieldFieldNumber));
    REQUIRE(allTypes.int32_field() == msg.get<pbview::type::Int32>(pbview::samples::AllTypes::kInt32FieldFieldNumber));
    REQUIRE(allTypes.int64_field() == msg.get<pbview::type::Int64>(pbview::samples::AllTypes::kInt64FieldFieldNumber));
    REQUIRE(allTypes.uint32_field() == msg.get<pbview::type::Uint32>(pbview::samples::AllTypes::kUint32FieldFieldNumber));
    REQUIRE(allTypes.uint64_field() == msg.get<pbview::type::Uint64>(pbview::samples::AllTypes::kUint64FieldFieldNumber));
    REQUIRE(allTypes.sint32_field() == msg.get<pbview::type::Sint32>(pbview::samples::AllTypes::kSint32FieldFieldNumber));
    REQUIRE(allTypes.sint64_field() == msg.get<pbview::type::Sint64>(pbview::samples::AllTypes::kSint64FieldFieldNumber));
    REQUIRE(allTypes.fixed32_field() == msg.get<pbview::type::Fixed32>(pbview::samples::AllTypes::kFixed32FieldFieldNumber));
    REQUIRE(allTypes.fixed64_field() == msg.get<pbview::type::Fixed64>(pbview::samples::AllTypes::kFixed64FieldFieldNumber));
    REQUIRE(allTypes.sfixed32_field() == msg.get<pbview::type::Sfixed32>(pbview::samples::AllTypes::kSfixed32FieldFieldNumber));
    REQUIRE(allTypes.sfixed64_field() == msg.get<pbview::type::Sfixed64>(pbview::samples::AllTypes::kSfixed64FieldFieldNumber));
    REQUIRE(allTypes.bool_field() == msg.get<pbview::type::Bool>(pbview::samples::AllTypes::kBoolFieldFieldNumber));
    REQUIRE(allTypes.string_field() == std::string{*msg.get<pbview::type::String>(pbview::samples::AllTypes::kStringFieldFieldNumber)});
    REQUIRE(allTypes.bytes_field() == std::string{*msg.get<pbview::type::Bytes>(pbview::samples::AllTypes::kBytesFieldFieldNumber)});

    REQUIRE(allTypes.myenum_field() == msg.get<pbview::type::Int32>(pbview::samples::AllTypes::kMyenumFieldFieldNumber));

    auto subMsg = msg.get<pbview::type::Message>(pbview::samples::AllTypes::kMysubmsgFieldFieldNumber);
    REQUIRE(subMsg);
    REQUIRE(allTypes.mysubmsg_field().id() == subMsg->get<pbview::type::Int32>(pbview::samples::MySubMsg::kIdFieldNumber));
    REQUIRE(allTypes.mysubmsg_field().value() == subMsg->get<pbview::type::String>(pbview::samples::MySubMsg::kValueFieldNumber));
}

TEST_CASE("BinMessageView on message with minimal values")
{
    pbview::samples::AllTypes allTypes;

    allTypes.set_double_field(std::numeric_limits<double>::min()); 
    allTypes.set_float_field(std::numeric_limits<float>::min());
    allTypes.set_int32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.set_int64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.set_uint32_field(std::numeric_limits<std::uint32_t>::min());
    allTypes.set_uint64_field(std::numeric_limits<std::uint64_t>::min());
    allTypes.set_sint32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.set_sint64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.set_fixed32_field(std::numeric_limits<std::uint32_t>::min());
    allTypes.set_fixed64_field(std::numeric_limits<std::uint64_t>::min());
    allTypes.set_sfixed32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.set_sfixed64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.set_bool_field(false);
    allTypes.set_string_field("");
    allTypes.set_bytes_field(""s);
 
    allTypes.set_myenum_field(pbview::samples::MyEnumVal1);
 
    allTypes.mutable_mysubmsg_field()->set_id(std::numeric_limits<std::int32_t>::min()); 
    allTypes.mutable_mysubmsg_field()->set_value("");

    auto binStr = allTypes.SerializeAsString();
    auto msg = pbview::BinMessageView<>::fromBytesString(binStr);

    REQUIRE(allTypes.double_field() == msg.get<pbview::type::Double>(pbview::samples::AllTypes::kDoubleFieldFieldNumber));
    REQUIRE(allTypes.float_field() == msg.get<pbview::type::Float>(pbview::samples::AllTypes::kFloatFieldFieldNumber));
    REQUIRE(allTypes.int32_field() == msg.get<pbview::type::Int32>(pbview::samples::AllTypes::kInt32FieldFieldNumber));
    REQUIRE(allTypes.int64_field() == msg.get<pbview::type::Int64>(pbview::samples::AllTypes::kInt64FieldFieldNumber));
    REQUIRE(allTypes.uint32_field() == msg.get<pbview::type::Uint32>(pbview::samples::AllTypes::kUint32FieldFieldNumber));
    REQUIRE(allTypes.uint64_field() == msg.get<pbview::type::Uint64>(pbview::samples::AllTypes::kUint64FieldFieldNumber));
    REQUIRE(allTypes.sint32_field() == msg.get<pbview::type::Sint32>(pbview::samples::AllTypes::kSint32FieldFieldNumber));
    REQUIRE(allTypes.sint64_field() == msg.get<pbview::type::Sint64>(pbview::samples::AllTypes::kSint64FieldFieldNumber));
    REQUIRE(allTypes.fixed32_field() == msg.get<pbview::type::Fixed32>(pbview::samples::AllTypes::kFixed32FieldFieldNumber));
    REQUIRE(allTypes.fixed64_field() == msg.get<pbview::type::Fixed64>(pbview::samples::AllTypes::kFixed64FieldFieldNumber));
    REQUIRE(allTypes.sfixed32_field() == msg.get<pbview::type::Sfixed32>(pbview::samples::AllTypes::kSfixed32FieldFieldNumber));
    REQUIRE(allTypes.sfixed64_field() == msg.get<pbview::type::Sfixed64>(pbview::samples::AllTypes::kSfixed64FieldFieldNumber));
    REQUIRE(allTypes.bool_field() == msg.get<pbview::type::Bool>(pbview::samples::AllTypes::kBoolFieldFieldNumber));
    REQUIRE(allTypes.string_field() == std::string{*msg.get<pbview::type::String>(pbview::samples::AllTypes::kStringFieldFieldNumber)});
    REQUIRE(allTypes.bytes_field() == std::string{*msg.get<pbview::type::Bytes>(pbview::samples::AllTypes::kBytesFieldFieldNumber)});

    REQUIRE(allTypes.myenum_field() == msg.get<pbview::type::Int32>(pbview::samples::AllTypes::kMyenumFieldFieldNumber));

    auto subMsg = msg.get<pbview::type::Message>(pbview::samples::AllTypes::kMysubmsgFieldFieldNumber);
    REQUIRE(subMsg);
    REQUIRE(allTypes.mysubmsg_field().id() == subMsg->get<pbview::type::Int32>(pbview::samples::MySubMsg::kIdFieldNumber));
    REQUIRE(allTypes.mysubmsg_field().value() == subMsg->get<pbview::type::String>(pbview::samples::MySubMsg::kValueFieldNumber));
}

TEST_CASE("BinMessageView on message with maximal values")
{
    pbview::samples::AllTypes allTypes;

    allTypes.set_double_field(std::numeric_limits<double>::max()); 
    allTypes.set_float_field(std::numeric_limits<float>::max());
    allTypes.set_int32_field(std::numeric_limits<std::int32_t>::max());
    allTypes.set_int64_field(std::numeric_limits<std::int64_t>::max());
    allTypes.set_uint32_field(std::numeric_limits<std::uint32_t>::max());
    allTypes.set_uint64_field(std::numeric_limits<std::uint64_t>::max());
    allTypes.set_sint32_field(std::numeric_limits<std::int32_t>::max());
    allTypes.set_sint64_field(std::numeric_limits<std::int64_t>::max());
    allTypes.set_fixed32_field(std::numeric_limits<std::uint32_t>::max());
    allTypes.set_fixed64_field(std::numeric_limits<std::uint64_t>::max());
    allTypes.set_sfixed32_field(std::numeric_limits<std::int32_t>::max());
    allTypes.set_sfixed64_field(std::numeric_limits<std::int64_t>::max());
    allTypes.set_bool_field(true);
    allTypes.set_string_field(std::string(64*1024, 'X'));
    allTypes.set_bytes_field(std::string(64*1024, '\0'));
 
    allTypes.set_myenum_field(pbview::samples::MyEnumVal3);
 
    allTypes.mutable_mysubmsg_field()->set_id(std::numeric_limits<std::int32_t>::max()); 
    allTypes.mutable_mysubmsg_field()->set_value(std::string(64*1024, ' '));

    auto binStr = allTypes.SerializeAsString();
    auto msg = pbview::BinMessageView<>::fromBytesString(binStr);

    REQUIRE(allTypes.double_field()   == msg.get<pbview::type::Double>(pbview::samples::AllTypes::kDoubleFieldFieldNumber));
    REQUIRE(allTypes.float_field()    == msg.get<pbview::type::Float>(pbview::samples::AllTypes::kFloatFieldFieldNumber));
    REQUIRE(allTypes.int32_field()    == msg.get<pbview::type::Int32>(pbview::samples::AllTypes::kInt32FieldFieldNumber));
    REQUIRE(allTypes.int64_field()    == msg.get<pbview::type::Int64>(pbview::samples::AllTypes::kInt64FieldFieldNumber));
    REQUIRE(allTypes.uint32_field()   == msg.get<pbview::type::Uint32>(pbview::samples::AllTypes::kUint32FieldFieldNumber));
    REQUIRE(allTypes.uint64_field()   == msg.get<pbview::type::Uint64>(pbview::samples::AllTypes::kUint64FieldFieldNumber));
    REQUIRE(allTypes.sint32_field()   == msg.get<pbview::type::Sint32>(pbview::samples::AllTypes::kSint32FieldFieldNumber));
    REQUIRE(allTypes.sint64_field()   == msg.get<pbview::type::Sint64>(pbview::samples::AllTypes::kSint64FieldFieldNumber));
    REQUIRE(allTypes.fixed32_field()  == msg.get<pbview::type::Fixed32>(pbview::samples::AllTypes::kFixed32FieldFieldNumber));
    REQUIRE(allTypes.fixed64_field()  == msg.get<pbview::type::Fixed64>(pbview::samples::AllTypes::kFixed64FieldFieldNumber));
    REQUIRE(allTypes.sfixed32_field() == msg.get<pbview::type::Sfixed32>(pbview::samples::AllTypes::kSfixed32FieldFieldNumber));
    REQUIRE(allTypes.sfixed64_field() == msg.get<pbview::type::Sfixed64>(pbview::samples::AllTypes::kSfixed64FieldFieldNumber));
    REQUIRE(allTypes.bool_field()     == msg.get<pbview::type::Bool>(pbview::samples::AllTypes::kBoolFieldFieldNumber));
    REQUIRE(allTypes.string_field()   == std::string{*msg.get<pbview::type::String>(pbview::samples::AllTypes::kStringFieldFieldNumber)});
    REQUIRE(allTypes.bytes_field()    == std::string{*msg.get<pbview::type::Bytes>(pbview::samples::AllTypes::kBytesFieldFieldNumber)});

    REQUIRE(allTypes.myenum_field()   == msg.get<pbview::type::Enum<pbview::samples::MyEnum>>(pbview::samples::AllTypes::kMyenumFieldFieldNumber));

    auto subMsg = msg.get<pbview::type::Message>(pbview::samples::AllTypes::kMysubmsgFieldFieldNumber);
    REQUIRE(subMsg);
    REQUIRE(allTypes.mysubmsg_field().id() == subMsg->get<pbview::type::Int32>(pbview::samples::MySubMsg::kIdFieldNumber));
    REQUIRE(allTypes.mysubmsg_field().value() == subMsg->get<pbview::type::String>(pbview::samples::MySubMsg::kValueFieldNumber));
}

TEST_CASE("BinMessageView on message with no values")
{
    pbview::samples::AllTypes allTypes;

    auto binStr = allTypes.SerializeAsString();
    REQUIRE(binStr.size() == 0);
    auto msg = pbview::BinMessageView<>::fromBytesString(binStr);

    REQUIRE_FALSE(msg.get<pbview::type::Double>(pbview::samples::AllTypes::kDoubleFieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Float>(pbview::samples::AllTypes::kFloatFieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Int32>(pbview::samples::AllTypes::kInt32FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Int64>(pbview::samples::AllTypes::kInt64FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Uint32>(pbview::samples::AllTypes::kUint32FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Uint64>(pbview::samples::AllTypes::kUint64FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Sint32>(pbview::samples::AllTypes::kSint32FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Sint64>(pbview::samples::AllTypes::kSint64FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Fixed32>(pbview::samples::AllTypes::kFixed32FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Fixed64>(pbview::samples::AllTypes::kFixed64FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Sfixed32>(pbview::samples::AllTypes::kSfixed32FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Sfixed64>(pbview::samples::AllTypes::kSfixed64FieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Bool>(pbview::samples::AllTypes::kBoolFieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::String>(pbview::samples::AllTypes::kStringFieldFieldNumber));
    REQUIRE_FALSE(msg.get<pbview::type::Bytes>(pbview::samples::AllTypes::kBytesFieldFieldNumber));

    REQUIRE_FALSE(msg.get<pbview::type::Enum<pbview::samples::MyEnum>>(pbview::samples::AllTypes::kMyenumFieldFieldNumber));

    REQUIRE_FALSE(msg.get<pbview::type::Message>(pbview::samples::AllTypes::kMysubmsgFieldFieldNumber));
}

namespace
{
static auto toString = ranges::view::transform([](std::string_view sv){ return std::string(sv.begin(), sv.end()); });
}

TEST_CASE("BinMessageView on message with repeated fields")
{
    pbview::samples::AllTypesRepeated allTypes;

    allTypes.add_double_field(3.1415926);
    allTypes.add_double_field(std::numeric_limits<double>::min());
    allTypes.add_double_field(std::numeric_limits<double>::max());

    allTypes.add_float_field(3.14);
    allTypes.add_float_field(std::numeric_limits<float>::min());
    allTypes.add_float_field(std::numeric_limits<float>::max());

    allTypes.add_int32_field(142);
    allTypes.add_int32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.add_int32_field(std::numeric_limits<std::int32_t>::max());

    allTypes.add_int64_field(242);
    allTypes.add_int64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.add_int64_field(std::numeric_limits<std::int64_t>::max());

    allTypes.add_uint32_field(342);
    allTypes.add_uint32_field(std::numeric_limits<std::uint32_t>::min());
    allTypes.add_uint32_field(std::numeric_limits<std::uint32_t>::max());

    allTypes.add_uint64_field(442);
    allTypes.add_uint64_field(std::numeric_limits<std::uint64_t>::min());
    allTypes.add_uint64_field(std::numeric_limits<std::uint64_t>::max());

    allTypes.add_sint32_field(542);
    allTypes.add_sint32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.add_sint32_field(std::numeric_limits<std::int32_t>::max());

    allTypes.add_sint64_field(642);
    allTypes.add_sint64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.add_sint64_field(std::numeric_limits<std::int64_t>::max());

    allTypes.add_fixed32_field(742);
    allTypes.add_fixed32_field(std::numeric_limits<std::uint32_t>::min());
    allTypes.add_fixed32_field(std::numeric_limits<std::uint32_t>::max());

    allTypes.add_fixed64_field(842);
    allTypes.add_fixed64_field(std::numeric_limits<std::uint64_t>::min());
    allTypes.add_fixed64_field(std::numeric_limits<std::uint64_t>::max());

    allTypes.add_sfixed32_field(942);
    allTypes.add_sfixed32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.add_sfixed32_field(std::numeric_limits<std::int32_t>::max());

    allTypes.add_sfixed64_field(1042);
    allTypes.add_sfixed64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.add_sfixed64_field(std::numeric_limits<std::int64_t>::max());

    allTypes.add_bool_field(true);
    allTypes.add_bool_field(false);
    allTypes.add_bool_field(true);

    allTypes.add_string_field("Lorem ipsum");
    allTypes.add_string_field("");
    allTypes.add_string_field(std::string(64*1024, 'X'));

    allTypes.add_bytes_field("Lorem\0ipsum"s);
    allTypes.add_bytes_field("");
    allTypes.add_bytes_field(std::string(64*1024, '\0'));
 
    allTypes.add_myenum_field(pbview::samples::MyEnumVal2);
    allTypes.add_myenum_field(pbview::samples::MyEnumVal1);
    allTypes.add_myenum_field(pbview::samples::MyEnumVal3);
 
    {
        auto subMsg0 = allTypes.add_mysubmsg_field();
        subMsg0->set_id(314); 
        subMsg0->set_value("asdf");

        auto subMsg1 = allTypes.add_mysubmsg_field();
        subMsg1->set_id(std::numeric_limits<std::int32_t>::min()); 
        subMsg1->set_value("");

        auto subMsg2 = allTypes.add_mysubmsg_field();
        subMsg2->set_id(std::numeric_limits<std::int32_t>::max()); 
        subMsg2->set_value(std::string(64*1024, ' '));
    }

    auto binStr = allTypes.SerializeAsString();
    auto msg = pbview::BinMessageView<>::fromBytesString(binStr);

    {
       auto dblRng = msg.getRepeated<pbview::type::Double>(pbview::samples::AllTypesRepeated::kDoubleFieldFieldNumber);
       static_assert(ranges::ForwardRange<decltype(dblRng)>());
    }

    REQUIRE(ranges::to_vector(allTypes.double_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Double>(pbview::samples::AllTypesRepeated::kDoubleFieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.float_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Float>(pbview::samples::AllTypesRepeated::kFloatFieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.int32_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Int32>(pbview::samples::AllTypesRepeated::kInt32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.int64_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Int64>(pbview::samples::AllTypesRepeated::kInt64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.uint32_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Uint32>(pbview::samples::AllTypesRepeated::kUint32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.uint64_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Uint64>(pbview::samples::AllTypesRepeated::kUint64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sint32_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Sint32>(pbview::samples::AllTypesRepeated::kSint32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sint64_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Sint64>(pbview::samples::AllTypesRepeated::kSint64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.fixed32_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Fixed32>(pbview::samples::AllTypes::kFixed32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.fixed64_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Fixed64>(pbview::samples::AllTypes::kFixed64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sfixed32_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Sfixed32>(pbview::samples::AllTypes::kSfixed32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sfixed64_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Sfixed64>(pbview::samples::AllTypes::kSfixed64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.bool_field()) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Bool>(pbview::samples::AllTypesRepeated::kBoolFieldFieldNumber)));

    {
        auto strRng = msg.getRepeated<pbview::type::String>(pbview::samples::AllTypes::kStringFieldFieldNumber);
        REQUIRE(ranges::to_vector(allTypes.string_field()) == 
                ranges::to_vector(strRng | toString));
    }

    {
        auto byteRng = msg.getRepeated<pbview::type::Bytes>(pbview::samples::AllTypes::kBytesFieldFieldNumber);
        REQUIRE(ranges::to_vector(allTypes.bytes_field()) == 
                ranges::to_vector(byteRng | toString));
    }

    REQUIRE(ranges::to_vector(allTypes.myenum_field() | ranges::view::transform([](int i){ return static_cast<pbview::samples::MyEnum>(i); })) == 
            ranges::to_vector(msg.getRepeated<pbview::type::Enum<pbview::samples::MyEnum>>(pbview::samples::AllTypesRepeated::kMyenumFieldFieldNumber)));

    auto subMsgVec = msg.getRepeated<pbview::type::Message>(pbview::samples::AllTypes::kMysubmsgFieldFieldNumber)
        | ranges::view::transform([](auto&& subMsgView){ return pbview::deserialize<pbview::samples::MySubMsg>(subMsgView); })
        | ranges::to_vector;
    
    REQUIRE(allTypes.mysubmsg_field_size() == subMsgVec.size());
    for (auto&& [origSubMsg, parsedSubMsg] : ranges::view::zip(allTypes.mysubmsg_field(), subMsgVec))
    {
       REQUIRE(origSubMsg.id() == parsedSubMsg.id());
       REQUIRE(origSubMsg.value() == parsedSubMsg.value());
    }
}

TEST_CASE("BinMessageView on message with packed repeated fields")
{
    pbview::samples::AllTypesRepeatedPacked allTypes;

    allTypes.add_double_field(3.1415926);
    allTypes.add_double_field(std::numeric_limits<double>::min());
    allTypes.add_double_field(std::numeric_limits<double>::max());

    allTypes.add_float_field(3.14);
    allTypes.add_float_field(std::numeric_limits<float>::min());
    allTypes.add_float_field(std::numeric_limits<float>::max());

    allTypes.add_int32_field(142);
    allTypes.add_int32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.add_int32_field(std::numeric_limits<std::int32_t>::max());

    allTypes.add_int64_field(242);
    allTypes.add_int64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.add_int64_field(std::numeric_limits<std::int64_t>::max());

    allTypes.add_uint32_field(342);
    allTypes.add_uint32_field(std::numeric_limits<std::uint32_t>::min());
    allTypes.add_uint32_field(std::numeric_limits<std::uint32_t>::max());

    allTypes.add_uint64_field(442);
    allTypes.add_uint64_field(std::numeric_limits<std::uint64_t>::min());
    allTypes.add_uint64_field(std::numeric_limits<std::uint64_t>::max());

    allTypes.add_sint32_field(542);
    allTypes.add_sint32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.add_sint32_field(std::numeric_limits<std::int32_t>::max());

    allTypes.add_sint64_field(642);
    allTypes.add_sint64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.add_sint64_field(std::numeric_limits<std::int64_t>::max());

    allTypes.add_fixed32_field(742);
    allTypes.add_fixed32_field(std::numeric_limits<std::uint32_t>::min());
    allTypes.add_fixed32_field(std::numeric_limits<std::uint32_t>::max());

    allTypes.add_fixed64_field(842);
    allTypes.add_fixed64_field(std::numeric_limits<std::uint64_t>::min());
    allTypes.add_fixed64_field(std::numeric_limits<std::uint64_t>::max());

    allTypes.add_sfixed32_field(942);
    allTypes.add_sfixed32_field(std::numeric_limits<std::int32_t>::min());
    allTypes.add_sfixed32_field(std::numeric_limits<std::int32_t>::max());

    allTypes.add_sfixed64_field(1042);
    allTypes.add_sfixed64_field(std::numeric_limits<std::int64_t>::min());
    allTypes.add_sfixed64_field(std::numeric_limits<std::int64_t>::max());

    allTypes.add_bool_field(true);
    allTypes.add_bool_field(false);
    allTypes.add_bool_field(true);
 
    allTypes.add_myenum_field(pbview::samples::MyEnumVal2);
    allTypes.add_myenum_field(pbview::samples::MyEnumVal1);
    allTypes.add_myenum_field(pbview::samples::MyEnumVal3);

    auto binStr = allTypes.SerializeAsString();
    auto msg = pbview::BinMessageView<>::fromBytesString(binStr);

    {
       auto dblRng = msg.getPackedRepeated<pbview::type::Double>(pbview::samples::AllTypesRepeated::kDoubleFieldFieldNumber);
       static_assert(ranges::ForwardRange<decltype(dblRng)>());
    }

    REQUIRE(ranges::to_vector(allTypes.double_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Double>(pbview::samples::AllTypesRepeated::kDoubleFieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.float_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Float>(pbview::samples::AllTypesRepeated::kFloatFieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.int32_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Int32>(pbview::samples::AllTypesRepeated::kInt32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.int64_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Int64>(pbview::samples::AllTypesRepeated::kInt64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.uint32_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Uint32>(pbview::samples::AllTypesRepeated::kUint32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.uint64_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Uint64>(pbview::samples::AllTypesRepeated::kUint64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sint32_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Sint32>(pbview::samples::AllTypesRepeated::kSint32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sint64_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Sint64>(pbview::samples::AllTypesRepeated::kSint64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.fixed32_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Fixed32>(pbview::samples::AllTypes::kFixed32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.fixed64_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Fixed64>(pbview::samples::AllTypes::kFixed64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sfixed32_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Sfixed32>(pbview::samples::AllTypes::kSfixed32FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.sfixed64_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Sfixed64>(pbview::samples::AllTypes::kSfixed64FieldFieldNumber)));
    REQUIRE(ranges::to_vector(allTypes.bool_field()) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Bool>(pbview::samples::AllTypesRepeated::kBoolFieldFieldNumber)));

    REQUIRE(ranges::to_vector(allTypes.myenum_field() | ranges::view::transform([](int i){ return static_cast<pbview::samples::MyEnum>(i); })) == 
            ranges::to_vector(msg.getPackedRepeated<pbview::type::Enum<pbview::samples::MyEnum>>(pbview::samples::AllTypesRepeated::kMyenumFieldFieldNumber)));
}
