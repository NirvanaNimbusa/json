//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/json
//

#ifndef BOOST_JSON_DETAIL_NUMBER_IPP
#define BOOST_JSON_DETAIL_NUMBER_IPP

#include <boost/json/detail/number.hpp>
#include <boost/json/detail/assert.hpp>
#include <boost/json/detail/sse2.hpp>

namespace boost {
namespace json {
namespace detail {

inline
double
pow10(int exp) noexcept
{
    static double const tab[618] = {
                        1e-308, 1e-307, 1e-306, 1e-305, 1e-304, 1e-303, 1e-302, 1e-301,

        1e-300, 1e-299, 1e-298, 1e-297, 1e-296, 1e-295, 1e-294, 1e-293, 1e-292, 1e-291,
        1e-290, 1e-289, 1e-288, 1e-287, 1e-286, 1e-285, 1e-284, 1e-283, 1e-282, 1e-281,
        1e-280, 1e-279, 1e-278, 1e-277, 1e-276, 1e-275, 1e-274, 1e-273, 1e-272, 1e-271,
        1e-270, 1e-269, 1e-268, 1e-267, 1e-266, 1e-265, 1e-264, 1e-263, 1e-262, 1e-261,
        1e-260, 1e-259, 1e-258, 1e-257, 1e-256, 1e-255, 1e-254, 1e-253, 1e-252, 1e-251,
        1e-250, 1e-249, 1e-248, 1e-247, 1e-246, 1e-245, 1e-244, 1e-243, 1e-242, 1e-241,
        1e-240, 1e-239, 1e-238, 1e-237, 1e-236, 1e-235, 1e-234, 1e-233, 1e-232, 1e-231,
        1e-230, 1e-229, 1e-228, 1e-227, 1e-226, 1e-225, 1e-224, 1e-223, 1e-222, 1e-221,
        1e-220, 1e-219, 1e-218, 1e-217, 1e-216, 1e-215, 1e-214, 1e-213, 1e-212, 1e-211,
        1e-210, 1e-209, 1e-208, 1e-207, 1e-206, 1e-205, 1e-204, 1e-203, 1e-202, 1e-201,

        1e-200, 1e-199, 1e-198, 1e-197, 1e-196, 1e-195, 1e-194, 1e-193, 1e-192, 1e-191,
        1e-190, 1e-189, 1e-188, 1e-187, 1e-186, 1e-185, 1e-184, 1e-183, 1e-182, 1e-181,
        1e-180, 1e-179, 1e-178, 1e-177, 1e-176, 1e-175, 1e-174, 1e-173, 1e-172, 1e-171,
        1e-170, 1e-169, 1e-168, 1e-167, 1e-166, 1e-165, 1e-164, 1e-163, 1e-162, 1e-161,
        1e-160, 1e-159, 1e-158, 1e-157, 1e-156, 1e-155, 1e-154, 1e-153, 1e-152, 1e-151,
        1e-150, 1e-149, 1e-148, 1e-147, 1e-146, 1e-145, 1e-144, 1e-143, 1e-142, 1e-141,
        1e-140, 1e-139, 1e-138, 1e-137, 1e-136, 1e-135, 1e-134, 1e-133, 1e-132, 1e-131,
        1e-130, 1e-129, 1e-128, 1e-127, 1e-126, 1e-125, 1e-124, 1e-123, 1e-122, 1e-121,
        1e-120, 1e-119, 1e-118, 1e-117, 1e-116, 1e-115, 1e-114, 1e-113, 1e-112, 1e-111,
        1e-110, 1e-109, 1e-108, 1e-107, 1e-106, 1e-105, 1e-104, 1e-103, 1e-102, 1e-101,

        1e-100, 1e-099, 1e-098, 1e-097, 1e-096, 1e-095, 1e-094, 1e-093, 1e-092, 1e-091,
        1e-090, 1e-089, 1e-088, 1e-087, 1e-086, 1e-085, 1e-084, 1e-083, 1e-082, 1e-081,
        1e-080, 1e-079, 1e-078, 1e-077, 1e-076, 1e-075, 1e-074, 1e-073, 1e-072, 1e-071,
        1e-070, 1e-069, 1e-068, 1e-067, 1e-066, 1e-065, 1e-064, 1e-063, 1e-062, 1e-061,
        1e-060, 1e-059, 1e-058, 1e-057, 1e-056, 1e-055, 1e-054, 1e-053, 1e-052, 1e-051,
        1e-050, 1e-049, 1e-048, 1e-047, 1e-046, 1e-045, 1e-044, 1e-043, 1e-042, 1e-041,
        1e-040, 1e-039, 1e-038, 1e-037, 1e-036, 1e-035, 1e-034, 1e-033, 1e-032, 1e-031,
        1e-030, 1e-029, 1e-028, 1e-027, 1e-026, 1e-025, 1e-024, 1e-023, 1e-022, 1e-021,
        1e-020, 1e-019, 1e-018, 1e-017, 1e-016, 1e-015, 1e-014, 1e-013, 1e-012, 1e-011,
        1e-010, 1e-009, 1e-008, 1e-007, 1e-006, 1e-005, 1e-004, 1e-003, 1e-002, 1e-001,

        1e+000, 1e+001, 1e+002, 1e+003, 1e+004, 1e+005, 1e+006, 1e+007, 1e+008, 1e+009,
        1e+010, 1e+011, 1e+012, 1e+013, 1e+014, 1e+015, 1e+016, 1e+017, 1e+018, 1e+019,
        1e+020, 1e+021, 1e+022, 1e+023, 1e+024, 1e+025, 1e+026, 1e+027, 1e+028, 1e+029,
        1e+030, 1e+031, 1e+032, 1e+033, 1e+034, 1e+035, 1e+036, 1e+037, 1e+038, 1e+039,
        1e+040, 1e+041, 1e+042, 1e+043, 1e+044, 1e+045, 1e+046, 1e+047, 1e+048, 1e+049,
        1e+050, 1e+051, 1e+052, 1e+053, 1e+054, 1e+055, 1e+056, 1e+057, 1e+058, 1e+059,
        1e+060, 1e+061, 1e+062, 1e+063, 1e+064, 1e+065, 1e+066, 1e+067, 1e+068, 1e+069,
        1e+070, 1e+071, 1e+072, 1e+073, 1e+074, 1e+075, 1e+076, 1e+077, 1e+078, 1e+079,
        1e+080, 1e+081, 1e+082, 1e+083, 1e+084, 1e+085, 1e+086, 1e+087, 1e+088, 1e+089,
        1e+090, 1e+091, 1e+092, 1e+093, 1e+094, 1e+095, 1e+096, 1e+097, 1e+098, 1e+099,

        1e+100, 1e+101, 1e+102, 1e+103, 1e+104, 1e+105, 1e+106, 1e+107, 1e+108, 1e+109,
        1e+110, 1e+111, 1e+112, 1e+113, 1e+114, 1e+115, 1e+116, 1e+117, 1e+118, 1e+119,
        1e+120, 1e+121, 1e+122, 1e+123, 1e+124, 1e+125, 1e+126, 1e+127, 1e+128, 1e+129,
        1e+130, 1e+131, 1e+132, 1e+133, 1e+134, 1e+135, 1e+136, 1e+137, 1e+138, 1e+139,
        1e+140, 1e+141, 1e+142, 1e+143, 1e+144, 1e+145, 1e+146, 1e+147, 1e+148, 1e+149,
        1e+150, 1e+151, 1e+152, 1e+153, 1e+154, 1e+155, 1e+156, 1e+157, 1e+158, 1e+159,
        1e+160, 1e+161, 1e+162, 1e+163, 1e+164, 1e+165, 1e+166, 1e+167, 1e+168, 1e+169,
        1e+170, 1e+171, 1e+172, 1e+173, 1e+174, 1e+175, 1e+176, 1e+177, 1e+178, 1e+179,
        1e+180, 1e+181, 1e+182, 1e+183, 1e+184, 1e+185, 1e+186, 1e+187, 1e+188, 1e+189,
        1e+190, 1e+191, 1e+192, 1e+193, 1e+194, 1e+195, 1e+196, 1e+197, 1e+198, 1e+199,

        1e+200, 1e+201, 1e+202, 1e+203, 1e+204, 1e+205, 1e+206, 1e+207, 1e+208, 1e+209,
        1e+210, 1e+211, 1e+212, 1e+213, 1e+214, 1e+215, 1e+216, 1e+217, 1e+218, 1e+219,
        1e+220, 1e+221, 1e+222, 1e+223, 1e+224, 1e+225, 1e+226, 1e+227, 1e+228, 1e+229,
        1e+230, 1e+231, 1e+232, 1e+233, 1e+234, 1e+235, 1e+236, 1e+237, 1e+238, 1e+239,
        1e+240, 1e+241, 1e+242, 1e+243, 1e+244, 1e+245, 1e+246, 1e+247, 1e+248, 1e+249,
        1e+250, 1e+251, 1e+252, 1e+253, 1e+254, 1e+255, 1e+256, 1e+257, 1e+258, 1e+259,
        1e+260, 1e+261, 1e+262, 1e+263, 1e+264, 1e+265, 1e+266, 1e+267, 1e+268, 1e+269,
        1e+270, 1e+271, 1e+272, 1e+273, 1e+274, 1e+275, 1e+276, 1e+277, 1e+278, 1e+279,
        1e+280, 1e+281, 1e+282, 1e+283, 1e+284, 1e+285, 1e+286, 1e+287, 1e+288, 1e+289,
        1e+290, 1e+291, 1e+292, 1e+293, 1e+294, 1e+295, 1e+296, 1e+297, 1e+298, 1e+299,

        1e+300, 1e+301, 1e+302, 1e+303, 1e+304, 1e+305, 1e+306, 1e+307, 1e+308 };

    exp += 308;
    BOOST_JSON_ASSERT(exp >= 0 && exp < 618);
    return tab[exp];
}

// return true on '-' '0' '1'..'9'.
// caller consumes ch on true
bool
number_parser::
maybe_init(char ch) noexcept
{
    if(ch == '-')
    {
        n_.u = 0;
        n_.kind = kind::int64;
        exp_ = 0;
        dig_ = 0;
        off_ = 0;
        neg_ = true;
        st_ = state::init0;
        return true;
    }
    unsigned char const d = ch - '0';
    if(d >= 10)
        return false;
    n_.u = d;
    exp_ = 0;
    off_ = 0;
    neg_ = false;
    n_.kind = kind::int64;
    if(ch == '0')
    {
        dig_ = 0;
        st_ = state::init1;
    }
    else
    {
        dig_ = 1;
        st_ = state::mant;
    }
    return true;
}

size_t
number_parser::
write_some(
    char const* const data,
    size_t const size,
    error_code& ec) noexcept
{
    auto p = data;
    auto const p0 = data;
    auto const p1 = data + size;
    ec = {};
loop:
    switch(st_)
    {
    case state::init:
        if(p >= p1)
            break;
        if(! maybe_init(*p))
        {
            ec = error::not_number;
            goto finish;
        }
        ++p;
        goto loop;

    //-----------------------------------

    // [0,1..9]
    case state::init0:
    {
        BOOST_JSON_ASSERT(
            n_.kind == kind::int64);
        BOOST_JSON_ASSERT(neg_);
        if(p >= p1)
            break;
        unsigned char const d = *p - '0';
        if(d >= 10)
        {
            ec = error::expected_mantissa;
            goto finish;
        }
        ++p;
        if(d == 0)
        {
            st_ = state::init1;
            goto loop;
        }
        n_.u = d;
        dig_ = 1;
        st_ = state::mantn;
        goto loop;
    }

    // [.eE]
    case state::init1:
        BOOST_JSON_ASSERT(
            n_.kind == kind::int64);
        if(p >= p1)
            break;
        if(*p == 'e' || *p == 'E')
        {
            ++p;
            n_.d = 0;
            n_.kind = kind::double_;
            st_ = state::exp1;
            goto loop;
        }
        if(*p == '.')
        {
            ++p;
            st_ = state::frac1;
            goto loop;
        }
        // zero
        n_.u = 0;
        st_ = state::done;
        goto finish;

    //-----------------------------------

    // *[0..9]
    case state::mant:
    {
        BOOST_JSON_ASSERT(
            n_.kind == kind::int64);
        BOOST_JSON_ASSERT(! neg_);
        if(p < p1)
        {
            auto m = n_.u;
            do
            {
                unsigned char const d = *p - '0';
                if(d < 10)
                {
                    //       18446744073709551615 UINT64_MAX
                    if( m  > 1844674407370955161 || (
                        m == 1844674407370955161 && d > 5))
                    {
                        ++p;
                        n_.d = static_cast<double>(m) * 10;
                        n_.kind = kind::double_;
                        st_ = state::mantd;
                        goto loop;
                    }
                    ++p;
                    ++dig_;
                    m = 10 * m + d;
                    continue;
                }
                if(*p == '.')
                {
                    ++p;
                    st_ = state::frac1;
                    goto loop;
                }
                if(*p == 'e' || *p == 'E')
                {
                    ++p;
                    n_.d = static_cast<double>(m);
                    n_.kind = kind::double_;
                    st_ = state::exp1;
                    goto loop;
                }
                if(m <= INT64_MAX)
                {
                    n_.i = static_cast<
                        int64_t>(m);
                }
                else
                {
                    n_.u = m;
                    n_.kind = kind::uint64;
                }
                st_ = state::done;
                goto finish;
            }
            while(p < p1);
            n_.u = m;
        }
        break;
    }

    // *[0..9] (negative)
    case state::mantn:
    {
        BOOST_JSON_ASSERT(n_.kind == kind::int64);
        BOOST_JSON_ASSERT(neg_);
        if(p < p1)
        {
            auto m = n_.u;
            do
            {
                unsigned char const d = *p - '0';
                if(d < 10)
                {
                    //       9223372036854775808 INT64_MIN
                    if( m  > 922337203685477580 || (
                        m == 922337203685477580 && d > 8))
                    {
                        n_.d = static_cast<double>(m);
                        n_.kind = kind::double_;
                        st_ = state::mantd;
                        goto loop;
                    }
                    m = 10 * m + d;
                    ++dig_;
                    ++p;
                    continue;
                }
                if(*p == '.')
                {
                    ++p;
                    st_ = state::frac1;
                    goto loop;
                }
                if(*p == 'e' || *p == 'E')
                {
                    ++p;
                    n_.d = static_cast<double>(m);
                    n_.kind = kind::double_;
                    st_ = state::exp1;
                    goto loop;
                }
                n_.u = m;
                st_ = state::done;
                goto finish;
            }
            while(p < p1);
            n_.u = m;
        }
        break;
    }

    // *[0..9] (double)
    case state::mantd:
    {
        BOOST_JSON_ASSERT(
            n_.kind == kind::double_);
        auto d = n_.d;
        while(p < p1)
        {
            if(*p == '.')
            {
                ++p;
                n_.d = d;
                st_ = state::fracd;
                goto loop;
            }
            if(*p == 'e' || *p == 'E')
            {
                ++p;
                n_.d = d;
                st_ = state::exp1;
                goto loop;
            }
            if(static_cast<unsigned char>(
                *p - '0') > 9)
            {
                n_.d = d;
                st_ = state::done;
                goto finish;
            }
            ++p;
            d = d * 10;
        }
        n_.d = d;
        break;
    }

    //-----------------------------------

    // [0..9]
    case state::frac1:
    {
        BOOST_JSON_ASSERT(
            n_.kind == kind::int64);
        if(p >= p1)
            break;
        unsigned char const d = *p - '0';
        if(d >= 10)
        {
            ec = error::expected_fraction;
            goto finish;
        }
        n_.kind = kind::double_;
        st_ = state::frac2;
        BOOST_FALLTHROUGH;
    }

    // zero or more [0..9]
    case state::frac2:
    {
        BOOST_JSON_ASSERT(
            n_.kind == kind::double_);
        if(p < p1)
        {
            auto m = n_.u;
            do
            {
                unsigned char const d = *p - '0';
                if(d < 10)
                {
                    if(m > 9007199254740991) // (2^53-1)
                    {
                        ++p;
                        n_.d = static_cast<double>(m);
                        st_ = state::fracd;
                        goto loop;
                    }
                    ++p;
                    m = 10 * m + d;
                    --off_;
                    if(m != 0)
                        ++dig_;
                    continue;
                }
                if(*p != 'e' && *p != 'E')
                {
                    n_.u = m;
                    st_ = state::done;
                    goto finish;
                }
                ++p;
                st_ = state::exp1;
                goto loop;
            }
            while(p < p1);
            n_.u = m;
        }
        break;
    }

    // zero or more [0..9] (double)
    case state::fracd:
    {
        BOOST_JSON_ASSERT(
            n_.kind == kind::double_);
        if(p < p1)
        {
            auto m = n_.d;
            do
            {
                unsigned char const d = *p - '0';
                if(d < 10)
                {
                    if(dig_ < 17)
                    {
                        m = 10 * m + d;
                        --off_;
                        if(m > 0)
                            ++dig_;
                    }
                    ++p;
                    continue;
                }
                if(*p != 'e' && *p != 'E')
                {
                    n_.d = m;
                    st_ = state::done;
                    goto finish;
                }
                ++p;
                n_.d = m;
                st_ = state::exp1;
                goto loop;
            }
            while(p < p1);
            n_.d = m;
        }
        break;
    }

    //-----------------------------------

    // + or -
    case state::exp1:
    {
        BOOST_JSON_ASSERT(
            n_.kind == kind::double_);
        if(p >= p1)
            break;
        if(*p == '-')
        {
            ++p;
            eneg_ = true;
        }
        else
        {
            if(*p == '+')
                ++p;
            eneg_ = false;
        }
        st_ = state::exp2;
        BOOST_FALLTHROUGH;
    }

    // [0..9]
    case state::exp2:
    {
        if(p >= p1)
            break;
        unsigned char const d = *p - '0';
        if(d > 9)
        {
            ec = error::expected_exponent;
            goto finish;
        }
        ++p;
        exp_ = d;
        st_ = state::exp3;
        BOOST_FALLTHROUGH;
    }

    // *[0..9]
    case state::exp3:
    {
        if(p < p1)
        {
            //auto const lim = 308 - off_;
            auto e = exp_;
            while(p < p1)
            {
                unsigned char const d = *p - '0';
                if(d < 10)
                {
                    ++p;
                    e = 10 * e + d;
                    //ec = error::exponent_overflow;
                    continue;
                }
                exp_ = e;
                st_ = state::done;
                goto finish;
            }
            while(p < p1);
            exp_ = e;
        }
        break;
    }

    case state::done:
        ec = error::illegal_extra_chars;
        break;
    }
finish:
    return p - p0;
}

size_t
number_parser::
write(
    char const* data,
    size_t size,
    error_code& ec) noexcept
{
    auto n = write_some(data, size, ec);
    if(! ec)
    {
        if(n < size)
            n += write_some(
                data + n, size - n, ec);
    }
    if(! ec)
        write_eof(ec);
    return n;
}

void
number_parser::
write_eof(
    error_code& ec) noexcept
{
    switch(st_)
    {
    case state::init:
    case state::init0:
        ec = error::expected_mantissa;
        break;

    case state::init1:
        BOOST_JSON_ASSERT(
            n_.kind == kind::int64);
        BOOST_JSON_ASSERT(n_.i == 0);
        ec = {};
        st_ = state::done;
        break;

    case state::mant:
        BOOST_JSON_ASSERT(
            n_.kind == kind::int64);
        BOOST_JSON_ASSERT(! neg_);
        ec = {};
        if(n_.u <= INT64_MAX)
            n_.i = static_cast<
                int64_t>(n_.u);
        else
            n_.kind = kind::uint64;
        st_ = state::done;
        break;

    case state::mantn:
        BOOST_JSON_ASSERT(
            n_.kind == kind::int64);
        BOOST_JSON_ASSERT(neg_);
        ec = {};
        n_.i = static_cast<
            int64_t>(~n_.u+1);
        st_ = state::done;
        break;

    case state::mantd:
        ec = {};
        if(neg_)
            n_.d = -n_.d;
        exp_ += off_;
        n_.d *= pow10(exp_);
        st_ = state::done;
        break;

    case state::frac1:
        ec = error::expected_fraction;
        break;

    case state::frac2:
        BOOST_JSON_ASSERT(
            n_.kind == kind::double_);
        ec = {};
        exp_ += off_;
        n_.d = n_.u * pow10(exp_);
        if(neg_)
            n_.d = -n_.d;
        st_ = state::done;
        break;

    case state::fracd:
        BOOST_JSON_ASSERT(
            n_.kind == kind::double_);
        ec = {};
        exp_ += off_;
        n_.d = n_.d * pow10(exp_);
        if(neg_)
            n_.d = -n_.d;
        st_ = state::done;
        break;

    case state::exp1: // expected [-,+,0..9]
        ec = error::expected_exponent;
        break;

    case state::exp2: // expected [0..9]
        ec = error::expected_exponent;
        break;

    case state::exp3:
        ec = {};
        exp_ += off_;
        if(eneg_)
            exp_ = -exp_;
        n_.d = n_.d * pow10(exp_);
        if(neg_)
            n_.d = -n_.d;
        st_ = state::done;
        break;

    case state::done:
        ec = {};
        break;
    }
}

} // detail
} // json
} // boost

#endif
