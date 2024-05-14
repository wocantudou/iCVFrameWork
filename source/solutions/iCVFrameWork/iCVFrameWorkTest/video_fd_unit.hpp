#pragma once
#include "test_type.h"
#include "video_common.hpp"
#include <iostream>
#include <sstream>
class FaceDetectCombInst : public CombInstBase {
  public:
    static int add_res(COMB_RES &res_and_path_map) {
        int ret = ICVBASE_NO_ERROR;
        res_and_path_map.clear();
        // resource real path
        const char *fd_dms_npu_res_path = NULL;
        res_and_path_map.insert(std::pair<RESTYPE, const char *>(
            fd_dms_npu_res_type, fd_dms_npu_res_path));

        // 2.add resourses  Just call once
        ret = add_all_res_you_need(res_and_path_map);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "add_all_res_you_need error ..." << std::endl;
            return ret;
        }
        return ret;
    }

    static int del_res(COMB_RES &res_and_path_map) {
        int ret = ICVBASE_NO_ERROR;
        ret = del_all_res_you_add(res_and_path_map);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "del_all_res_you_add error ..." << std::endl;
            return ret;
        }
        res_and_path_map.clear();
        return ret;
    }

  public:
    int creat_inst(COMB_INST &res_inst_map) {
        int ret = ICVBASE_NO_ERROR;
        ICV_BASE_INST fd_dms_npu_inst = NULL;
        res_inst_map.clear();
        res_inst_map.insert(std::pair<RESTYPE, ICV_BASE_INST>(
            fd_dms_npu_res_type, fd_dms_npu_inst));

        // 3.create inst
        ret = create_all_inst_you_need(res_inst_map);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "create_all_inst_you_need error ..." << std::endl;
            return ret;
        }
        return ret;
    }

    int destroy_inst(COMB_INST &res_inst_map) {
        int ret = ICVBASE_NO_ERROR;
        ret = destroy_all_inst_you_create(res_inst_map);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "destroy_all_inst_you_create error ..." << std::endl;
        }
        res_inst_map.clear();
        return ret;
    }
};

