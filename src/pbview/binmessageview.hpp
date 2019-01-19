#pragma once

#include <string_view>
#include <optional>

#include <google/protobuf/message.h>
#include <google/protobuf/wire_format_lite.h>

#include <range/v3/view/generate.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>

#ifdef __GNUC__
#define PBVIEW_FORCE_INLINE __attribute__((always_inline)) 
#elif defined(__MSV_VER)
#define PBVIEW_FORCE_INLINE __forceinline
#else
#define PBVIEW_FORCE_INLINE inline
#endif

namespace pbview
{

using ValueType = const std::byte;
using DataSpan = std::basic_string_view<ValueType>;

enum class ParserMode
{
   // Same as "Fast" but without bounds checking.
   // Use this mode only for fully trusted input.
   // Malicious input could cause memory accesses after end of buffer. 
   Fast_WithoutBoundsChecking,

   // Parses input that was written by standard encoders correctly.
   // If the fields in the binary messages are not encoded strictly in
   // the order of the field nummbers.
   // If a non repeated field is occuring multiple times in the input,
   // the first value will be returned, while conforming encoders
   // have to return the last one.
   Fast,

   // This mode should behave the same as the official parsers for
   // any input.
   // The parser has to read all message up to the end even when accessing
   // the first field, as the last value of non repeated fields has to be
   // used.
   StrictConforming
};

enum class WireType : std::uint32_t
{
   Varint = 0,
   Bits64 = 1,
   LengthDelimited = 2,
   StartGroup = 3,
   EndGroup = 4,
   Bits32 = 5
};

enum class Serialization
{
   Fixed,
   Varint,
   VarintZigZag,
   LengthDelimited
};

template <ParserMode parser = ParserMode::Fast>
struct BinMessageView;

namespace impl
{
template <typename Exception = std::runtime_error, typename T, typename... ExceptionArgs>
decltype(auto) enforce(T &&t, ExceptionArgs &&... exceptionArgs)
{
   if (!!t)
      return std::forward<T>(t);
   throw Exception{std::forward<ExceptionArgs>(exceptionArgs)...};
}

template <typename T>
struct Conv
{
   static T conv(DataSpan bytes)
   {
      return T{bytes};
   }
};

template <>
struct Conv<std::string_view>
{
   static std::string_view conv(DataSpan bytes)
   {
      return std::string_view{reinterpret_cast<const char *>(bytes.data()), bytes.size()};
   }
};

} // namespace impl

namespace type
{
struct Double
{
   using CppType = double;
   static constexpr auto serialization = Serialization::Fixed;
};

struct Float
{
   using CppType = float;
   static constexpr auto serialization = Serialization::Fixed;
};

struct Int32
{
   using CppType = std::int32_t;
   static constexpr auto serialization = Serialization::Varint;
};

struct Int64
{
   using CppType = std::int64_t;
   static constexpr auto serialization = Serialization::Varint;
};

struct Uint32
{
   using CppType = std::uint32_t;
   static constexpr auto serialization = Serialization::Varint;
};

struct Uint64
{
   using CppType = std::uint64_t;
   static constexpr auto serialization = Serialization::Varint;
};

struct Sint32
{
   using CppType = std::int32_t;
   static constexpr auto serialization = Serialization::VarintZigZag;
};

struct Sint64
{
   using CppType = std::int64_t;
   static constexpr auto serialization = Serialization::VarintZigZag;
};

struct Fixed32
{
   using CppType = std::uint32_t;
   static constexpr auto serialization = Serialization::Fixed;
};

struct Fixed64
{
   using CppType = std::uint64_t;
   static constexpr auto serialization = Serialization::Fixed;
};

struct Sfixed32
{
   using CppType = std::int32_t;
   static constexpr auto serialization = Serialization::Fixed;
};

struct Sfixed64
{
   using CppType = std::int64_t;
   static constexpr auto serialization = Serialization::Fixed;
};

struct Bool
{
   using CppType = bool;
   static constexpr auto serialization = Serialization::Varint;
};

struct String
{
   using CppType = std::string_view;
   static constexpr auto serialization = Serialization::LengthDelimited;
};

struct Bytes
{
   using CppType = std::string_view;
   static constexpr auto serialization = Serialization::LengthDelimited;
};

template <typename T>
struct Enum
{
   static_assert(std::is_enum_v<T>);
   static_assert(sizeof(T) == sizeof(std::int32_t));
   using CppType = T;
   static constexpr auto serialization = Serialization::Varint;
};

struct EnumUntyped
{
   using CppType = int;
   static constexpr auto serialization = Serialization::Varint;
};

struct Message
{
   using CppType = BinMessageView<ParserMode::Fast>;
   static constexpr auto serialization = Serialization::LengthDelimited;
};

struct MessageWithIrregularEncoding
{
   using CppType = BinMessageView<ParserMode::StrictConforming>;
   static constexpr auto serialization = Serialization::LengthDelimited;
};
} // namespace type

template <ParserMode mode>
struct BinMessageView
{
   DataSpan bytes;

