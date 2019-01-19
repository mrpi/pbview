#include <limits>

#include <test/samples-pb2.pbview.h>

#include <catch2/catch.hpp>

#include <range/v3/to_container.hpp>
#include <range/v3/view/zip.hpp>

using namespace std::literals;

TEST_CASE("GeneratedView on message with simple values")
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    using View = pbview::View<Msg>;

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
    auto view = View::fromBytesString(binStr);

    REQUIRE(allTypes.double_field()   == view.double_field());
    REQUIRE(allTypes.float_field()    == view.float_field());
    REQUIRE(allTypes.int32_field()    == view.int32_field());
    REQUIRE(allTypes.int64_field()    == view.int64_field());
    REQUIRE(allTypes.uint32_field()   == view.uint32_field());
    REQUIRE(allTypes.uint64_field()   == view.uint64_field());
    REQUIRE(allTypes.sint32_field()   == view.sint32_field());
    REQUIRE(allTypes.sint64_field()   == view.sint64_field());
    REQUIRE(allTypes.fixed32_field()  == view.fixed32_field());
    REQUIRE(allTypes.fixed64_field()  == view.fixed64_field());
    REQUIRE(allTypes.sfixed32_field() == view.sfixed32_field());
    REQUIRE(allTypes.sfixed64_field() == view.sfixed64_field());
    REQUIRE(allTypes.bool_field()     == view.bool_field());
    REQUIRE(allTypes.string_field()   == view.string_field());
    REQUIRE(allTypes.bytes_field()    == view.bytes_field());
    REQUIRE(allTypes.myenum_field()   == view.myenum_field());
    REQUIRE(allTypes.mysubmsg_field().id()    == view.mysubmsg_field().id());
    REQUIRE(allTypes.mysubmsg_field().value() == view.mysubmsg_field().value());
}

TEST_CASE("GeneratedView on message with minimal values")
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    using View = pbview::View<Msg>;

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
    auto view = View::fromBytesString(binStr);

    REQUIRE(allTypes.double_field()   == view.double_field());
    REQUIRE(allTypes.float_field()    == view.float_field());
    REQUIRE(allTypes.int32_field()    == view.int32_field());
    REQUIRE(allTypes.int64_field()    == view.int64_field());
    REQUIRE(allTypes.uint32_field()   == view.uint32_field());
    REQUIRE(allTypes.uint64_field()   == view.uint64_field());
    REQUIRE(allTypes.sint32_field()   == view.sint32_field());
    REQUIRE(allTypes.sint64_field()   == view.sint64_field());
    REQUIRE(allTypes.fixed32_field()  == view.fixed32_field());
    REQUIRE(allTypes.fixed64_field()  == view.fixed64_field());
    REQUIRE(allTypes.sfixed32_field() == view.sfixed32_field());
    REQUIRE(allTypes.sfixed64_field() == view.sfixed64_field());
    REQUIRE(allTypes.bool_field()     == view.bool_field());
    REQUIRE(allTypes.string_field()   == view.string_field());
    REQUIRE(allTypes.bytes_field()    == view.bytes_field());
    REQUIRE(allTypes.myenum_field()   == view.myenum_field());
    REQUIRE(allTypes.mysubmsg_field().id()    == view.mysubmsg_field().id());
    REQUIRE(allTypes.mysubmsg_field().value() == view.mysubmsg_field().value());
}

TEST_CASE("GeneratedView on message with maximal values")
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    using View = pbview::View<Msg>;

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
    auto view = View::fromBytesString(binStr);

    REQUIRE(allTypes.double_field()   == view.double_field());
    REQUIRE(allTypes.float_field()    == view.float_field());
    REQUIRE(allTypes.int32_field()    == view.int32_field());
    REQUIRE(allTypes.int64_field()    == view.int64_field());
    REQUIRE(allTypes.uint32_field()   == view.uint32_field());
    REQUIRE(allTypes.uint64_field()   == view.uint64_field());
    REQUIRE(allTypes.sint32_field()   == view.sint32_field());
    REQUIRE(allTypes.sint64_field()   == view.sint64_field());
    REQUIRE(allTypes.fixed32_field()  == view.fixed32_field());
    REQUIRE(allTypes.fixed64_field()  == view.fixed64_field());
    REQUIRE(allTypes.sfixed32_field() == view.sfixed32_field());
    REQUIRE(allTypes.sfixed64_field() == view.sfixed64_field());
    REQUIRE(allTypes.bool_field()     == view.bool_field());
    REQUIRE(allTypes.string_field()   == view.string_field());
    REQUIRE(allTypes.bytes_field()    == view.bytes_field());
    REQUIRE(allTypes.myenum_field()   == view.myenum_field());
    REQUIRE(allTypes.mysubmsg_field().id()    == view.mysubmsg_field().id());
    REQUIRE(allTypes.mysubmsg_field().value() == view.mysubmsg_field().value());
}

TEST_CASE("GeneratedView on message with no values")
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    using View = pbview::View<Msg>;

    auto binStr = allTypes.SerializeAsString();
    REQUIRE(binStr.size() == 0);
    auto view = View::fromBytesString(binStr);

    REQUIRE_FALSE(view.has_double_field());
    REQUIRE_FALSE(view.has_float_field());
    REQUIRE_FALSE(view.has_int32_field());
    REQUIRE_FALSE(view.has_int64_field());
    REQUIRE_FALSE(view.has_uint32_field());
    REQUIRE_FALSE(view.has_uint64_field());
    REQUIRE_FALSE(view.has_sint32_field());
    REQUIRE_FALSE(view.has_sint64_field());
    REQUIRE_FALSE(view.has_fixed32_field());
    REQUIRE_FALSE(view.has_fixed64_field());
    REQUIRE_FALSE(view.has_sfixed32_field());
    REQUIRE_FALSE(view.has_sfixed64_field());
    REQUIRE_FALSE(view.has_bool_field());
    REQUIRE_FALSE(view.has_string_field());
    REQUIRE_FALSE(view.has_bytes_field());
    REQUIRE_FALSE(view.has_myenum_field());
    REQUIRE_FALSE(view.has_mysubmsg_field());
}