int do_facedetect(std::shared_ptr<InstPoolClass<COMB_INST>> &inst_pool,
                  std::string input_video_path, std::string output_video_path,
                  bool efficiency_mode, int loop_cnt) {
    int ret = ICVBASE_NO_ERROR;
    std::cout << "#######thread id = " << std::this_thread::get_id()
              << std::endl;
    RESTYPE res_type = RESTYPE_NONE;
    ICVFaces face_targets;
    // face_targets.face_idx = 0;
    // face_targets.face_num = 1;
    // Decode
    if (input_video_path.empty()) {
        std::cerr << "empty input path" << std::endl;
        return EXIT_FAILURE;
    }
    cv::VideoCapture cap(input_video_path);

    // 检查是否成功打开
    if (!cap.isOpened()) {
        std::cerr << "Error: Failed to open video file." << std::endl;
        return EXIT_FAILURE;
    }

    // 获取视频帧数、宽度和高度
    int frame_count = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    int fps = static_cast<int>(cap.get(cv::CAP_PROP_FPS));
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::cout << "Video has " << frame_count << " frames, "
              << "fps " << fps << ", "
              << "width = " << width << ", "
              << "height = " << height << std::endl;

    FaceDetectCombInst comb_inst;

    // Encode
    cv::VideoWriter writer;
    // cv::Size fd_frame_size = cv::Size(DMS_ST_W, DMS_ST_H);
    cv::Size fd_frame_size = cv::Size(640, 480);
    // cv::Size write_frame_size = cv::Size(width, height);//1080*1080
    // cv::Size write_frame_size = cv::Size(640, 480);
    cv::Size write_frame_size = cv::Size(360, 360);
    auto res_inst_map = inst_pool->acquire();
    // RAII{
    auto guard = iCVMakeScopeGuardImpl(std::bind([&]() {
        writer.release();
        cap.release();
        inst_pool->release(res_inst_map);
        // std::cout << "RAII release res" << std::endl;
    }));
    // RAII}

    // Motion-JPEG 编码器，每帧都是 JPEG 图像 保存 到avi文件，不支持mp4
    // writer.open(output_video_path, cv::VideoWriter::fourcc('M','J','P','G'),
    // fps, size); //fat avi 要将视频保存为 MP4 格式，通常可以使用 H.264 或
    // MPEG-4 Part 2 编码器进行压缩 writer.open(output_video_path,
    // cv::VideoWriter::fourcc('X','V','I','D'), fps, size); //avi
    writer.open(output_video_path, cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                fps, write_frame_size); // both avi and mp4

    int frame_idx = 0;
    cv::Mat frame;
    while (cap.read(frame)) {
        // face detect
        res_type = fd_dms_npu_res_type;
        if (res_inst_map->end() == res_inst_map->find(res_type)) {
            std::cerr << "cannot find restype = " << res_type
                      << " from res_inst_map ..." << std::endl;
            return ret;
        }
        ICV_BASE_INST &fd_dms_npu_inst = res_inst_map->at(res_type);
        cv::Mat frame_fd;
        // M32 preprocess
        // cv::resize(frame, frame_fd, fd_frame_size);
        // T1E/M1E preprocess
        // cv::resize(frame, frame_fd, cv::Size(640, 360));
        // cv::copyMakeBorder(frame_fd, frame_fd, 60, 60, 0, 0,
        // cv::BORDER_CONSTANT, cv::Scalar{0, 0, 0});

        // M32处理方式    =>
        // 这里是因为数据未裁剪，当实际上车时，只需要resize就行了 cv::Rect roi;
        // roi.x = 420;//ROI起点x坐标
        // roi.y = 0;//ROI起点y坐标
        // roi.width = 1080;//ROI宽度
        // roi.height = 1080;//ROI高度
        // frame_fd = frame(roi).clone();
        cv::resize(frame, frame_fd, cv::Size(360, 360));

        ICVBaseDataIn face_data_in;
        face_data_in.data_stat = ICV_BASE_DATA_STAT_IMG;
        face_data_in.img.h = frame_fd.rows;
        face_data_in.img.w = frame_fd.cols;
        face_data_in.img.fmt = ICV_BASE_IMG_FMT_BGR;
        face_data_in.img.data = (char *)frame_fd.data;
        face_data_in.img.data_byte_size = frame_fd.rows * frame_fd.cols * 3;
        face_data_in.img.next = NULL;
        // ret = comb_inst.get_encrypt_module_result(efficiency_mode, loop_cnt,
        // res_type, fd_dms_npu_inst, (void *)&face_data_in, (void
        // *)&face_targets, sizeof(face_targets));
        ret = comb_inst.get_module_result(
            efficiency_mode, loop_cnt, res_type, fd_dms_npu_inst,
            (void *)&face_data_in, (void *)&face_targets);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "get_module_result = " << res_type << " error ..."
                      << std::endl;
            return ret;
        }

        cv::resize(frame, frame, write_frame_size);
        if (face_targets.face_num > 0) {
            draw_rect_and_points(frame_fd, face_targets,
                                 IFLY_DSM_DMS_TYPE::IFLY_DSM_DMS_TYPE_FACES);
        } else {
            std::cout << "no face detect" << std::endl;
        }
        if (0 == frame_idx % 1) {
            std::cout << input_video_path
                      << "-> progress --- frame idx : " << frame_idx
                      << std::endl;
        }
        frame_idx++;
        std::string output_img_path =
            "./test_dms_st/out_image_fd/" + std::to_string(frame_idx) + ".jpg";
        cv::imwrite(output_img_path, frame_fd);
        writer << frame_fd;
    }
    std::cout << input_video_path
              << "-> progress 100% --- frame idx : " << frame_idx++
              << std::endl;
    return ret;
}