   explicit BinMessageView(DataSpan span) : bytes(span)
   {
   }

   static BinMessageView fromBytesString(std::string_view sv)
   {
      return pbview::BinMessageView{pbview::DataSpan{reinterpret_cast<const std::byte *>(sv.data()), sv.size()}};
   }

 private:
   static inline char pop(DataSpan &bin)
   {
      char c = static_cast<char>(bin[0]);
      bin.remove_prefix(1);
      return c;
   }

   static constexpr inline bool lastByteOfVarint(std::uint8_t c)
   {
      return !(c >> 7);
   } 

   static constexpr inline bool lastByteOfVarint(std::byte b)
   {
      return lastByteOfVarint(static_cast<std::uint8_t>(b));
   } 

   static inline DataSpan popVarint(DataSpan &bin)
   {
      auto all = bin;

      while (true)
      {
         if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
            impl::enforce(!bin.empty(), "Reached end of data while reading varint");

         if (lastByteOfVarint(pop(bin)))
            break;
      }

      return all.substr(0, bin.data() - all.data());
   }

   template <int size, typename T>
   static const std::uint8_t* VarintParse(const std::uint8_t* p, T* out) {
      T res = 0;
      T extra = 0;
      for (int i = 0; i < size; i++) {
         T byte = p[i];
         res += byte << (i * 7);
         int j = i + 1;
         if (lastByteOfVarint(byte)) {
            *out = res - extra;
            return p + j;
         }
         extra += 128ull << (i * 7);
      }
      return nullptr;
   }

   template <typename Int>
   static inline Int popVarint(DataSpan& bin)
   {
      if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
         impl::enforce(!bin.empty(), "Input was empty when trying to read a varint");

      if (const auto c = bin.front(); lastByteOfVarint(c))
      {
         bin.remove_prefix(1);
         return static_cast<std::uint8_t>(c);
      }

      auto bytes = popVarint(bin);

      Int res{};
      VarintParse<(sizeof(Int) > 4) ? 10 : 5>(reinterpret_cast<const std::uint8_t*>(bytes.data()), &res);
      return res;
   }

   static inline std::uint32_t popTag(DataSpan &bin)
   {
      if (bin.empty())
         return 0;
      return popVarint<std::uint32_t>(bin);
   }

   template<typename T>
   static inline T popFixed(DataSpan& bin)
   {
      T res{};
      memcpy(&res, bin.data(), sizeof(T));
      bin.remove_prefix(sizeof(T));
      return res;
   }

   static inline DataSpan popLengthDelimited(DataSpan& bin)
   {
      auto len = popVarint<std::uint32_t>(bin);
      if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
         impl::enforce(len <= bin.size(), "Input is shorter than the expected value");
      auto res = bin.substr(0, len);
      bin.remove_prefix(len);
      return res;
   }

   static PBVIEW_FORCE_INLINE void skipValue(DataSpan& bin, WireType type)
   {
      switch (type)
      {
      case WireType::Varint:
         popVarint(bin);
         return;
      case WireType::Bits32:
         if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
            impl::enforce(bin.size() >= sizeof(std::uint32_t), "Failed to skip 32 bit field");
         bin.remove_prefix(sizeof(std::uint32_t));
         return;
      case WireType::Bits64:
         if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
            impl::enforce(bin.size() >= sizeof(std::uint64_t), "Failed to skip 64 bits field");
         bin.remove_prefix(sizeof(std::uint64_t));
         return;
      case WireType::LengthDelimited:
      {
         auto len = popVarint<std::uint32_t>(bin);
         if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
            impl::enforce(bin.size() >= len, "Input is too short for expected length delimited value");
         bin.remove_prefix(len);
         return;
      }
      default:
         throw std::runtime_error("Failed to skip field with unknown wire type " + std::to_string(static_cast<int>(type)));
      }
   }

