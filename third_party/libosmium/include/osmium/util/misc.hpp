#ifndef OSMIUM_UTIL_MISC_HPP
#define OSMIUM_UTIL_MISC_HPP

/*

This file is part of Osmium (https://osmcode.org/libosmium).

Copyright 2013-2018 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <type_traits>

namespace osmium {

    /**
     * Like std::tie(), but takes its arguments as const references. Used
     * as a helper function when sorting.
     */
    template <typename... Ts>
    inline std::tuple<const Ts&...>
    const_tie(const Ts&... args) noexcept {
        return std::tuple<const Ts&...>(args...);
    }

    namespace detail {

        /**
         * Interpret the input string as number. Leading white space is
         * ignored. If there is any error, return 0.
         *
         * @tparam TReturn The return type.
         * @param str The input string.
         *
         * @pre @code str != nullptr @endcode
         *
         */

        template <typename TReturn>
        inline TReturn str_to_int(const char* str) {
            static_assert(std::is_integral<TReturn>::value, "Must be integral type");
            using r_type = typename std::conditional<std::is_unsigned<TReturn>::value, unsigned long long, long long>::type; // NOLINT(google-runtime-int)
            assert(str);
            char* end = nullptr;
            const auto value = std::strtoll(str, &end, 10);
            if (value < 0 || value == std::numeric_limits<long long>::max() || static_cast<r_type>(value) >= std::numeric_limits<TReturn>::max() || end == nullptr || *end != '\0') { // NOLINT(google-runtime-int)
                return 0;
            }

            return static_cast<TReturn>(value);
        }

    } // namespace detail

} // namespace osmium

#endif // OSMIUM_UTIL_MISC_HPP
