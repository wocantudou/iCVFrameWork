#pragma once
#include "dp_strategy.hpp"
#include "dynamic_loader.hpp"
#include "file_utils.hpp"
#include "iCVFrameWork_api.h"
#include "inst_pool.hpp"
#include "opencv2/opencv.hpp"
#include "raii_scope_guard.hpp"
#include "string_utils.hpp"
#include "thread_pool.hpp"
#include "time_utils.hpp"
#include <map>
#include <string>
#include <thread>

#ifdef WIN32
static char g_delimiter = '\\';
static std::string g_delimiter_str = "\\";
#else
static char g_delimiter = '/';
static std::string g_delimiter_str = "/";
#endif

API_LIST_FW(DeclareApi);

static std::map<RESTYPE, const std::string> res_map = {
    {RESTYPE_FACE_DETECT_CPU, "RESTYPE_FACE_DETECT_CPU"},
    {RESTYPE_FACE_DETECT_NPU, "RESTYPE_FACE_DETECT_NPU"},
    {RESTYPE_FACE_ALIGNMENT_CPU, "RESTYPE_FACE_ALIGNMENT_CPU"},
    {RESTYPE_FACE_ALIGNMENT_NPU, "RESTYPE_FACE_ALIGNMENT_NPU"},
    {MODTYPE_FACE_SINGLETRACK_CPU, "MODTYPE_FACE_SINGLETRACK_CPU"},
    {MODTYPE_FACE_SINGLETRACK_NPU, "MODTYPE_FACE_SINGLETRACK_NPU"}};

#define COUT_TIME(RES_TYPE, TIME)                                              \
    printf("iCVFrameWorkGetResult_ %s during time = %lf ms\n",                 \
           res_map[RES_TYPE].c_str(), TIME);
#define COMB_RES std::map<RESTYPE, const char *>
#define COMB_INST std::map<RESTYPE, ICV_BASE_INST>
#define CACHE_QUEUE_SIZE 200
#define X_MIN_SCALE 1
#define X_SHIFT 100
#define Y_MIN_SCALE 1
#define Y_SHIFT 100
#define Y_AMPLITUDE 100

#define DMS_ST_W 360
#define DMS_ST_H 360

#define OMS_DETECT_W 512
#define OMS_DETECT_H 288

#define OMS_WRITE_W 1280
#define OMS_WRITE_H 720

// resource type
RESTYPE fd_dms_npu_res_type = RESTYPE_FACE_DETECT_CPU;

int image2cv(std::string input_image, cv::Mat &frame) {
    int ret = ICVBASE_NO_ERROR;
    frame.release();
    // read image
    frame = cv::imread(input_image.c_str());
    if (frame.empty()) {
        std::cout << "imread error " << input_image << std::endl;
        return ICVBASE_FILE_OPERATION_ERROR;
    }
    return ret;
}

std::vector<std::string> read_file_list(std::string file_list) {
    std::ifstream infile(file_list);
    std::string line;
    std::vector<std::string> file_list_vec;
    while (std::getline(infile, line)) {
        file_list_vec.push_back(line);
    }
    return file_list_vec;
}

int select_max_face(ICVFaces &face_targets) {
    int max_index = -1;
    int area = 0;
    if (1 == face_targets.face_num) {
        return 0;
    }
    for (int i = 0; i < face_targets.face_num; i++) {
        int area_temp = face_targets.faces[i].face_rect_with_info.rect.w *
                        face_targets.faces[i].face_rect_with_info.rect.h;
        if (area_temp > area) {
            max_index = i;
            area = area_temp;
        }
    }
    return max_index;
}