namespace
{
static auto toString = ranges::view::transform([](std::string_view sv){ return std::string(sv.begin(), sv.end()); });
}

TEST_CASE("GeneratedView on message with repeated fields")
{
    using Msg = pbview::samples::AllTypesRepeated;
    Msg allTypes;
    using View = pbview::View<Msg>;

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
    auto view = View::fromBytesString(binStr);

    REQUIRE(ranges::to_vector(allTypes.double_field()) == 
            ranges::to_vector(view.double_field()));
    REQUIRE(ranges::to_vector(allTypes.float_field()) == 
            ranges::to_vector(view.float_field()));
    REQUIRE(ranges::to_vector(allTypes.int32_field()) == 
            ranges::to_vector(view.int32_field()));
    REQUIRE(ranges::to_vector(allTypes.int64_field()) == 
            ranges::to_vector(view.int64_field()));
    REQUIRE(ranges::to_vector(allTypes.uint32_field()) == 
            ranges::to_vector(view.uint32_field()));
    REQUIRE(ranges::to_vector(allTypes.uint64_field()) == 
            ranges::to_vector(view.uint64_field()));
    REQUIRE(ranges::to_vector(allTypes.sint32_field()) == 
            ranges::to_vector(view.sint32_field()));
    REQUIRE(ranges::to_vector(allTypes.sint64_field()) == 
            ranges::to_vector(view.sint64_field()));
    REQUIRE(ranges::to_vector(allTypes.fixed32_field()) == 
            ranges::to_vector(view.fixed32_field()));
    REQUIRE(ranges::to_vector(allTypes.fixed64_field()) == 
            ranges::to_vector(view.fixed64_field()));
    REQUIRE(ranges::to_vector(allTypes.sfixed32_field()) == 
            ranges::to_vector(view.sfixed32_field()));
    REQUIRE(ranges::to_vector(allTypes.sfixed64_field()) == 
            ranges::to_vector(view.sfixed64_field()));
    REQUIRE(ranges::to_vector(allTypes.bool_field()) == 
            ranges::to_vector(view.bool_field()));
    REQUIRE(ranges::to_vector(allTypes.string_field()) == 
            ranges::to_vector(view.string_field() | toString));
    REQUIRE(ranges::to_vector(allTypes.bytes_field()) == 
            ranges::to_vector(view.bytes_field() | toString));
    REQUIRE(ranges::to_vector(allTypes.myenum_field()) == 
            ranges::to_vector(view.myenum_field()));

    REQUIRE(allTypes.mysubmsg_field_size() == view.mysubmsg_field_size());
    for (auto&& [origSubMsg, viewSubMsg] : ranges::view::zip(allTypes.mysubmsg_field(), view.mysubmsg_field()))
    {   
       REQUIRE(origSubMsg.id() == viewSubMsg.id());
       REQUIRE(origSubMsg.value() == viewSubMsg.value());
    }
}

TEST_CASE("GeneratedView on message with packed repeated fields")
{
    using Msg = pbview::samples::AllTypesRepeatedPacked;
    Msg allTypes;
    using View = pbview::View<Msg>;

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
    auto view = View::fromBytesString(binStr);

    REQUIRE(ranges::to_vector(allTypes.double_field()) == 
            ranges::to_vector(view.double_field()));
    REQUIRE(ranges::to_vector(allTypes.float_field()) == 
            ranges::to_vector(view.float_field()));
    REQUIRE(ranges::to_vector(allTypes.int32_field()) == 
            ranges::to_vector(view.int32_field()));
    REQUIRE(ranges::to_vector(allTypes.int64_field()) == 
            ranges::to_vector(view.int64_field()));
    REQUIRE(ranges::to_vector(allTypes.uint32_field()) == 
            ranges::to_vector(view.uint32_field()));
    REQUIRE(ranges::to_vector(allTypes.uint64_field()) == 
            ranges::to_vector(view.uint64_field()));
    REQUIRE(ranges::to_vector(allTypes.sint32_field()) == 
            ranges::to_vector(view.sint32_field()));
    REQUIRE(ranges::to_vector(allTypes.sint64_field()) == 
            ranges::to_vector(view.sint64_field()));
    REQUIRE(ranges::to_vector(allTypes.fixed32_field()) == 
            ranges::to_vector(view.fixed32_field()));
    REQUIRE(ranges::to_vector(allTypes.fixed64_field()) == 
            ranges::to_vector(view.fixed64_field()));
    REQUIRE(ranges::to_vector(allTypes.sfixed32_field()) == 
            ranges::to_vector(view.sfixed32_field()));
    REQUIRE(ranges::to_vector(allTypes.sfixed64_field()) == 
            ranges::to_vector(view.sfixed64_field()));
    REQUIRE(ranges::to_vector(allTypes.bool_field()) == 
            ranges::to_vector(view.bool_field()));
    REQUIRE(ranges::to_vector(allTypes.myenum_field()) == 
            ranges::to_vector(view.myenum_field()));
}
