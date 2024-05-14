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
    TYPE_VIDEO_DECODE_TEST_UNIT,
    TYPE_VIDEO_SINGLETRACK_TEST_UNIT,
    TYPE_IMAGE_QA_TEST_UNIT,
    TYPE_VIDEO_MULTIHAND_GR_TEST_UNIT,
    TYPE_VIDEO_ST_GAZE_TEST_UNIT,
    TYPE_VIDEO_MULTITRACK_TEST_UNIT,
    TYPE_IMAGE_GAZE_TEST_UNIT,
    TYPE_VIDEO_ST_AGE_GENDER_TEST_UNIT,
    TYPE_VIDEO_ST_EXPR_TEST_UNIT,
    TYPE_VIDEO_ST_DB_TEST_UNIT,
    TYPE_VIDEO_ST_SPEC_OCC_TEST_UNIT,
    TYPE_VIDEO_MT_AGE_GENDER_TEST_UNIT,
    TYPE_VIDEO_MT_EXPR_TEST_UNIT,
    TYPE_VIDEO_MT_DB_TEST_UNIT,
    TYPE_VIDEO_DMS_ID_TEST_UNIT,
    TYPE_VIDEO_OMS_ID_TEST_UNIT,
    TYPE_VIDEO_DMS_ALL_TEST_UNIT,
    TYPE_IMAGE_OMS_ALL_TEST_UNIT,
    TYPE_IMAGE_DMS_ALL_TEST_UNIT,
    TYPE_VIDEO_DMS_GR_TEST_UNIT,
    TYPE_VIDEO_FACEDETECT_TEST_UNIT,
    TYPE_VIDEO_ST_HEMO_TEST_UNIT,
    TYPE_VIDEO_OMS_PD_TEST_UNIT,
    TYPE_IMAGE_OMS_PD_TEST_UNIT,
    TYPE_IMAGE_OMS_IMD_TEST_UNIT,
    TYPE_VIDEO_OMS_IMD_TEST_UNIT,
    TYPE_VIDEO_OMS_HEMO_TEST_UNIT,
    TYPE_IMAGE_DMS_3D_ALIGN_TEST_UNIT,
    TYPE_IMAGE_DMS_FACE_HEADPOSE_TEST_UNIT,
    TYPE_VIDEO_DMS_FACE_HEADPOSE_TEST_UNIT,
    TYPE_VIDEO_DMS_FACE_ALIGNMENT_TEST_UNIT,
    TYPE_VIDEO_OMS_BODY_ATTRIBUTION_TEST_UNIT,
    COUNT,
};

