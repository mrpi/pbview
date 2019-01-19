#include <limits>

#include <test/samples-pb2.pbvar.h>

#include <catch2/catch.hpp>

#include <range/v3/to_container.hpp>
#include <range/v3/view/zip.hpp>

using namespace std::literals;

TEST_CASE("GeneratedVar on message with simple values")
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    using View = pbview::View<Msg>;
    using ViewOrValue = pbview::ViewOrValue<Msg>;
    using ViewOrRef = pbview::ViewOrRef<Msg>;

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

    auto runChecksOn = [&allTypes](const auto& viewOrVal){
        REQUIRE(allTypes.double_field()   == viewOrVal.double_field());
        REQUIRE(allTypes.float_field()    == viewOrVal.float_field());
        REQUIRE(allTypes.int32_field()    == viewOrVal.int32_field());
        REQUIRE(allTypes.int64_field()    == viewOrVal.int64_field());
        REQUIRE(allTypes.uint32_field()   == viewOrVal.uint32_field());
        REQUIRE(allTypes.uint64_field()   == viewOrVal.uint64_field());
        REQUIRE(allTypes.sint32_field()   == viewOrVal.sint32_field());
        REQUIRE(allTypes.sint64_field()   == viewOrVal.sint64_field());
        REQUIRE(allTypes.fixed32_field()  == viewOrVal.fixed32_field());
        REQUIRE(allTypes.fixed64_field()  == viewOrVal.fixed64_field());
        REQUIRE(allTypes.sfixed32_field() == viewOrVal.sfixed32_field());
        REQUIRE(allTypes.sfixed64_field() == viewOrVal.sfixed64_field());
        REQUIRE(allTypes.bool_field()     == viewOrVal.bool_field());
        REQUIRE(allTypes.string_field()   == viewOrVal.string_field());
        REQUIRE(allTypes.bytes_field()    == viewOrVal.bytes_field());
        REQUIRE(allTypes.myenum_field()   == viewOrVal.myenum_field());
        REQUIRE(allTypes.mysubmsg_field().id()    == viewOrVal.mysubmsg_field().id());
        REQUIRE(allTypes.mysubmsg_field().value() == viewOrVal.mysubmsg_field().value());
    };

    SECTION("ViewOrValue containing view")
    {
        runChecksOn(ViewOrValue{view});
    }

    SECTION("ViewOrValue containing value")
    {
        runChecksOn(ViewOrValue{allTypes});
    }

    SECTION("ViewOrRef containing view")
    {
        runChecksOn(ViewOrRef{view});
    }

    SECTION("ViewOrRef containing value")
    {
        runChecksOn(ViewOrRef{allTypes});
    }
}

TEST_CASE("GeneratedVar: read optional value on message with simple values")
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    using View = pbview::View<Msg>;
    using ViewOrValue = pbview::ViewOrValue<Msg>;
    using ViewOrRef = pbview::ViewOrRef<Msg>;

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

    auto runChecksOn = [&allTypes](const auto& viewOrVal){
        REQUIRE(allTypes.double_field()   == viewOrVal.opt_double_field());
        REQUIRE(allTypes.float_field()    == viewOrVal.opt_float_field());
        REQUIRE(allTypes.int32_field()    == viewOrVal.opt_int32_field());
        REQUIRE(allTypes.int64_field()    == viewOrVal.opt_int64_field());
        REQUIRE(allTypes.uint32_field()   == viewOrVal.opt_uint32_field());
        REQUIRE(allTypes.uint64_field()   == viewOrVal.opt_uint64_field());
        REQUIRE(allTypes.sint32_field()   == viewOrVal.opt_sint32_field());
        REQUIRE(allTypes.sint64_field()   == viewOrVal.opt_sint64_field());
        REQUIRE(allTypes.fixed32_field()  == viewOrVal.opt_fixed32_field());
        REQUIRE(allTypes.fixed64_field()  == viewOrVal.opt_fixed64_field());
        REQUIRE(allTypes.sfixed32_field() == viewOrVal.opt_sfixed32_field());
        REQUIRE(allTypes.sfixed64_field() == viewOrVal.opt_sfixed64_field());
        REQUIRE(allTypes.bool_field()     == viewOrVal.opt_bool_field());
        REQUIRE(allTypes.string_field()   == viewOrVal.opt_string_field());
        REQUIRE(allTypes.bytes_field()    == viewOrVal.opt_bytes_field());
        REQUIRE(allTypes.myenum_field()   == viewOrVal.opt_myenum_field());
        REQUIRE(allTypes.mysubmsg_field().id()    == viewOrVal.opt_mysubmsg_field()->id());
        REQUIRE(allTypes.mysubmsg_field().value() == viewOrVal.opt_mysubmsg_field()->value());
    };

    SECTION("ViewOrValue containing view")
    {
        runChecksOn(ViewOrValue{view});
    }

    SECTION("ViewOrValue containing value")
    {
        runChecksOn(ViewOrValue{allTypes});
    }

    SECTION("ViewOrRef containing view")
    {
        runChecksOn(ViewOrRef{view});
    }

    SECTION("ViewOrRef containing value")
    {
        ViewOrRef var{allTypes};
        runChecksOn(var);

        auto subVar = var.mysubmsg_field();
        allTypes.mutable_mysubmsg_field()->set_value("new value");
        REQUIRE(subVar.value() == "new value");
    }
}

TEST_CASE("GeneratedVar: read optional value on message with no values")
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    using View = pbview::View<Msg>;
    using ViewOrValue = pbview::ViewOrValue<Msg>;
    using ViewOrRef = pbview::ViewOrRef<Msg>;

    auto binStr = allTypes.SerializeAsString();
    auto view = View::fromBytesString(binStr);

    auto runChecksOn = [&allTypes](const auto& viewOrVal){
        REQUIRE_FALSE(viewOrVal.opt_double_field());
        REQUIRE_FALSE(viewOrVal.opt_float_field());
        REQUIRE_FALSE(viewOrVal.opt_int32_field());
        REQUIRE_FALSE(viewOrVal.opt_int64_field());
        REQUIRE_FALSE(viewOrVal.opt_uint32_field());
        REQUIRE_FALSE(viewOrVal.opt_uint64_field());
        REQUIRE_FALSE(viewOrVal.opt_sint32_field());
        REQUIRE_FALSE(viewOrVal.opt_sint64_field());
        REQUIRE_FALSE(viewOrVal.opt_fixed32_field());
        REQUIRE_FALSE(viewOrVal.opt_fixed64_field());
        REQUIRE_FALSE(viewOrVal.opt_sfixed32_field());
        REQUIRE_FALSE(viewOrVal.opt_sfixed64_field());
        REQUIRE_FALSE(viewOrVal.opt_bool_field());
        REQUIRE_FALSE(viewOrVal.opt_string_field());
        REQUIRE_FALSE(viewOrVal.opt_bytes_field());
        REQUIRE_FALSE(viewOrVal.opt_myenum_field());
        REQUIRE_FALSE(viewOrVal.opt_mysubmsg_field());
    };

    SECTION("ViewOrValue containing view")
    {
        runChecksOn(ViewOrValue{view});
    }

    SECTION("ViewOrValue containing value")
    {
        runChecksOn(ViewOrValue{allTypes});
    }

    SECTION("ViewOrRef containing view")
    {
        runChecksOn(ViewOrRef{view});
    }

    SECTION("ViewOrRef containing value")
    {
        runChecksOn(ViewOrRef{allTypes});
    }
}
