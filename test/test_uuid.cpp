//  (C) Copyright Andy Tompkins 2007. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

//  libs/uuid/test/test_uuid.cpp  -------------------------------//

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/current_function.hpp>
#include <iostream>

void test_uuid_equal_array(char const * file, int line, char const * function,
                           boost::uuids::uuid const& lhs, const unsigned char (&rhs)[16])
{
    for (size_t i=0; i<16; i++) {
        if ( *(lhs.begin()+i) != rhs[i]) {
            std::cerr << file << "(" << line << "): uuid " << lhs << " not equal " << "{";
            for (size_t j=0; j<16; j++) {
                if (j != 0) {
                    std::cerr << " ";
                }
                std::cerr << std::hex << (int)rhs[j];
            }
            std::cerr << "} in function '" << function << "'" << std::endl;
            ++boost::detail::test_errors();
            return;
        }
    }
}


#define BOOST_TEST_UUID(lhs, rhs) ( test_uuid_equal_array(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, lhs, rhs) )

int main()
{
    using namespace boost::uuids;

    // uuid::static_size
    BOOST_TEST_EQ(uuid::static_size(), 16U);

    { // uuid::operator=()
        uuid u1 = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
        uuid u2 = u1;
        BOOST_TEST_EQ(u2, u1);
    }

    { // uuid::begin(), end()
        uuid u = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
        unsigned char values[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        BOOST_TEST_UUID(u, values);
    }

    { // uuid::begin() const, end() const
        const uuid u = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
        unsigned char values[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        BOOST_TEST_UUID(u, values);
    }

     { // uuid::size()
        uuid u; // uninitialized
        BOOST_TEST_EQ(u.size(), 16U);
    }

    { // uuid::is_nil()
        uuid u1 = {{0}};
        BOOST_TEST_EQ(u1.is_nil(), true);

        uuid u2 = {{1,0}};
        BOOST_TEST_EQ(u2.is_nil(), false);
    }

    { // uuid::variant()
        struct Test {
            unsigned char octet7;
            boost::uuids::uuid::variant_type variant;
        };
        const Test tests[] =
            { { 0x00, boost::uuids::uuid::variant_ncs }
            , { 0x10, boost::uuids::uuid::variant_ncs }
            , { 0x20, boost::uuids::uuid::variant_ncs }
            , { 0x30, boost::uuids::uuid::variant_ncs }
            , { 0x40, boost::uuids::uuid::variant_ncs }
            , { 0x50, boost::uuids::uuid::variant_ncs }
            , { 0x60, boost::uuids::uuid::variant_ncs }
            , { 0x70, boost::uuids::uuid::variant_ncs }
            , { 0x80, boost::uuids::uuid::variant_rfc_4122 }
            , { 0x90, boost::uuids::uuid::variant_rfc_4122 }
            , { 0xa0, boost::uuids::uuid::variant_rfc_4122 }
            , { 0xb0, boost::uuids::uuid::variant_rfc_4122 }
            , { 0xc0, boost::uuids::uuid::variant_microsoft }
            , { 0xd0, boost::uuids::uuid::variant_microsoft }
            , { 0xe0, boost::uuids::uuid::variant_future }
            , { 0xf0, boost::uuids::uuid::variant_future }
            };
        for (size_t i=0; i<sizeof(tests)/sizeof(Test); i++) {
            uuid u = { {} };
            u.data[8] = tests[i].octet7; // note that octet7 is array index 8

            BOOST_TEST_EQ(u.variant(), tests[i].variant);
        }
    }

     { // uuid::version()
        struct Test {
            unsigned char octet9;
            boost::uuids::uuid::version_type version;
        };
        const Test tests[] =
            { { 0x00, boost::uuids::uuid::version_unknown }
            , { 0x10, boost::uuids::uuid::version_time_based }
            , { 0x20, boost::uuids::uuid::version_dce_security }
            , { 0x30, boost::uuids::uuid::version_name_based_md5 }
            , { 0x40, boost::uuids::uuid::version_random_number_based }
            , { 0x50, boost::uuids::uuid::version_name_based_sha1 }
            , { 0x60, boost::uuids::uuid::version_unknown }
            , { 0x70, boost::uuids::uuid::version_unknown }
            , { 0x80, boost::uuids::uuid::version_unknown }
            , { 0x90, boost::uuids::uuid::version_unknown }
            , { 0xa0, boost::uuids::uuid::version_unknown }
            , { 0xb0, boost::uuids::uuid::version_unknown }
            , { 0xc0, boost::uuids::uuid::version_unknown }
            , { 0xd0, boost::uuids::uuid::version_unknown }
            , { 0xe0, boost::uuids::uuid::version_unknown }
            , { 0xf0, boost::uuids::uuid::version_unknown }
            };
        for (size_t i=0; i<sizeof(tests)/sizeof(Test); i++) {
            uuid u = {{0}};
            u.data[6] = tests[i].octet9; // note that octet9 is array index 8

            BOOST_TEST_EQ(u.version(), tests[i].version);
        }
    }

    { // uuid::swap(), swap()
        uuid u1 = {{0}};
        uuid u2 = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
        u1.swap(u2);

        unsigned char values1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        unsigned char values2[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        BOOST_TEST_UUID(u1, values2);
        BOOST_TEST_UUID(u2, values1);

        swap(u1, u2);
        BOOST_TEST_UUID(u1, values1);
        BOOST_TEST_UUID(u2, values2);
    }

    { // test comparison
        uuid u1 = {{0}};
        uuid u2 = {{1,0}};
        uuid u3 = {{255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}};
        uuid u4 = {{0,1,0}};
        uuid u5 = {{0,255,0}};
        uuid u6 = {{0, 0, 0, 0, 0, 0, 0, 0, 1}};
        uuid u7 = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};

        BOOST_TEST_EQ( u1, u1 );
        BOOST_TEST_EQ( u2, u2 );
        BOOST_TEST_EQ( u3, u3 );
        BOOST_TEST_EQ( u4, u4 );
        BOOST_TEST_EQ( u5, u5 );
        BOOST_TEST_EQ( u6, u6 );
        BOOST_TEST_EQ( u7, u7 );

        BOOST_TEST_NE( u1, u2 );
        BOOST_TEST_NE( u6, u7 );

        BOOST_TEST_LT( u1, u2 );
        BOOST_TEST_LT( u2, u3 );
        BOOST_TEST_LT( u1, u4);
        BOOST_TEST_LT( u1, u5 );
        BOOST_TEST_LT( u4, u5 );
        BOOST_TEST_LT( u4, u2 );
        BOOST_TEST_LT( u5, u2 );

        BOOST_TEST_LT( u1, u6 );
        BOOST_TEST_LT( u1, u7 );
        BOOST_TEST_LT( u7, u6 );
        BOOST_TEST_LT( u6, u4 );
        BOOST_TEST_LT( u7, u4 );

        BOOST_TEST_LE( u1, u1 );
        BOOST_TEST_LE( u1, u2 );
        BOOST_TEST_LE( u2, u3 );

        BOOST_TEST_LE( u1, u6 );
        BOOST_TEST_LE( u1, u7 );
        BOOST_TEST_LE( u6, u6 );
        BOOST_TEST_LE( u7, u6 );
        BOOST_TEST_LE( u6, u4 );
        BOOST_TEST_LE( u7, u4 );

        BOOST_TEST_GE( u1, u1 );
        BOOST_TEST_GE( u2, u2 );
        BOOST_TEST_GE( u3, u3 );

        BOOST_TEST_GE( u3, u2 );
        BOOST_TEST_GE( u2, u1 );
        BOOST_TEST_GE( u3, u1 );

        BOOST_TEST_GE( u6, u1 );
        BOOST_TEST_GE( u7, u1 );
        BOOST_TEST_GE( u6, u6 );
        BOOST_TEST_GE( u6, u7 );
        BOOST_TEST_GE( u4, u6 );
        BOOST_TEST_GE( u4, u7 );

#if defined(BOOST_UUID_HAS_THREE_WAY_COMPARISON)

        constexpr auto eq = std::strong_ordering::equal;
        constexpr auto lt = std::strong_ordering::less;
        constexpr auto gt = std::strong_ordering::greater;

        BOOST_TEST( ( u1 <=> u1 ) == eq );
        BOOST_TEST( ( u2 <=> u2 ) == eq );
        BOOST_TEST( ( u3 <=> u3 ) == eq );
        BOOST_TEST( ( u4 <=> u4 ) == eq );
        BOOST_TEST( ( u5 <=> u5 ) == eq );
        BOOST_TEST( ( u6 <=> u6 ) == eq );
        BOOST_TEST( ( u7 <=> u7 ) == eq );

        BOOST_TEST( ( u1 <=> u2 ) == lt );
        BOOST_TEST( ( u2 <=> u3 ) == lt );
        BOOST_TEST( ( u1 <=> u4 ) == lt );
        BOOST_TEST( ( u1 <=> u5 ) == lt );
        BOOST_TEST( ( u4 <=> u5 ) == lt );
        BOOST_TEST( ( u4 <=> u2 ) == lt );
        BOOST_TEST( ( u5 <=> u2 ) == lt );

        BOOST_TEST( ( u1 <=> u6 ) == lt );
        BOOST_TEST( ( u1 <=> u7 ) == lt );
        BOOST_TEST( ( u7 <=> u6 ) == lt );
        BOOST_TEST( ( u6 <=> u4 ) == lt );
        BOOST_TEST( ( u7 <=> u4 ) == lt );

        BOOST_TEST( ( u2 <=> u1 ) == gt );
        BOOST_TEST( ( u3 <=> u1 ) == gt );
        BOOST_TEST( ( u3 <=> u2 ) == gt );

        BOOST_TEST( ( u6 <=> u1 ) == gt );
        BOOST_TEST( ( u7 <=> u1 ) == gt );
        BOOST_TEST( ( u6 <=> u7 ) == gt );
        BOOST_TEST( ( u4 <=> u6 ) == gt );
        BOOST_TEST( ( u4 <=> u7 ) == gt );

#endif
    }

    { // ticket 10510
        // the uuids in the report
        uuid u6 = {{0x14,0x5c,0xfc,0x95,0x80,0x50,0x45,0x5a,0x83,0x82,0x44,0xca,0x57,0xc1,0x48,0x3b}};
        uuid u7 = {{0x14,0x5c,0xfc,0x95,0x80,0x50,0x45,0x5a,0x83,0x82,0x44,0xca,0x57,0xc1,0x48,0x3c}};

        // simple uuids to reproduce problem
        uuid u8 = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}};
        uuid u9 = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2}};
        uuid u10 = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,254}};
        uuid u11 = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255}};

        // some additional uuids for testing boundary cases
        uuid u12 = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1}};
        uuid u13 = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,2}};
        uuid u14 = {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}};
        uuid u15 = {{255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255}};

        BOOST_TEST(u6 < u7);
        BOOST_TEST(!(u7 < u6));

        BOOST_TEST(u8 < u9);
        BOOST_TEST(!(u9 < u8));
        BOOST_TEST(u8 < u10);
        BOOST_TEST(!(u10 < u8));
        BOOST_TEST(u8 < u11);
        BOOST_TEST(!(u11 < u8));

        BOOST_TEST(u9 < u10);
        BOOST_TEST(!(u10 < u9));
        BOOST_TEST(u9 < u11);
        BOOST_TEST(!(u11 < u9));

        BOOST_TEST(u10 < u11);
        BOOST_TEST(!(u11 < u10));

        BOOST_TEST(u12 < u13);
        BOOST_TEST(!(u13 < u12));
        BOOST_TEST(u14 < u15);
        BOOST_TEST(!(u15 < u14));
    }

    { // test hash
        uuid u1 = {{0}};
        uuid u2 = {{1,0}};
        uuid u3 = {{255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255}};

        boost::hash<uuid> uuid_hasher;

        BOOST_TEST_NE(uuid_hasher(u1), uuid_hasher(u2));
        BOOST_TEST_NE(uuid_hasher(u1), uuid_hasher(u3));
        BOOST_TEST_NE(uuid_hasher(u2), uuid_hasher(u3));
    }

    { // test is_pod
        BOOST_TEST_EQ(std::is_pod<uuid>::value, true);
    }

    return boost::report_errors();
}
