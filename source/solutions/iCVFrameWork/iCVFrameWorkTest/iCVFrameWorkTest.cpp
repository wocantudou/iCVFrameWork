#include "option_parser.hpp"
#include "video_fd_unit.hpp"
#include <iostream>

// #include "image_fd_unit.hpp"

#if __cplusplus >= 201402L // C++14 and beyond
// using std::enable_if_t;
using std::make_unique;
using make_unique_cust = std::make_unique;
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique_cust(Args &&...args) {
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

enum class TestUnitType : int {
    NONE,
    TYPE_VIDEO_FACEDETECT_TEST_UNIT,
    COUNT,
};

ContextClass<int, TestParams> create_strategys_context() {
    // strategy vedio decode
    std::string strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_FACEDETECT_TEST_UNIT));
    std::function<int(TestParams)> func =
        std::bind(&test_video_facedetect_unit, std::placeholders::_1);
    std::unique_ptr<LambdaStrategyClass<int, TestParams>> strategy =
        make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    ContextClass<int, TestParams> context(strategy_name, std::move(strategy));

    // // strategy facedetect
    // strategy_name = std::to_string(
    //     static_cast<int>(TestUnitType::TYPE_VIDEO_FACEDETECT_TEST_UNIT));
    // func = std::bind(&test_video_facedetect_unit, std::placeholders::_1);
    // strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    // context.setStrategy(strategy_name, std::move(strategy));

    return context;
}

int main(int argc, char **argv) {
    int ret = EXIT_SUCCESS;
    if (argc < 5) {
        std::cerr << "Usage " << argv[0]
                  << " -t TestUnitType(int) -i input_path(string) [-o "
                     "output_path(string)], at least 7 parameter need but only "
                  << argc << " params provided!" << std::endl;
        std::cerr << "-t [TYPE_VIDEO_FACEDETECT_TEST_UNIT(1)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        return EXIT_FAILURE;
    }

    TestParams test_params;
    OptionParserClass parser(argc, argv);
    parser.add_option('t', test_params.test_unit_type);
    parser.add_option('i', test_params.input_path);
    parser.add_option('o', test_params.output_path);
    parser.add_option('e', test_params.efficiency_mode);
    parser.add_option('l', test_params.loop_cnt);
    parser.add_option('p', test_params.pool_size);
    parser.parse();
    if (static_cast<TestUnitType>(test_params.test_unit_type) >=
            TestUnitType::COUNT ||
        static_cast<TestUnitType>(test_params.test_unit_type) <=
            TestUnitType::NONE) {
        std::cerr << "not support Test unit type" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "test_unit_type: " << test_params.test_unit_type << std::endl;
    std::cout << "input_path: " << test_params.input_path << std::endl;
    std::cout << "output_path: " << test_params.output_path << std::endl;
    std::cout << "efficiency_mode: " << test_params.efficiency_mode
              << std::endl;
    std::cout << "loop_cnt: " << test_params.loop_cnt << std::endl;
    std::cout << "pool size: " << test_params.pool_size << std::endl;

    auto context = create_strategys_context();
    std::string strategy_name = std::to_string(test_params.test_unit_type);
    ret = context.execute(strategy_name, test_params);
    if (EXIT_SUCCESS != ret) {
        std::cerr << "do test unit : " << test_params.test_unit_type << " error"
                  << std::endl;
    }
    return ret;
}