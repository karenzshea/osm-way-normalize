#ifndef OSMIUM_UTIL_DELTA_HPP
#define OSMIUM_UTIL_DELTA_HPP

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

#include <osmium/util/cast.hpp>

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace osmium {

    inline namespace util {

        /**
         * Helper class for delta encoding.
         */
        template <typename TValue, typename TDelta = int64_t>
        class DeltaEncode {

            static_assert(std::is_integral<TValue>::value,
                          "DeltaEncode value type must be some integer");

            static_assert(std::is_integral<TDelta>::value && std::is_signed<TDelta>::value,
                          "DeltaEncode delta type must be some signed integer");

            // Not a perfect check, because of signed vs. unsigned, but
            // might find some problems.
            static_assert(sizeof(TDelta) >= sizeof(TValue),
                          "Delta type size should be larger or equal to value type size");

            TValue m_value;

        public:

            using value_type = TValue;
            using delta_type = TDelta;

            explicit DeltaEncode(TValue value = 0) :
                m_value(value) {
            }

            void clear() noexcept {
                m_value = 0;
            }

            TValue value() const noexcept {
                return m_value;
            }

            TDelta update(TValue new_value) noexcept {
                using std::swap;
                swap(m_value, new_value);
                // Checking the static_cast here doesn't help much, because
                // the substraction can still lead to an overflow. This is
                // dependend on the input data being "reasonable". XXX
                return static_cast<TDelta>(m_value) -
                       static_cast<TDelta>(new_value);
            }

        }; // class DeltaEncode

        /**
         * Helper class for delta decoding.
         */
        template <typename TValue, typename TDelta = int64_t>
        class DeltaDecode {

            static_assert(std::is_integral<TValue>::value,
                          "DeltaDecode value type must be some integer");

            static_assert(std::is_integral<TDelta>::value && std::is_signed<TDelta>::value,
                          "DeltaDecode delta type must be some signed integer");

            TValue m_value;

        public:

            using value_type = TValue;
            using delta_type = TDelta;

            DeltaDecode() :
                m_value(0) {
            }

            void clear() noexcept {
                m_value = 0;
            }

            TValue update(TDelta delta) noexcept {
                // Do not check for overflow. With real data this should not
                // happen and if somebody is trying to trick us they can only
                // create values this way they would also be able to generate
                // without having an overflow.
                m_value = static_cast<TValue>(
                              static_cast<TDelta>(m_value) + delta);
                return m_value;
            }

        }; // class DeltaDecode

    } // namespace util

} // namespace osmium

#endif // OSMIUM_UTIL_DELTA_HPP
