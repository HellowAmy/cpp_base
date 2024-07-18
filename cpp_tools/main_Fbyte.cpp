#include "../include/Tvlog.h"
#include "../include/Fbyte.h"

#include <vector>
#include <list>

#include <chrono>
#include <iostream>
#include <string>
#include <cstring>



void test_1()
{
    {
        bool ok1 = Fbyte::is_hex_letter_big('A');
        bool ok2 = Fbyte::is_hex_letter_big('a');
        bool ok3 = Fbyte::is_hex_letter_big('1');
        vlogd($(ok1) $(ok2) $(ok3));
    }
    {
        bool ok1 = Fbyte::is_hex_letter_little('A');
        bool ok2 = Fbyte::is_hex_letter_little('a');
        bool ok3 = Fbyte::is_hex_letter_little('1');
        vlogd($(ok1) $(ok2) $(ok3));
    }
    {
        bool ok1 = Fbyte::is_hex_digit('A');
        bool ok2 = Fbyte::is_hex_digit('a');
        bool ok3 = Fbyte::is_hex_digit('1');
        vlogd($(ok1) $(ok2) $(ok3));
    }
    {
        bool ok1 = Fbyte::is_hex_range('A');
        bool ok2 = Fbyte::is_hex_range('a');
        bool ok3 = Fbyte::is_hex_range('1');
        bool ok4 = Fbyte::is_hex_range('#');
        vlogd($(ok1) $(ok2) $(ok3) $(ok4));
    }
    {
        std::string s1 = Fbyte::to_sbyte(0x00);
        std::string s2 = Fbyte::to_sbyte(0xff);
        std::string s3 = Fbyte::to_sbyte(0x01);
        std::string s4 = Fbyte::to_sbyte(0x02);
        std::string s5 = Fbyte::to_sbyte(0x04);
        std::string s6 = Fbyte::to_sbyte(0x08);
        vlogd($(s1) $(s2) $(s3) $(s4) $(s5) $(s6));

    }
    {
        char c1 = Fbyte::to_hex_right('0');
        char c2 = Fbyte::to_hex_right('9');
        char c3 = Fbyte::to_hex_right('a');
        char c4 = Fbyte::to_hex_right('f');
        char c5 = Fbyte::to_hex_right('A');
        char c6 = Fbyte::to_hex_right('F');
        std::string s1 = Fbyte::to_sbyte(c1);
        std::string s2 = Fbyte::to_sbyte(c2);
        std::string s3 = Fbyte::to_sbyte(c3);
        std::string s4 = Fbyte::to_sbyte(c4);
        std::string s5 = Fbyte::to_sbyte(c5);
        std::string s6 = Fbyte::to_sbyte(c6);
        vlogd($(s1) $(s2) $(s3) $(s4) $(s5) $(s6));
    }
    {
        char c1 = Fbyte::to_hex_left('0');
        char c2 = Fbyte::to_hex_left('9');
        char c3 = Fbyte::to_hex_left('a');
        char c4 = Fbyte::to_hex_left('f');
        char c5 = Fbyte::to_hex_left('A');
        char c6 = Fbyte::to_hex_left('F');
        std::string s1 = Fbyte::to_sbyte(c1);
        std::string s2 = Fbyte::to_sbyte(c2);
        std::string s3 = Fbyte::to_sbyte(c3);
        std::string s4 = Fbyte::to_sbyte(c4);
        std::string s5 = Fbyte::to_sbyte(c5);
        std::string s6 = Fbyte::to_sbyte(c6);
        vlogd($(s1) $(s2) $(s3) $(s4) $(s5) $(s6));
    }
    {
        char s1 = Fbyte::to_upper('1');
        char s2 = Fbyte::to_upper('a');
        char s3 = Fbyte::to_upper('F');
        vlogd($(s1) $(s2) $(s3));
    }
    {
        char s1 = Fbyte::to_lower('1');
        char s2 = Fbyte::to_lower('a');
        char s3 = Fbyte::to_lower('F');
        vlogd($(s1) $(s2) $(s3));
    }
    {
        std::string s = "123qweASD";
        std::string s1 = Fbyte::to_upper(s);
        std::string s2 = Fbyte::to_lower(s);

        vlogd($(s) $(s1) $(s2));
    }
    {
        char c1 = Fbyte::to_hex_byte('0','0');
        char c2 = Fbyte::to_hex_byte('4','c');
        char c3 = Fbyte::to_hex_byte('f','f');
        std::string s1 = Fbyte::to_sbyte(c1);
        std::string s2 = Fbyte::to_sbyte(c2);
        std::string s3 = Fbyte::to_sbyte(c3);
        vlogd($(s1) $(s2) $(s3));
    }
    {
        std::string c1 = Fbyte::sto_hex("04ff4c4ca");
        auto s1 = Fbyte::to_strbyte(c1);
        vlogd($(s1));
    }
    {
        auto s1 = Fbyte::to_num_multi(11,2);
        auto s2 = Fbyte::to_num_multi(10,2);
        auto s3 = Fbyte::to_num_multi(9,3);
        auto s4 = Fbyte::to_num_multi(10,3);
        auto s5 = Fbyte::to_num_multi(11,3);
        auto s6 = Fbyte::to_num_multi(12,3);
        vlogd($(s1) $(s2) $(s3) $(s4) $(s5) $(s6));
    }
    {
        int c1 = Fbyte::to_hex_right('f');
        auto s1 = Fbyte::Tto_sbyte<int>(c1);
        auto s2 = Fbyte::Tto_sbyte<int>(1);
        auto s3 = Fbyte::Tto_sbyte<int>(8);
        vlogd($(c1) $(s1) $(s2) $(s3));
    }
    {
        char x1 = Fbyte::to_hex_byte('0','0');
        char x2 = Fbyte::to_hex_byte('f','f');
        char x3 = Fbyte::to_hex_byte('4','c');
        char x4 = Fbyte::to_hex_byte('c','4');

        auto s1 = Fbyte::cto_shex(x1);
        auto s2 = Fbyte::cto_shex(x2);
        auto s3 = Fbyte::cto_shex(x3);
        auto s4 = Fbyte::cto_shex(x4);
        vlogd($(s1) $(s2) $(s3) $(s4));
    }
    {
        std::string c1 = Fbyte::sto_hex("04ff4c4cffececaa");
        std::string s1 = Fbyte::hto_hex(c1);
        std::string s2 = Fbyte::to_strbyte(c1);
        vlogd($(s1) $(s2));
    }
}