void draw_face(cv::Mat &frame, ICVFaces &face_targets) {
    ICVFace &face_target = face_targets.faces[face_targets.face_num];
    cv::Rect draw_rect;
    draw_rect.x = face_target.face_rect_with_info.rect.x;
    draw_rect.y = face_target.face_rect_with_info.rect.y;
    draw_rect.height = face_target.face_rect_with_info.rect.h;
    draw_rect.width = face_target.face_rect_with_info.rect.w;
    std::string str_face_score =
        std::to_string(face_target.face_rect_with_info.conf_score);
    std::string str_face_id =
        std::to_string(face_target.face_rect_with_info.id);

    cv::rectangle(frame, draw_rect, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
    cv::putText(frame, str_face_score, cv::Point(draw_rect.x, draw_rect.y - 10),
                cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    cv::putText(frame, str_face_id,
                cv::Point(draw_rect.x + draw_rect.width / 2, draw_rect.y - 30),
                cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
}

void draw_faces(cv::Mat &frame, ICVFaces &face_targets) {
    for (int face_index = 0; face_index < face_targets.face_num; face_index++) {
        ICVFace &face_target = face_targets.faces[face_index];
        cv::Rect draw_rect;
        draw_rect.x = face_target.face_rect_with_info.rect.x;
        draw_rect.y = face_target.face_rect_with_info.rect.y;
        draw_rect.height = face_target.face_rect_with_info.rect.h;
        draw_rect.width = face_target.face_rect_with_info.rect.w;
        std::string str_face_score =
            std::to_string(face_target.face_rect_with_info.conf_score);
        cv::rectangle(frame, draw_rect, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
        cv::putText(frame, str_face_score,
                    cv::Point(draw_rect.x, draw_rect.y - 10),
                    cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    }
}

enum class IFLY_DSM_DMS_TYPE : int {
    IFLY_DSM_DMS_TYPE_NONE,
    IFLY_DSM_DMS_TYPE_FACE,
    IFLY_DSM_DMS_TYPE_FACES,
    IFLY_DSM_DMS_TYPE_COUNT,
};

void draw_rect_and_points(
    cv::Mat &frame, ICVFaces &face_targets,
    IFLY_DSM_DMS_TYPE dms_type = IFLY_DSM_DMS_TYPE::IFLY_DSM_DMS_TYPE_FACE) {
    switch (dms_type) {
    case IFLY_DSM_DMS_TYPE::IFLY_DSM_DMS_TYPE_FACE:
        draw_face(frame, face_targets);
        break;
    case IFLY_DSM_DMS_TYPE::IFLY_DSM_DMS_TYPE_FACES:
        draw_faces(frame, face_targets);
        break;
    default:
        std::cerr << "dms_type = " << static_cast<int>(dms_type)
                  << " not support!" << std::endl;
        break;
    }
}

class CombInstBase {
  public:
    CombInstBase() {}
    virtual ~CombInstBase() {}

  public:
    static int init_engine() {
        API_LIST_FW(GetProcApiVar);
        int ret = ICVBASE_NO_ERROR;

        constexpr int max_len = 128;
        const char *engine_version = "";
        ret = iCVFrameWorkGetEngineVersion_(&engine_version);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "iCVFrameWorkGetEngineVersion  failed..." << std::endl;
            return ret;
        }

        std::string cfg_path = "../../cfg/iCVFrameWork/iCVFrameWork.cfg";
        ret = iCVFrameWorkInitialize_(cfg_path.c_str(), NULL);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "iCVFrameWorkInitialize_ failed" << std::endl;
            return ret;
        }
        std::cout << "iCVFrameWorkInitialize successful ..." << std::endl;

        const char *expired_time = "";
        ret = iCVFrameWorkGetExpiredTime_(&expired_time);
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "iCVFrameWorkGetExpiredTime_ error ..." << std::endl;
            return ret;
        }
        std::cout << "##########Engine expired time = " << expired_time
                  << std::endl;
        return ret;
    }

    static int add_all_res_you_need(COMB_RES &res_and_path_map) {
        int ret = ICVBASE_NO_ERROR;
        for (auto &res_and_path : res_and_path_map) {
            RESTYPE res_type = res_and_path.first;
            const char *res_path = res_and_path.second;
            ret = iCVFrameWorkResourceAdd_(res_type, res_path);
            if (ICVBASE_NO_ERROR != ret) {
                std::cerr << "iCVFrameWorkResourceAdd_ " << res_type
                          << " failed, and res_path = " << res_path
                          << std::endl;
                return ret;
            }
            std::cout
                << "################################iCVFrameWorkResourceAdd_ "
                << res_type << " successful !" << std::endl;
        }

        return ret;
    }

    static int create_all_inst_you_need(COMB_INST &res_inst_map) {
        int ret = ICVBASE_NO_ERROR;
        for (auto &res_inst : res_inst_map) {
            RESTYPE res_type = res_inst.first;
            ICV_BASE_INST &ifly_dsm_inst = res_inst.second;
            ret = iCVFrameWorkCreateInst_(&ifly_dsm_inst, res_type);
            if (ICVBASE_NO_ERROR != ret) {
                std::cerr << "iCVFrameWorkCreateInst_ " << res_type
                          << " failed...\n";
                return ret;
            }
            std::cout
                << "################################iCVFrameWorkCreateInst_ "
                << res_type << " --- " << ifly_dsm_inst << " successful !\n";
        }

        return ret;
    }

    static int destroy_all_inst_you_create(COMB_INST &res_inst_map) {
        int ret = ICVBASE_NO_ERROR;
        for (auto &res_inst : res_inst_map) {
            RESTYPE res_type = res_inst.first;
            ICV_BASE_INST &ifly_dsm_inst = res_inst.second;
            // Face Destroy Inst
            ret = iCVFrameWorkDestroyInst_(&ifly_dsm_inst);
            if (ICVBASE_NO_ERROR != ret) {
                std::cerr << "iCVFrameWorkDestroyInst_ " << res_type
                          << " failed..." << std::endl;
                return ret;
            }
            std::cout
                << "################################iCVFrameWorkDestroyInst_ "
                << res_type << " successful !" << std::endl;
        }
        return ret;
    }

    static int del_all_res_you_add(COMB_RES &res_and_path_map) {
        int ret = ICVBASE_NO_ERROR;
        for (auto &curr_res : res_and_path_map) {
            // delete the common resource
            ret = iCVFrameWorkResourceDelete_(curr_res.first);
            if (ICVBASE_NO_ERROR != ret) {
                std::cout << "iCVFrameWorkResourceDelete_ " << curr_res.first
                          << " failed..." << std::endl;
                return ret;
            }
            std::cout << "################################"
                         "iCVFrameWorkResourceDelete_ "
                      << curr_res.first << " successful !" << std::endl;
        }
        return ret;
    }

    int reset(ICV_BASE_INST &ifly_dsm_inst) {
        int ret = ICVBASE_NO_ERROR;
        ret = iCVFrameWorkResetInst_(ifly_dsm_inst);
        return ret;
    }

    int get_module_result(bool efficiency_mode, int loop_cnt, RESTYPE &res_type,
                          ICV_BASE_INST &ifly_dsm_inst, void *data_in,
                          void *data_out) {
        int ret = ICVBASE_NO_ERROR;
        if (!efficiency_mode) {
            loop_cnt = 1;
        }
        if (loop_cnt <= 0) {
            loop_cnt = 500;
        }

        double dur_tim = 0.;
        for (int i = 0; i < loop_cnt; ++i) {
            double begin_tim = iCVMsTickCountImpl();
            ret = iCVFrameWorkGetResult_(ifly_dsm_inst, data_in, data_out);
            if (ICVBASE_NO_ERROR != ret) {
                std::cerr << "threadid : " << std::this_thread::get_id()
                          << " -- iCVFrameWorkGetResult_ " << res_type
                          << " Failed" << std::endl;
                return ret;
            }
            double end_tim = iCVMsTickCountImpl();
            dur_tim += end_tim - begin_tim;
        }

        if (efficiency_mode) {
            COUT_TIME(res_type, dur_tim / loop_cnt);
        }
        return ret;
    }

    static int fini_engine() {
        int ret = ICVBASE_NO_ERROR;
        ret = iCVFrameWorkUninitialize_();
        if (ICVBASE_NO_ERROR != ret) {
            std::cerr << "iCVFrameWorkUninitialize_ failed" << std::endl;
            return ret;
        }
        std::cout << "iCVFrameWorkUninitialize_ successful ..." << std::endl;
        return ret;
    }
};