int test_video_facedetect_unit(TestParams test_params) {
    int ret = ICVBASE_NO_ERROR;
    std::string &input_videolist_path = test_params.input_path;
    std::string &output_video_path = test_params.output_path;
    bool &efficiency_mode = test_params.efficiency_mode;
    int &loop_cnt = test_params.loop_cnt;
    int &pool_num = test_params.pool_size;
    // 1.Just need call once
    ret = FaceDetectCombInst::init_engine();
    if (ICVBASE_NO_ERROR != ret) {
        std::cerr << "init_engine failed" << std::endl;
        return ret;
    }

    // 2.add res
    COMB_RES res_and_path_map;
    res_and_path_map.clear();
    ret = FaceDetectCombInst::add_res(res_and_path_map);
    if (ICVBASE_NO_ERROR != ret) {
        std::cerr << "add_res failed" << std::endl;
        return ret;
    }

    std::cout << "input_videolist_path : " << input_videolist_path << std::endl;
    std::cout << "output_video_path : " << output_video_path << std::endl;
    std::cout << "pool_num : " << pool_num << std::endl;

    if (g_delimiter != output_video_path.back()) {
        output_video_path += g_delimiter;
    }
    std::vector<std::string> file_list_vec =
        read_file_list(input_videolist_path);
    // inst pool {
    auto inst_pool = std::make_shared<InstPoolClass<COMB_INST>>(pool_num);
    for (int i = 0; i < pool_num; ++i) {
        FaceDetectCombInst comb_inst;
        auto inst = inst_pool->acquire();
        // std::cout << "avail_insts_num " << inst_pool->avail_insts_num() <<
        // std::endl; std::cout << "inst = " << inst << std::endl;
        ret = comb_inst.creat_inst(*inst);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "creat_inst error ..." << std::endl;
            inst_pool->release(inst);
            return ret;
        }
        inst_pool->release(inst);
    }
    // inst pool }

    // system("showmem -t mm_dma");
    // return ret;

    // RAII{
    auto guard = iCVMakeScopeGuardImpl(std::bind([&]() {
        // 5. delete the inst
        for (int i = 0; i < pool_num; ++i) {
            FaceDetectCombInst comb_inst;
            auto inst = inst_pool->acquire();
            ret = comb_inst.destroy_inst(*inst);
            if (ICVBASE_NO_ERROR != ret) {
                std::cerr << "destroy_inst error ..." << std::endl;
            }
            inst_pool->release(inst);
        }
        // 6. delete the res
        ret = FaceDetectCombInst::del_res(res_and_path_map);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "del_res failed" << std::endl;
        }
        res_and_path_map.clear();
        // 7.Just need call once
        ret = FaceDetectCombInst::fini_engine();
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "fini_engine failed" << std::endl;
        }
    }));
    // RAII}

    auto thread_pool = std::make_shared<ThreadPoolClass>(pool_num);
    std::vector<std::future<int>> results;
    // 3.do task
    for (int i = 0; i < file_list_vec.size(); ++i) {
        auto tokens = iCVSplitImpl(file_list_vec.at(i), g_delimiter_str);
        std::string output_realpath =
            output_video_path + tokens.back() + g_delimiter;
        if (!iCVFileExistsImpl(output_realpath)) {
            if (!iCVCreateFoldersImpl(output_realpath)) {
                std::cerr << "create folder : " << output_realpath << " failed"
                          << std::endl;
                continue;
            }
        }
        results.emplace_back(thread_pool->enqueue(
            std::bind(do_facedetect, inst_pool, file_list_vec.at(i),
                      output_realpath + tokens.back() + "output.mp4",
                      efficiency_mode, loop_cnt)));
        std::cout << "-------------------------------------- : " << i
                  << std::endl;
    }

    int task_idx = 0;
    for (auto &&result : results) {
        std::cout << "task idx : " << task_idx << " --- ret = " << result.get()
                  << std::endl;
        task_idx++;
    }
    return ret;
}