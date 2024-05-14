#ifndef __TEST_TYPE_H__
#define __TEST_TYPE_H__
#include <string>
typedef struct _TestParams
{
    int test_unit_type = 1;
    std::string input_path = "";
    std::string output_path = "";
    bool efficiency_mode = false;
    int loop_cnt = 1;
    int pool_size = 1;
}TestParams;

#endif //__TEST_TYPE_H__