void test_2()
{
    {
        if(IS_BIG_ENDIAN)
        {
            vlogd("IS_BIG_ENDIAN");
        }
        if(IS_LITTLE_ENDIAN)
        {
            vlogd("IS_LITTLE_ENDIAN");
        }
    }
    {
        int v = 1920;
        auto s1 = Fbyte::Tto_sbyte<int>(v);
        vlogd($(s1));

    }
    {
        int v = 1920;
        auto s1 = Fbyte::Tto_sbyte_endian<int>(v);
        vlogd($(s1));
    }
    {
        int v = 1920;
        int ret = Fbyte::Tto_swap_endian(v);
        auto s1 = Fbyte::Tto_sbyte_endian(v);
        auto s2 = Fbyte::Tto_sbyte_endian(ret);
        vlogd($(s1) $(s2));
    }
    {
        short v = 3968;
        short ret = Fbyte::Tto_swap_endian(v);
        auto s1 = Fbyte::Tto_sbyte_endian(v);
        auto s2 = Fbyte::Tto_sbyte_endian(ret);
        vlogd($(s1) $(s2));
    }
    {
        short v = 3968;
        short ret = Fbyte::Tto_endian_net(v);
        auto s1 = Fbyte::Tto_sbyte_endian(v);
        auto s2 = Fbyte::Tto_sbyte_endian(ret);
        vlogd($(s1) $(s2));
    }
    {
        long long v = 67554046000496655;
        long long ret = Fbyte::Tto_swap_endian(v);
        auto s1 = Fbyte::Tto_sbyte_endian(v);
        auto s2 = Fbyte::Tto_sbyte_endian(ret);
        vlogd($(s1) $(s2));
    }
}

int main()
{
    Tvlogs::get()->set_level(vlevel4::e_info);

    vlogd("== begin ==");

    // test_1();
    test_2();
    // test_2();
    // test_3();
    // test_4();

    vlogd("== end ==");

    return 0;
}