ContextClass<int, TestParams> create_strategys_context() {
    // strategy vedio decode
    std::string strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_DECODE_TEST_UNIT));
    std::function<int(TestParams)> func =
        std::bind(&test_video_decode, std::placeholders::_1);
    std::unique_ptr<LambdaStrategyClass<int, TestParams>> strategy =
        make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    ContextClass<int, TestParams> context(strategy_name, std::move(strategy));
    // strategy facedetect
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_FACEDETECT_TEST_UNIT));
    func = std::bind(&test_video_facedetect_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy singletrack
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_SINGLETRACK_TEST_UNIT));
    func = std::bind(&test_video_singletrack_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy image qa
    strategy_name =
        std::to_string(static_cast<int>(TestUnitType::TYPE_IMAGE_QA_TEST_UNIT));
    func = std::bind(&test_qa_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy multihand gr
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_MULTIHAND_GR_TEST_UNIT));
    func = std::bind(&test_video_multihand_gr_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video st gaze
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_ST_GAZE_TEST_UNIT));
    func = std::bind(&test_video_st_gaze_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy multitrack
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_MULTITRACK_TEST_UNIT));
    func = std::bind(&test_video_multitrack_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy image gaze
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_IMAGE_GAZE_TEST_UNIT));
    func = std::bind(&test_img_gaze_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video dms age gender
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_ST_AGE_GENDER_TEST_UNIT));
    func = std::bind(&test_video_st_age_gender_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video dms expr
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_ST_EXPR_TEST_UNIT));
    func = std::bind(&test_video_st_expr_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video dms db
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_ST_DB_TEST_UNIT));
    func = std::bind(&test_video_st_db_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video dms spec occ
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_ST_SPEC_OCC_TEST_UNIT));
    func = std::bind(&test_video_st_spec_occ_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video oms age gender
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_MT_AGE_GENDER_TEST_UNIT));
    func = std::bind(&test_video_mt_age_gender_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video oms expr
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_MT_EXPR_TEST_UNIT));
    func = std::bind(&test_video_mt_expr_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video oms db
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_MT_DB_TEST_UNIT));
    func = std::bind(&test_video_mt_db_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video dms item detect
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_DMS_ID_TEST_UNIT));
    func = std::bind(&test_video_dms_id_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));
    // strategy video oms item detect
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_OMS_ID_TEST_UNIT));
    func = std::bind(&test_video_oms_id_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video dms data dump
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_DMS_ALL_TEST_UNIT));
    func = std::bind(&test_video_dms_all_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy image oms all
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_IMAGE_OMS_ALL_TEST_UNIT));
    func = std::bind(&test_img_oms_all_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy image dms all
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_IMAGE_DMS_ALL_TEST_UNIT));
    func = std::bind(&test_img_dms_all_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video dms gr
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_DMS_GR_TEST_UNIT));
    func = std::bind(&test_video_dms_gr_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video dms hemo
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_ST_HEMO_TEST_UNIT));
    func = std::bind(&test_video_st_hemo_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video oms pd
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_OMS_PD_TEST_UNIT));
    func = std::bind(&test_video_oms_pd_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy image oms pd
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_IMAGE_OMS_PD_TEST_UNIT));
    func = std::bind(&test_img_oms_pd_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy image oms imd
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_IMAGE_OMS_IMD_TEST_UNIT));
    func = std::bind(&test_img_oms_imd_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video oms imd
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_OMS_IMD_TEST_UNIT));
    func = std::bind(&test_video_oms_imd_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video oms hemo
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_OMS_HEMO_TEST_UNIT));
    func = std::bind(&test_video_mt_hemo_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video dms align 3d
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_IMAGE_DMS_3D_ALIGN_TEST_UNIT));
    func = std::bind(&test_img_dms_3d_align_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy img dms face headpose
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_IMAGE_DMS_FACE_HEADPOSE_TEST_UNIT));
    func = std::bind(&test_img_faceheadpose_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video dms face headpose
    strategy_name = std::to_string(
        static_cast<int>(TestUnitType::TYPE_VIDEO_DMS_FACE_HEADPOSE_TEST_UNIT));
    func = std::bind(&test_video_face_headpose_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video dms face align
    strategy_name = std::to_string(static_cast<int>(
        TestUnitType::TYPE_VIDEO_DMS_FACE_ALIGNMENT_TEST_UNIT));
    func = std::bind(&test_video_face_alignment_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    // strategy video oms body attribution
    strategy_name = std::to_string(static_cast<int>(
        TestUnitType::TYPE_VIDEO_OMS_BODY_ATTRIBUTION_TEST_UNIT));
    func = std::bind(&test_video_mt_bodyattr_unit, std::placeholders::_1);
    strategy = make_unique_cust<LambdaStrategyClass<int, TestParams>>(func);
    context.setStrategy(strategy_name, std::move(strategy));

    return context;
}

int main(int argc, char **argv) {
    int ret = EXIT_SUCCESS;
    if (argc < 5) {
        std::cerr << "Usage " << argv[0]
                  << " -t TestUnitType(int) -i input_path(string) [-o "
                     "output_path(string)], at least 7 parameter need but only "
                  << argc << " params provided!" << std::endl;
        std::cerr << "-t [TYPE_VIDEO_DECODE_TEST_UNIT(1)] -i [input video "
                     "path] -o [output images path]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_SINGLETRACK_TEST_UNIT(2)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_IMAGE_QA_TEST_UNIT(3)] -i [input imagelist "
                     "path] -o [output image path]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_MULTIHAND_GR_TEST_UNIT(4)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_ST_GAZE_TEST_UNIT(5)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_MULTITRACK_TEST_UNIT(6)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_IMAGE_GAZE_TEST_UNIT(7)] -i [input imagelist "
                     "path] -o [output image path]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_ST_AGE_GENDER_TEST_UNIT(8)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_ST_EXPR_TEST_UNIT(9)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_ST_DB_TEST_UNIT(10)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_ST_SPEC_OCC_TEST_UNIT(11)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_MT_AGE_GENDER_TEST_UNIT(12)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_MT_EXPR_TEST_UNIT(13)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_MT_DB_TEST_UNIT(14)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_DMS_ID_TEST_UNIT(15)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_OMS_ID_TEST_UNIT(16)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr
            << "-t [TYPE_VIDEO_DMS_ALL_TEST_UNIT(17)] -i [input imagelist path]"
            << std::endl;
        std::cerr << "-t [TYPE_IMAGE_OMS_ALL_TEST_UNIT(18)] -i [input "
                     "imagelist path] -o [output image path]"
                  << std::endl;
        std::cerr << "-t [TYPE_IMAGE_DMS_ALL_TEST_UNIT(19)] -i [input "
                     "imagelist path] -o [output image path]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_DMS_GR_TEST_UNIT(20)] -i [input imagelist "
                     "path] -o [output image path]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_FACEDETECT_TEST_UNIT(21)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_ST_HEMO_TEST_UNIT(22)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_OMS_PD_TEST_UNIT(23)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_IMAGE_OMS_PD_TEST_UNIT(24)] -i [input videolist "
                     "path] -o [output video path] -e [efficiency mode(false)] "
                     "-l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_IMAGE_OMS_IMD_TEST_UNIT(25)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_OMS_IMD_TEST_UNIT(26)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_VIDEO_OMS_HEMO_TEST_UNIT(27)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_IMAGE_DMS_3D_ALIGN_TEST_UNIT(28)] -i [input "
                     "videolist path] -o [output video path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr << "-t [TYPE_IMAGE_DMS_FACE_HEADPOSE_TEST_UNIT(29)] -i "
                     "[input image path] -o [output image path] -e [efficiency "
                     "mode(false)] -l [loop count(1) -p [pool size(1)]]"
                  << std::endl;
        std::cerr
            << "-t [TYPE_VIDEO_DMS_FACE_HEADPOSE_TEST_UNIT(30)] -i [input "
               "videolist path] -o [output video path] -e [efficiency "
               "mode(false)] -l [loop count(1) -p [pool size(1)]]"
            << std::endl;
        std::cerr
            << "-t [TYPE_VIDEO_DMS_FACE_ALIGNMENT_TEST_UNIT(31)] -i [input "
               "videolist path] -o [output video path] -e [efficiency "
               "mode(false)] -l [loop count(1) -p [pool size(1)]]"
            << std::endl;
        std::cerr
            << "-t [TYPE_VIDEO_OMS_BODY_ATTRIBUTION_TEST_UNIT(32)] -i [input "
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