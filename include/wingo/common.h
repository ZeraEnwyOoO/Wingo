 /*
 * Wingo — P2P Internet Sharing Tool
 * Copyright (C) 2024 Wingo Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WINGO_COMMON_H
#define WINGO_COMMON_H

/*
 * ============================================================================
 * WINGO COMMON HEADER
 * ============================================================================
 *
 * This header provides:
 *   - Version information
 *   - Standard includes
 *   - Basic type definitions
 *   - Boolean type
 *   - Utility macros
 *   - Compiler attributes
 *   - Endianness helpers
 *
 * ============================================================================
 */

/* ============================================================================
 * VERSION INFORMATION
 * ============================================================================ */

#define WINGO_VERSION_MAJOR     0
#define WINGO_VERSION_MINOR     1
#define WINGO_VERSION_PATCH     0
#define WINGO_VERSION_STRING    "0.1.0"
#define WINGO_NAME              "Wingo"
#define WINGO_DESCRIPTION       "P2P Internet Sharing Tool"

/* ============================================================================
 * C STANDARD CHECK
 * ============================================================================ */

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#error "Wingo requires C11 or later. Please compile with -std=c11"
#endif

/* ============================================================================
 * STANDARD INCLUDES
 * ============================================================================ */

#include <stddef.h>     /* size_t, ptrdiff_t, NULL */
#include <stdint.h>     /* uint8_t, uint16_t, uint32_t, uint64_t */
#include <stdbool.h>    /* bool, true, false */
#include <stdlib.h>     /* malloc, free, exit */
#include <string.h>     /* memcpy, memset, strlen */
#include <stdio.h>      /* printf, fprintf, snprintf */

/* ============================================================================
 * BASIC TYPE DEFINITIONS
 * ============================================================================ */

/*
 * Fixed-width integer types for network and crypto operations.
 * These are aliases for stdint.h types for consistency.
 */

typedef uint8_t     wingo_u8;
typedef uint16_t    wingo_u16;
typedef uint32_t    wingo_u32;
typedef uint64_t    wingo_u64;

typedef int8_t      wingo_i8;
typedef int16_t     wingo_i16;
typedef int32_t     wingo_i32;
typedef int64_t     wingo_i64;

/*
 * Size and pointer types.
 */

typedef size_t      wingo_size;
typedef ptrdiff_t   wingo_ssize;

/*
 * Boolean type (C11 has stdbool.h, but we alias for consistency).
 */

typedef bool        wingo_bool;

/*
 * Byte type for raw data.
 */

typedef unsigned char wingo_byte;

/*
 * Network address types (opaque, defined in net/socket.h).
 */

typedef struct wingo_addr wingo_addr;

/*
 * Peer ID type (20 bytes, like BitTorrent).
 */

#define WINGO_ID_SIZE       20
#define WINGO_ID_HEX_SIZE   41  /* 20 bytes * 2 + null terminator */

typedef struct {
    wingo_u8 bytes[WINGO_ID_SIZE];
} wingo_id;

/*
 * Node ID type (same as peer ID).
 */

typedef wingo_id wingo_node_id;

/*
 * Info hash type (20 bytes, like BitTorrent).
 */

typedef wingo_id wingo_info_hash;

/* ============================================================================
 * BOOLEAN CONSTANTS
 * ============================================================================ */

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef WINGO_TRUE
#define WINGO_TRUE   true
#endif

#ifndef WINGO_FALSE
#define WINGO_FALSE  false
#endif

/* ============================================================================
 * UTILITY MACROS
 * ============================================================================ */

/*
 * Minimum and maximum.
 */

#define WINGO_MIN(a, b)     ((a) < (b) ? (a) : (b))
#define WINGO_MAX(a, b)     ((a) > (b) ? (a) : (b))

/*
 * Clamp value between min and max.
 */

#define WINGO_CLAMP(x, min, max) \
    ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/*
 * Array size.
 */

#define WINGO_ARRAY_SIZE(arr)   (sizeof(arr) / sizeof((arr)[0]))

/*
 * Unused parameter (to silence compiler warnings).
 */

#define WINGO_UNUSED(x)         ((void)(x))

/*
 * Align value up to a power of two.
 */

#define WINGO_ALIGN_UP(x, align) \
    (((x) + ((align) - 1)) & ~((align) - 1))

/*
 * Align value down to a power of two.
 */

#define WINGO_ALIGN_DOWN(x, align) \
    ((x) & ~((align) - 1))

/*
 * Check if value is power of two.
 */

