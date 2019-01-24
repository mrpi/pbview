#include <limits>

#include <test/samples-pb2.pbview.h>
#include <test/samples-pb2.pbvar.h>

#include <range/v3/to_container.hpp>
#include <range/v3/view/zip.hpp>

#include <benchmark/benchmark.h>

#include <range/v3/numeric/accumulate.hpp>

using namespace std::literals;

using Msg = pbview::samples::AllTypesRepeated;

void init(Msg& allTypes)
{
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
    allTypes.add_string_field(std::string(32, 'X'));

    allTypes.add_bytes_field("Lorem\0ipsum"s);
    allTypes.add_bytes_field("");
    allTypes.add_bytes_field(std::string(64, '\0'));
 
    allTypes.add_myenum_field(pbview::samples::MyEnumVal2);
    allTypes.add_myenum_field(pbview::samples::MyEnumVal1);
    allTypes.add_myenum_field(pbview::samples::MyEnumVal3);
 
    {
        auto subMsg0 = allTypes.add_mysubmsg_field();
        subMsg0->set_id(314); 
        subMsg0->set_value("asdf");

        auto subMsg1 = allTypes.add_mysubmsg_field();
        subMsg1->set_id(42); 
        subMsg1->set_value("");

        auto subMsg2 = allTypes.add_mysubmsg_field();
        subMsg2->set_id(23); 
        subMsg2->set_value(std::string(13, ' '));
    }
}

void benchDeserializeMessage(benchmark::State& state)
{
    Msg allTypes;
    init(allTypes);

    auto binStr = allTypes.SerializeAsString();

    for (auto _ : state) {
       Msg parsed;
       parsed.ParseFromString(binStr);
       benchmark::DoNotOptimize(parsed);

       auto sum = ranges::accumulate(parsed.mysubmsg_field(), 0, ranges::plus{}, &pbview::samples::MySubMsg::id);
       benchmark::DoNotOptimize(sum);
       if (sum != 314+42+23)
          throw std::runtime_error("Unexpected result!");
    }
}
BENCHMARK(benchDeserializeMessage);

void benchDeserializeMessageArena(benchmark::State& state)
{
    Msg allTypes;
    init(allTypes);

    auto binStr = allTypes.SerializeAsString();

    for (auto _ : state) {
       google::protobuf::Arena arena;
       auto parsed = google::protobuf::Arena::CreateMessage<Msg>(&arena);
       parsed->ParseFromString(binStr);
       benchmark::DoNotOptimize(parsed);

       auto sum = ranges::accumulate(parsed->mysubmsg_field(), 0, ranges::plus{}, &pbview::samples::MySubMsg::id);
       benchmark::DoNotOptimize(sum);
       if (sum != 314+42+23)
          throw std::runtime_error("Unexpected result!");
    }
}
BENCHMARK(benchDeserializeMessageArena);

void benchDeserializeRecycledMessage(benchmark::State& state)
{
    Msg allTypes;
    init(allTypes);

    auto binStr = allTypes.SerializeAsString();

    Msg parsed;
    for (auto _ : state) {
       parsed.Clear();
       parsed.ParseFromString(binStr);
       benchmark::DoNotOptimize(parsed);

       auto sum = ranges::accumulate(parsed.mysubmsg_field(), 0, ranges::plus{}, &pbview::samples::MySubMsg::id);
       benchmark::DoNotOptimize(sum);
       if (sum != 314+42+23)
          throw std::runtime_error("Unexpected result!");
    }
}
BENCHMARK(benchDeserializeRecycledMessage);

template <pbview::ParserMode mode>
void benchReadSerializedWithView(benchmark::State& state)
{
    Msg allTypes;
    init(allTypes);

    using BinReader = pbview::BinMessageView<mode>;
    using View = pbview::View<Msg, BinReader>;

    auto binStr = allTypes.SerializeAsString();

    for (auto _ : state) {
       auto view = View::fromBytesString(binStr);
       benchmark::DoNotOptimize(view);

       auto sum = ranges::accumulate(view.mysubmsg_field(), 0, ranges::plus{}, &pbview::View<pbview::samples::MySubMsg, BinReader>::id);
       benchmark::DoNotOptimize(sum);
       if (sum != 314+42+23)
          throw std::runtime_error("Unexpected result!");
    }
}

void benchReadSerializedWithView_Fast(benchmark::State& state)
{
    benchReadSerializedWithView<pbview::ParserMode::Fast>(state);
}
BENCHMARK(benchReadSerializedWithView_Fast);