   inline static std::optional<WireType> seekToField(DataSpan& bin, int fieldNo)
   {
      if constexpr (mode == ParserMode::StrictConforming)
      {
         std::optional<WireType> res;
         DataSpan pos = bin;
         while(auto next = seekToNextField(pos, fieldNo))
         {
            res = next;
            bin = pos;
            skipValue(pos, *next);
         }
         return res;
      }
      else
         return seekToNextField(bin, fieldNo);
   }

   inline static std::optional<WireType> seekToNextField(DataSpan& bin, int fieldNo)
   {
      while (auto tag = popTag(bin))
      {
         const int currentFieldNumber = tag >> 3;
         constexpr uint32_t WireTypeBitMask = 0b111;
         const WireType type{tag & WireTypeBitMask};

         if (currentFieldNumber == fieldNo)
            return type;

         if constexpr (mode != ParserMode::StrictConforming)
         {
            // https://developers.google.com/protocol-buffers/docs/encoding#order
            // A valid parser has to except fields that come in random order. But the default implementations write the known fields in
            // sorted order so we can break faster on these messages.
            if (currentFieldNumber > fieldNo)
               return {};
         }

         skipValue(bin, type);
      }

      return {};
   }

   template <typename Int>
   static Int decodeZigZag(Int sn)
   {
      std::make_unsigned_t<Int> n = sn;
      return static_cast<Int>((n >> 1) ^ (~(n & 1) + 1));
   }

   struct CompiletimeError
   {
      CompiletimeError();
   };

   template <typename T>
   static constexpr WireType wireTypeOf()
   {
      if (T::serialization == Serialization::Fixed)
      {
         if (sizeof(typename T::CppType) * 8 == 64)
            return WireType::Bits64;
         else
            return WireType::Bits32;
      }
      if (T::serialization == Serialization::Varint || T::serialization == Serialization::VarintZigZag)
         return WireType::Varint;
      if (T::serialization == Serialization::LengthDelimited)
         return WireType::LengthDelimited;
   }

   template <typename T>
   static auto popNextValue(DataSpan& bin) -> typename T::CppType
   {
      constexpr auto expectedWireType = wireTypeOf<T>();
      using CppType = typename T::CppType;

      if constexpr (expectedWireType == WireType::Varint)
      {
         using UInt = std::conditional_t<sizeof(CppType) == sizeof(std::uint64_t), std::uint64_t, std::uint32_t>;
         UInt res = popVarint<std::uint64_t>(bin);
         if constexpr (T::serialization == Serialization::VarintZigZag)
            return decodeZigZag(res);
         return static_cast<CppType>(res);
      }
      else if constexpr (T::serialization == Serialization::Fixed)
      {
         if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
            impl::enforce(bin.size() >= sizeof(CppType), "Input is too short to contain the expected fixed length value");
         return popFixed<CppType>(bin);
      }
      else if constexpr (expectedWireType == WireType::LengthDelimited)
      {
         return impl::Conv<CppType>::conv(popLengthDelimited(bin));
      }
      else
         throw CompiletimeError{};
   }

   template <typename T>
   static auto popNextField(DataSpan& bin, int fieldNo) -> typename std::optional<typename T::CppType>
   {
      constexpr auto expectedWireType = wireTypeOf<T>();

      if (auto wireType = seekToNextField(bin, fieldNo))
      {
         if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
            impl::enforce(wireType == expectedWireType, "Invalid wire type!");

         return popNextValue<T>(bin);
      }

      return {};
   }

   template <typename T>
   static auto popField(DataSpan& bin, int fieldNo) -> typename std::optional<typename T::CppType>
   {
      constexpr auto expectedWireType = wireTypeOf<T>();

      if (auto wireType = seekToField(bin, fieldNo))
      {
         if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
            impl::enforce(wireType == expectedWireType, "Invalid wire type!");

         return popNextValue<T>(bin);
      }

      return {};
   }

   template <typename T>
   struct Repeated
       : ranges::view_facade<Repeated<T>, ranges::finite>
   {
    private:
      friend ranges::range_access;
      DataSpan mBytes{};
      int mFieldNo{};

      struct cursor
      {
       private:
         using CppType = typename T::CppType;
         DataSpan mBytes{};
         int mFieldNo{};
         std::optional<CppType> mValue;

       public:
         cursor() = default;

         explicit cursor(Repeated rng)
             : mBytes{rng.mBytes}, mFieldNo{rng.mFieldNo}
         {
            next();
         }

         void next()
         {
            mValue = popNextField<T>(mBytes, mFieldNo);
         }

         CppType read() const noexcept
         {
            return *mValue;
         }

         bool equal(ranges::default_sentinel) const
         {
            return !mValue;
         }

         bool equal(const cursor& other) const
         {
            assert(mFieldNo == other.mFieldNo);
            assert(mBytes.data() + mBytes.size() == other.mBytes.data() + other.mBytes.size());
            return mBytes == other.mBytes;
         }
      };

      cursor begin_cursor() const
      {
         return cursor{*this};
      }

    public:
      Repeated() = default;

      Repeated(DataSpan bytes, int fieldNo)
          : mBytes(bytes), mFieldNo(fieldNo)
      {
      }
   };