#define WINGO_IS_POWER_OF_TWO(x) \
    ((x) != 0 && ((x) & ((x) - 1)) == 0)

/*
 * Offset of a field in a struct.
 */

#define WINGO_OFFSETOF(type, member) \
    offsetof(type, member)

/*
 * Container of a member pointer.
 */

#define WINGO_CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/*
 * Stringify macro argument.
 */

#define WINGO_STRINGIFY(x)      #x
#define WINGO_TOSTRING(x)       WINGO_STRINGIFY(x)

/*
 * Concatenate macro arguments.
 */

#define WINGO_CONCAT(a, b)      a##b
#define WINGO_CONCAT2(a, b)     WINGO_CONCAT(a, b)

/*
 * Bit manipulation.
 */

#define WINGO_BIT_SET(x, bit)       ((x) |= (1U << (bit)))
#define WINGO_BIT_CLEAR(x, bit)     ((x) &= ~(1U << (bit)))
#define WINGO_BIT_TOGGLE(x, bit)    ((x) ^= (1U << (bit)))
#define WINGO_BIT_CHECK(x, bit)     (((x) >> (bit)) & 1U)

/*
 * Set/Clear multiple bits.
 */

#define WINGO_BITS_SET(x, mask)     ((x) |= (mask))
#define WINGO_BITS_CLEAR(x, mask)   ((x) &= ~(mask))
#define WINGO_BITS_CHECK(x, mask)   (((x) & (mask)) == (mask))

/* ============================================================================
 * COMPILER ATTRIBUTES
 * ============================================================================ */

#if defined(__GNUC__) || defined(__clang__)

/*
 * Function attributes.
 */

#define WINGO_ATTR_FORMAT(fmt, args) \
    __attribute__((format(printf, fmt, args)))

#define WINGO_ATTR_NORETURN \
    __attribute__((noreturn))

#define WINGO_ATTR_UNUSED \
    __attribute__((unused))

#define WINGO_ATTR_USED \
    __attribute__((used))

#define WINGO_ATTR_PACKED \
    __attribute__((packed))

#define WINGO_ATTR_ALIGNED(n) \
    __attribute__((aligned(n)))

#define WINGO_ATTR_WEAK \
    __attribute__((weak))

#define WINGO_ATTR_PURE \
    __attribute__((pure))

#define WINGO_ATTR_CONST \
    __attribute__((const))

#define WINGO_ATTR_MALLOC \
    __attribute__((malloc))

#define WINGO_ATTR_NONNULL(...) \
    __attribute__((nonnull(__VA_ARGS__)))

#define WINGO_ATTR_WARN_UNUSED_RESULT \
    __attribute__((warn_unused_result))

#define WINGO_ATTR_DEPRECATED(msg) \
    __attribute__((deprecated(msg)))

#define WINGO_ATTR_FALLTHROUGH \
    __attribute__((fallthrough))

#define WINGO_LIKELY(x)     __builtin_expect(!!(x), 1)
#define WINGO_UNLIKELY(x)   __builtin_expect(!!(x), 0)

#else

/*
 * Fallback for non-GCC/Clang compilers.
 */

#define WINGO_ATTR_FORMAT(fmt, args)
#define WINGO_ATTR_NORETURN
#define WINGO_ATTR_UNUSED
#define WINGO_ATTR_USED
#define WINGO_ATTR_PACKED
#define WINGO_ATTR_ALIGNED(n)
#define WINGO_ATTR_WEAK
#define WINGO_ATTR_PURE
#define WINGO_ATTR_CONST
#define WINGO_ATTR_MALLOC
#define WINGO_ATTR_NONNULL(...)
#define WINGO_ATTR_WARN_UNUSED_RESULT
#define WINGO_ATTR_DEPRECATED(msg)
#define WINGO_ATTR_FALLTHROUGH
#define WINGO_LIKELY(x)     (x)
#define WINGO_UNLIKELY(x)   (x)

#endif /* __GNUC__ || __clang__ */

/* ============================================================================
 * STATIC ASSERT (C11)
 * ============================================================================ */

/*
 * Compile-time assertion.
 * Usage: WINGO_STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes);
 */