void benchReadSerializedWithView_TrustedInputWithoutBoundsChecking(benchmark::State& state)
{
    benchReadSerializedWithView<pbview::ParserMode::Fast_WithoutBoundsChecking>(state);
}
BENCHMARK(benchReadSerializedWithView_TrustedInputWithoutBoundsChecking);

void benchReadSerializedWithView_StrictConforming(benchmark::State& state)
{
    benchReadSerializedWithView<pbview::ParserMode::StrictConforming>(state);
}
BENCHMARK(benchReadSerializedWithView_StrictConforming);

void init(pbview::samples::AllTypes& allTypes)
{
    allTypes.set_double_field(3.1415926); 
    allTypes.set_float_field(3.14);
    allTypes.set_int32_field(14);
    allTypes.set_int64_field(24);
    allTypes.set_uint32_field(34);
    allTypes.set_uint64_field(44);
    allTypes.set_sint32_field(54);
    allTypes.set_sint64_field(64);
    allTypes.set_fixed32_field(74);
    allTypes.set_fixed64_field(84);
    allTypes.set_sfixed32_field(94);
    allTypes.set_sfixed64_field(104);
    allTypes.set_bool_field(true);
    allTypes.set_string_field("Lorem ipsum");
    allTypes.set_bytes_field("Lorem\0ipsum"s);
 
    allTypes.set_myenum_field(pbview::samples::MyEnumVal2);
 
    allTypes.mutable_mysubmsg_field()->set_id(314); 
    allTypes.mutable_mysubmsg_field()->set_value("asdf");
}

void benchSimpleMessageDeserialized(benchmark::State& state)
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    init(allTypes);

    auto binStr = allTypes.SerializeAsString();

    for (auto _ : state) {
       Msg parsed;
       parsed.ParseFromString(binStr);
       benchmark::DoNotOptimize(parsed);

       auto val = parsed.fixed64_field();
       benchmark::DoNotOptimize(val);
       if (val != 84)
          throw std::runtime_error("Unexpected result!");
    }
}
BENCHMARK(benchSimpleMessageDeserialized);

void benchSimpleMessageDeserializedReused(benchmark::State& state)
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    init(allTypes);

    auto binStr = allTypes.SerializeAsString();

    Msg parsed;
    for (auto _ : state) {
       parsed.Clear();
       parsed.ParseFromString(binStr);
       benchmark::DoNotOptimize(parsed);

       auto val = parsed.fixed64_field();
       benchmark::DoNotOptimize(val);
       if (val != 84)
          throw std::runtime_error("Unexpected result!");
    }
}
BENCHMARK(benchSimpleMessageDeserializedReused);

template <pbview::ParserMode mode>
void benchSimpleMessage(benchmark::State& state)
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    init(allTypes);

    using BinReader = pbview::BinMessageView<mode>;
    using View = pbview::View<Msg, BinReader>;

    auto binStr = allTypes.SerializeAsString();

    for (auto _ : state) {
       auto view = View::fromBytesString(binStr);
       benchmark::DoNotOptimize(view);

       auto val = view.fixed64_field();
       benchmark::DoNotOptimize(val);
       if (val != 84)
          throw std::runtime_error("Unexpected result!");
    }
}

void benchSimpleMessage_TrustedInputWithoutBoundsChecking(benchmark::State& state)
{
    benchSimpleMessage<pbview::ParserMode::Fast_WithoutBoundsChecking>(state);
}
BENCHMARK(benchSimpleMessage_TrustedInputWithoutBoundsChecking);

void benchSimpleMessage_Fast(benchmark::State& state)
{
    benchSimpleMessage<pbview::ParserMode::Fast>(state);
}
BENCHMARK(benchSimpleMessage_Fast);

void benchSimpleMessage_StrictConforming(benchmark::State& state)
{
    benchSimpleMessage<pbview::ParserMode::StrictConforming>(state);
}
BENCHMARK(benchSimpleMessage_StrictConforming);

void benchSimpleMessage_Fast_Variant(benchmark::State& state)
{
    using Msg = pbview::samples::AllTypes;
    Msg allTypes;
    init(allTypes);

    using View = pbview::View<Msg>;
    using ViewOrRef = pbview::ViewOrRef<Msg>;

    auto binStr = allTypes.SerializeAsString();

    for (auto _ : state) {
       auto var = ViewOrRef{View::fromBytesString(binStr)};
       benchmark::DoNotOptimize(var);

       auto val = var.fixed64_field();
       benchmark::DoNotOptimize(val);
       if (val != 84)
          throw std::runtime_error("Unexpected result!");
    }
}
BENCHMARK(benchSimpleMessage_Fast_Variant);


BENCHMARK_MAIN();