   template <typename T>
   struct PackedRepeated
       : ranges::view_facade<PackedRepeated<T>, ranges::finite>
   {
    private:
      friend ranges::range_access;
      DataSpan mBytes{};

      struct cursor
      {
       private:
         using CppType = typename T::CppType;
         DataSpan mBytes{};
         std::optional<CppType> mValue;

       public:
         cursor() = default;

         explicit cursor(PackedRepeated rng)
             : mBytes{rng.mBytes}
         {
            next();
         }

         void next()
         {
            if (mBytes.empty())
               mValue.reset();
            else
               mValue = popNextValue<T>(mBytes);
         }

         CppType read() const noexcept
         {
            return *mValue;
         }

         bool equal(ranges::default_sentinel) const
         {
            return !mValue;
         }

         bool equal(const cursor& other) const
         {
            assert(mBytes.data() + mBytes.size() == other.mBytes.data() + other.mBytes.size());
            return mBytes == other.mBytes;
         }
      };

      cursor begin_cursor()
      {
         return cursor{*this};
      }

    public:
      PackedRepeated() = default;

      PackedRepeated(DataSpan bytes, int fieldNo)
      {
         if (auto wireType = seekToNextField(bytes, fieldNo))
         {
            if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
               impl::enforce(wireType == WireType::LengthDelimited, "Invalid wire type on reading packed repeated field");
            auto len = popVarint<std::uint32_t>(bytes);
            if constexpr (mode != ParserMode::Fast_WithoutBoundsChecking)
               impl::enforce(len <= bytes.size(), "Input too short for expected packed repeated field");
            mBytes = bytes.substr(0, len);
         }
      }
   };

 public:
   bool has(int fieldNo) const
   {
      auto bin = bytes;
      return seekToNextField(bin, fieldNo) != std::nullopt;
   }

   template <typename T>
   auto get(int fieldNo) const -> typename std::optional<typename T::CppType>
   {
      auto bin = bytes;
      return popField<T>(bin, fieldNo);
   }

   template <typename T>
   auto getRepeated(int fieldNo) const
   {
      auto bin = bytes;
      return Repeated<T>(bin, fieldNo);
   }

   template <typename T>
   auto getPackedRepeated(int fieldNo) const
   {
      auto bin = bytes;
      return PackedRepeated<T>(bin, fieldNo);
   }
};

template <typename T, ParserMode parserMode>
T deserialize(BinMessageView<parserMode> msgView)
{
   T msg;
   google::protobuf::io::CodedInputStream is{
      reinterpret_cast<const std::uint8_t *>(msgView.bytes.data()), 
      static_cast<int>(msgView.bytes.size())};
   msg.MergePartialFromCodedStream(&is);
   return msg;
}

template <typename Msg, typename BinView>
struct ViewFor
{};

template <typename Msg, typename BinView = BinMessageView<>>
using View = typename ViewFor<Msg, BinView>::Type;

template <typename Msg>
struct VariantFor
{};

template <typename Msg, typename BinView = BinMessageView<>>
using ViewOrValue = typename VariantFor<Msg>::template Type<typename ViewFor<Msg, BinView>::Type, Msg>;

template <typename Msg, typename BinView = BinMessageView<>>
using ViewOrRef = typename VariantFor<Msg>::template Type<typename ViewFor<Msg, BinView>::Type, std::reference_wrapper<Msg>>;

namespace impl
{
   template<typename T>
   decltype(auto) unwrap(T&& t)
   {
      return std::forward<T>(t);
   }

   template<typename T>
   decltype(auto) unwrap(std::reference_wrapper<T> t)
   {
      return t.get();
   }

   template<typename T, typename = std::enable_if_t<!std::is_convertible_v<T, const google::protobuf::Message&>>>
   decltype(auto) wrap(T&& t)
   {
      return std::forward<T>(t);
   }

   template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, const google::protobuf::Message&>>>
   auto wrap(T&& t)
   {
      return std::cref(t);
   }
}

} // namespace pbview