#define WINGO_STATIC_ASSERT(cond, msg) \
    _Static_assert(cond, #msg)

/*
 * Assert sizes of basic types.
 */

WINGO_STATIC_ASSERT(sizeof(wingo_u8) == 1, wingo_u8_must_be_1_byte);
WINGO_STATIC_ASSERT(sizeof(wingo_u16) == 2, wingo_u16_must_be_2_bytes);
WINGO_STATIC_ASSERT(sizeof(wingo_u32) == 4, wingo_u32_must_be_4_bytes);
WINGO_STATIC_ASSERT(sizeof(wingo_u64) == 8, wingo_u64_must_be_8_bytes);

WINGO_STATIC_ASSERT(sizeof(wingo_i8) == 1, wingo_i8_must_be_1_byte);
WINGO_STATIC_ASSERT(sizeof(wingo_i16) == 2, wingo_i16_must_be_2_bytes);
WINGO_STATIC_ASSERT(sizeof(wingo_i32) == 4, wingo_i32_must_be_4_bytes);
WINGO_STATIC_ASSERT(sizeof(wingo_i64) == 8, wingo_i64_must_be_8_bytes);

WINGO_STATIC_ASSERT(sizeof(wingo_id) == WINGO_ID_SIZE, wingo_id_must_be_20_bytes);

/* ============================================================================
 * ENDIANNESS HELPERS
 * ============================================================================ */

/*
 * Detect endianness at compile time.
 */

#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define WINGO_LITTLE_ENDIAN  1
        #define WINGO_BIG_ENDIAN     0
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define WINGO_LITTLE_ENDIAN  0
        #define WINGO_BIG_ENDIAN     1
    #else
        #error "Unknown endianness"
    #endif
#else
    #error "Cannot detect endianness"
#endif

/*
 * Byte swap functions (inline).
 */

static inline wingo_u16 wingo_bswap16(wingo_u16 x)
{
    return (wingo_u16)((x << 8) | (x >> 8));
}

static inline wingo_u32 wingo_bswap32(wingo_u32 x)
{
    return ((x << 24) & 0xFF000000U) |
           ((x <<  8) & 0x00FF0000U) |
           ((x >>  8) & 0x0000FF00U) |
           ((x >> 24) & 0x000000FFU);
}

static inline wingo_u64 wingo_bswap64(wingo_u64 x)
{
    return ((x << 56) & 0xFF00000000000000ULL) |
           ((x << 40) & 0x00FF000000000000ULL) |
           ((x << 24) & 0x0000FF0000000000ULL) |
           ((x <<  8) & 0x000000FF00000000ULL) |
           ((x >>  8) & 0x00000000FF000000ULL) |
           ((x >> 24) & 0x0000000000FF0000ULL) |
           ((x >> 40) & 0x000000000000FF00ULL) |
           ((x >> 56) & 0x00000000000000FFULL);
}

/*
 * Host to network byte order.
 */

#if WINGO_LITTLE_ENDIAN

#define wingo_htons(x)  wingo_bswap16(x)
#define wingo_htonl(x)  wingo_bswap32(x)
#define wingo_htonll(x) wingo_bswap64(x)

#define wingo_ntohs(x)  wingo_bswap16(x)
#define wingo_ntohl(x)  wingo_bswap32(x)
#define wingo_ntohll(x) wingo_bswap64(x)

#else /* Big endian */

#define wingo_htons(x)  (x)
#define wingo_htonl(x)  (x)
#define wingo_htonll(x) (x)

#define wingo_ntohs(x)  (x)
#define wingo_ntohl(x)  (x)
#define wingo_ntohll(x) (x)

#endif /* WINGO_LITTLE_ENDIAN */

/* ============================================================================
 * COMMON CONSTANTS
 * ============================================================================ */

/*
 * Network constants.
 */

#define WINGO_MAX_PACKET_SIZE       65535
#define WINGO_MTU                   1500
#define WINGO_MIN_MTU               576

/*
 * Time constants (in seconds).
 */

#define WINGO_SECOND                1
#define WINGO_MINUTE                60
#define WINGO_HOUR                  3600
#define WINGO_DAY                   86400

/*
 * Time constants (in milliseconds).
 */

#define WINGO_MS_PER_SECOND         1000
#define WINGO_MS_PER_MINUTE         60000
#define WINGO_MS_PER_HOUR           3600000

/*
 * Time constants (in microseconds).
 */

#define WINGO_US_PER_SECOND         1000000
#define WINGO_US_PER_MILLISECOND    1000

/*
 * Buffer sizes.
 */

#define WINGO_SMALL_BUFFER          256
#define WINGO_MEDIUM_BUFFER         1024
#define WINGO_LARGE_BUFFER          4096
#define WINGO_HUGE_BUFFER           65536

/*
 * String sizes.
 */

#define WINGO_MAX_PATH              4096
#define WINGO_MAX_HOSTNAME          256
#define WINGO_MAX_IP_STRING         46  /* IPv6 max: 45 chars + null */

/*
 * Peer constants.
 */

#define WINGO_MAX_PEERS             256
#define WINGO_MAX_CONNECTIONS       64

/*
 * DHT constants.
 */

#define WINGO_DHT_BUCKET_SIZE       8
#define WINGO_DHT_MAX_BUCKETS       160
#define WINGO_DHT_MAX_NODES         2048

/* ============================================================================
 * COMMON ENUMS
 * ============================================================================ */

/*
 * Address family.
 */

typedef enum {
    WINGO_AF_UNSPEC = 0,
    WINGO_AF_INET   = 2,    /* IPv4 */
    WINGO_AF_INET6  = 10,   /* IPv6 */
} wingo_af_t;

/*
 * Socket type.
 */

typedef enum {
    WINGO_SOCK_STREAM   = 1,    /* TCP */
    WINGO_SOCK_DGRAM    = 2,    /* UDP */
    WINGO_SOCK_RAW      = 3,    /* Raw */
} wingo_sock_type_t;

/*
 * Protocol.
 */

typedef enum {
    WINGO_IPPROTO_TCP   = 6,
    WINGO_IPPROTO_UDP   = 17,
    WINGO_IPPROTO_ICMP  = 1,
    WINGO_IPPROTO_ICMPV6 = 58,
} wingo_proto_t;

/*
 * Return codes.
 */

typedef enum {
    WINGO_OK        =  0,
    WINGO_ERROR     = -1,
    WINGO_AGAIN     = -2,
    WINGO_BUSY      = -3,
    WINGO_TIMEOUT   = -4,
    WINGO_INVALID   = -5,
    WINGO_NOMEM     = -6,
    WINGO_NOTFOUND  = -7,
    WINGO_EXISTS    = -8,
    WINGO_PERM      = -9,
    WINGO_AGAIN     = -10,  /* Duplicate, kept for clarity */
} wingo_rc_t;

/* ============================================================================
 * COMMON STRUCTURES
 * ============================================================================ */

/*
 * Time value (seconds + microseconds).
 */

typedef struct {
    wingo_i64 sec;      /* Seconds */
    wingo_i64 usec;     /* Microseconds (0-999999) */
} wingo_time_t;

/*
 * Buffer (data + length + capacity).
 */

typedef struct {
    wingo_u8   *data;
    wingo_size  len;
    wingo_size  cap;
} wingo_buf_t;

/* ============================================================================
 * HELPER FUNCTIONS (INLINE)
 * ============================================================================ */

/*
 * Zero a structure.
 */

static inline void wingo_zero(void *ptr, wingo_size size)
{
    memset(ptr, 0, size);
}

/*
 * Copy memory.
 */

static inline void wingo_memcpy(void *dst, const void *src, wingo_size size)
{
    memcpy(dst, src, size);
}

/*
 * Compare memory.
 */

static inline int wingo_memcmp(const void *a, const void *b, wingo_size size)
{
    return memcmp(a, b, size);
}

/*
 * Compare IDs.
 */

static inline int wingo_id_cmp(const wingo_id *a, const wingo_id *b)
{
    return memcmp(a->bytes, b->bytes, WINGO_ID_SIZE);
}

/*
 * Check if ID is zero.
 */

static inline bool wingo_id_is_zero(const wingo_id *id)
{
    static const wingo_u8 zero[WINGO_ID_SIZE] = {0};
    return memcmp(id->bytes, zero, WINGO_ID_SIZE) == 0;
}

/*
 * Copy ID.
 */

static inline void wingo_id_copy(wingo_id *dst, const wingo_id *src)
{
    memcpy(dst->bytes, src->bytes, WINGO_ID_SIZE);
}

/*
 * Convert ID to hex string.
 * Buffer must be at least WINGO_ID_HEX_SIZE bytes.
 */

static inline void wingo_id_to_hex(const wingo_id *id, char *out)
{
    static const char hex[] = "0123456789abcdef";
    for (int i = 0; i < WINGO_ID_SIZE; i++) {
        out[i * 2]     = hex[(id->bytes[i] >> 4) & 0x0F];
        out[i * 2 + 1] = hex[id->bytes[i] & 0x0F];
    }
    out[WINGO_ID_SIZE * 2] = '\0';
}

/* ============================================================================
 * END OF HEADER
 * ============================================================================ */

#endif /* WINGO_COMMON_H */
