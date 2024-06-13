#include "mmyolo_face_detect_with_engine.h"
#include "base_img_proc.h"
#include "iCVFaceDetect_cfg.h"
#include "nanodet_decode.hpp"
#include "nms.hpp"
#include <algorithm>
#include <fstream>
#include <numeric>
#include <stdio.h>

//+------------------------------------------------------+
//+         marco defines
//+------------------------------------------------------+
#define COORD_BIAS (1)
#define COORD_BOUND (5)
#define FACE_LABEL (0)

#define FD_MEAN_B (103.53f)
#define FD_MEAN_G (116.28f)
#define FD_MEAN_R (123.675f)
#define FD_VAR_B (57.375f)
#define FD_VAR_G (57.12f)
#define FD_VAR_R (58.395f)

#define INPUT_NODE_NAME ("images")
#define OUTPUT_NODE_NAME_ONE ("o0")
#define OUTPUT_NODE_NAME_TWO ("738")
#define OUTPUT_NODE_NAME_THREE ("746")

const std::map<const std::string, const int32_t>
    MmyoloFaceDetectWithEngine::fd_heads_strides_{{OUTPUT_NODE_NAME_ONE, 8},
                                                  {OUTPUT_NODE_NAME_TWO, 16},
                                                  {OUTPUT_NODE_NAME_THREE, 32}};

int32_t MmyoloFaceDetectWithEngine::reset() {
    int32_t ret = ICVBASE_NO_ERROR;
    return ret;
}

int32_t MmyoloFaceDetectWithEngine::load_image_static(const cv::Mat &img_mat,
                                                      int32_t model_input_w,
                                                      int32_t model_input_h,
                                                      cv::Mat &resize_mat) {
    int32_t ret = ICVBASE_NO_ERROR;
    int32_t src_height = img_mat.rows;
    int32_t src_width = img_mat.cols;
    int32_t max_src_size = (std::max)(src_width, src_height);
    int32_t max_dst_size = (std::max)(model_input_w, model_input_h);
    float scale_ratio = float(max_dst_size) / max_src_size;
    if (scale_ratio != 1) {
        if (scale_ratio < 1) {
            cv::resize(img_mat, resize_mat,
                       cv::Size(int32_t(scale_ratio * src_width),
                                int32_t(scale_ratio * src_height)),
                       0, 0, 3);
        } else {
            cv::resize(img_mat, resize_mat,
                       cv::Size(int32_t(scale_ratio * src_width),
                                int32_t(scale_ratio * src_height)),
                       0, 0, 1);
        }
    } else {
        resize_mat = img_mat.clone();
    }
    return ret;
}

int32_t MmyoloFaceDetectWithEngine::letterbox(
    const cv::Mat &src_img, int32_t img_size_w, int32_t img_size_h,
    cv::Mat &dst_img, ResizedPaddingInfo &resize_padding_info) {
    int32_t ret = ICVBASE_NO_ERROR;
    int32_t src_height = src_img.rows;
    int32_t src_width = src_img.cols;

    float scale_ratio = (std::min)(float(img_size_h) / src_height,
                                   float(img_size_w) / src_width);
    scale_ratio = (std::min)(scale_ratio, 1.0f);
    int32_t new_pad_w = int32_t(round(scale_ratio * src_width));
    int32_t new_pad_h = int32_t(round(scale_ratio * src_height));
    float dw = img_size_w - new_pad_w;
    float dh = img_size_h - new_pad_h;
    if (new_pad_h != src_height || new_pad_w != src_width) {
        cv::resize(src_img, dst_img, cv::Size(new_pad_w, new_pad_h));
    } else {
        dst_img = src_img.clone();
    }
    int32_t padding_top = (int32_t)(round(dh / 2 - 0.1f));
    int32_t padding_bottom = (int32_t)(dh - padding_top);
    int32_t padding_left = (int32_t)(round(dw / 2 - 0.1f));
    int32_t padding_right = (int32_t)(dw - padding_left);
    cv::copyMakeBorder(dst_img, dst_img, padding_top, padding_bottom,
                       padding_left, padding_right, cv::BORDER_CONSTANT,
                       cv::Scalar(114, 114, 114));
    resize_padding_info.orginal_img_hw_[0] = src_height;
    resize_padding_info.orginal_img_hw_[1] = src_width;
    resize_padding_info.resized_padding_img_hw_[0] = dst_img.cols;
    resize_padding_info.resized_padding_img_hw_[1] = dst_img.rows;
    resize_padding_info.expansion_ratio_hw_[0] = new_pad_h * 1.0f / src_height;
    resize_padding_info.expansion_ratio_hw_[1] = new_pad_w * 1.0f / src_width;
    resize_padding_info.valid_img_hw_[0] = new_pad_h;
    resize_padding_info.valid_img_hw_[1] = new_pad_w;
    resize_padding_info.padding_tblr_[0] = padding_top;
    resize_padding_info.padding_tblr_[1] = padding_bottom;
    resize_padding_info.padding_tblr_[2] = padding_left;
    resize_padding_info.padding_tblr_[3] = padding_right;

    return ret;
}

float MmyoloFaceDetectWithEngine::clip_coord(float cord,
                                             const int32_t img_size) {
    return (std::min)((std::max)(0.f, cord), static_cast<float>(img_size - 1));
}

int32_t MmyoloFaceDetectWithEngine::rtm_preprocess(
    const cv::Mat &img_mat, int32_t model_input_w, int32_t model_input_h,
    cv::Mat &resize_mat, ResizedPaddingInfo &resize_padding_info) {
    int32_t ret = ICVBASE_NO_ERROR;
    resize_mat.release();
    srlog_error_return(
        (!img_mat.empty()),
        ("MmyoloFaceDetectWithEngine::preprocess | img_mat is empty!"),
        ICVBASE_INPUT_ERROR);

    srlog_error_return(img_mat.cols > 0,
                       ("img_mat.cols = %d <= 0", img_mat.cols),
                       ICVBASE_INPUT_ERROR);
    srlog_error_return(img_mat.rows > 0,
                       ("img_mat.rows = %d <= 0", img_mat.rows),
                       ICVBASE_INPUT_ERROR);

    srlog_error_return(model_input_w > 0,
                       ("model_input_w = %d <= 0", model_input_w),
                       ICVBASE_INPUT_ERROR);
    srlog_error_return(model_input_h > 0,
                       ("model_input_h = %d <= 0", model_input_h),
                       ICVBASE_INPUT_ERROR);

    cv::Mat temp_dst;
    ret = load_image_static(img_mat, model_input_w, model_input_h, temp_dst);

    ret = letterbox(temp_dst, model_input_w, model_input_h, resize_mat,
                    resize_padding_info);
    srlog_error_return((!ret), ("rtm_preprocess | resize_uniform inlegal !"),
                       ret);
    return ret;
}

int32_t MmyoloFaceDetectWithEngine::preprocess_impl(const RESTYPE res_type,
                                                    const cv::Mat &input_mat,
                                                    void *target_data,
                                                    void *reserved) {
    int32_t ret = ICVBASE_NO_ERROR;
    ICVFrameWorkFaces &face_targets = *(ICVFrameWorkFaces *)target_data;
    int32_t model_input_h = data_io_.inputs_.at(INPUT_NODE_NAME).dims_.d[2];
    int32_t model_input_w = data_io_.inputs_.at(INPUT_NODE_NAME).dims_.d[3];

    // 1.preprocess
    cv::Mat resize_mat;

    ret = rtm_preprocess(input_mat, model_input_w, model_input_h, resize_mat,
                         resize_padding_info_);
    srlog_error_return((!ret), ("detect_face | preprocess inlegal !"), ret);
    // 2.sub mean and divide std
    std::vector<float> mean_data;
    mean_data.push_back(static_cast<float>(FD_MEAN_R));
    mean_data.push_back(static_cast<float>(FD_MEAN_G));
    mean_data.push_back(static_cast<float>(FD_MEAN_B));
    std::vector<float> std_data;
    std_data.push_back(static_cast<float>(FD_VAR_R));
    std_data.push_back(static_cast<float>(FD_VAR_G));
    std_data.push_back(static_cast<float>(FD_VAR_B));

    ret = DNNCVLIB::sub_mean_and_divide_std(
        DNNCVLIB::BGRBGRBGRTOBBBGGGRRR, resize_mat,
        data_io_.inputs_.at(INPUT_NODE_NAME).data_.data(), mean_data.data(),
        std_data.data());
    srlog_error_return((!ret), ("sub_mean_and_divide_std inlegal !"), ret);

    return ret;
}

int32_t
MmyoloFaceDetectWithEngine::detect_face(const RESTYPE res_type,
                                        const cv::Mat &img,
                                        ICVFrameWorkFaces &face_targets) {
    int32_t ret = ICVBASE_NO_ERROR;
    srlog_error_return(
        ((res_type > RESTYPE_NONE) && (res_type < RESTYPE_COUNT)),
        ("detect_face:%d, inlegal !", static_cast<int32_t>(res_type)),
        ICVBASE_SUPPORT_ERROR);

    std::call_once(once_flag_, [this, res_type, &ret]() {
        ret = DnnWrapperInst()->data_io_prepares(res_type, data_io_);
    });
    srlog_error_return((!ret),
                       ("DnnWrapperInst | prepare_inputs( {} ) error",
                        static_cast<int32_t>(res_type)),
                       ret);

    ori_img_ = img;

    ret = preprocess(res_type, img, (void *)&face_targets);
    srlog_error_return(
        (!ret),
        ("MmyoloFaceDetectWithEngine::detect_face | preprocess inlegal !"),
        ret);

    ret = forward(res_type);
    srlog_error_return(
        (!ret), ("MmyoloFaceDetectWithEngine::detect_face | forward inlegal !"),
        ret);

    ret = postprocess(res_type, (void *)&face_targets);
    srlog_error_return(
        (!ret),
        ("MmyoloFaceDetectWithEngine::detect_face | postprocess inlegal !"),
        ret);

    return ret;
}

int32_t MmyoloFaceDetectWithEngine::postprocess_impl(const RESTYPE res_type,
                                                     void *target_data,
                                                     void *reserved) {
    int32_t ret = ICVBASE_NO_ERROR;

    ICVFrameWorkFaces &face_targets = *(ICVFrameWorkFaces *)target_data;

    float detect_thre = param_inst_.get_iCVFaceDetect_detect_thres();
    int32_t model_input_h = data_io_.inputs_.at(INPUT_NODE_NAME).dims_.d[2];
    int32_t model_input_w = data_io_.inputs_.at(INPUT_NODE_NAME).dims_.d[3];

    std::vector<cv::Rect2f> det_rects;
    std::vector<float> scores;
    std::vector<std::vector<BoxInfo>> decode_results;
    decode_results.resize(NUM_CLASS);
    for (auto &output : data_io_.outputs_) {
        const std::string output_name = output.first;
        int32_t patch = model_input_h * model_input_w /
                        (fd_heads_strides_.at(output_name) *
                         fd_heads_strides_.at(output_name));
        std::vector<ClsPred> cls_pred_vec;
        std::vector<MmyoloDisPred> dis_pred_vec;
        float *result = output.second.data_.data();
        for (int32_t i = 0; i < patch; ++i) {
            ClsPred cls_pred;
            memcpy((void *)cls_pred.cls_preds, (void *)result,
                   NUM_CLASS * sizeof(float));
            cls_pred_vec.push_back(cls_pred);
            result += NUM_CLASS;
            MmyoloDisPred dis_pred;
            memcpy((void *)dis_pred.dis_preds, (void *)result,
                   MMYOLO_DIS_PRED_LEN * sizeof(float));
            dis_pred_vec.push_back(dis_pred);
            result += MMYOLO_DIS_PRED_LEN;
        }
        mmyolo_decode_infer(model_input_h, model_input_w, cls_pred_vec,
                            dis_pred_vec, fd_heads_strides_.at(output_name),
                            detect_thre, decode_results);
    }

    for (auto box_info : decode_results[FACE_LABEL]) {
        scores.push_back(box_info.score);
        cv::Rect2f rect;
        rect.x = (box_info.x1 - resize_padding_info_.padding_tblr_[2]) /
                 resize_padding_info_.valid_img_hw_[1] * 1.f;
        rect.y = (box_info.y1 - resize_padding_info_.padding_tblr_[0]) /
                 resize_padding_info_.valid_img_hw_[0] * 1.f;
        rect.width = (box_info.x2 - box_info.x1) /
                     resize_padding_info_.valid_img_hw_[1] * 1.f;
        rect.height = (box_info.y2 - box_info.y1) /
                      resize_padding_info_.valid_img_hw_[0] * 1.f;
        det_rects.push_back(rect);
    }

    float iou_thres = param_inst_.get_iCVFaceDetect_nms_iou_thres();
    int32_t max_init_nms_cnt =
        param_inst_.get_iCVFaceDetect_max_init_nms_rect_cnt();
    std::vector<NMSOutData> nms_res;
    ret = nms2(det_rects, scores, iou_thres, max_init_nms_cnt, nms_res);
    srlog_error_return((!ret), ("detect_face | nms2 error !"),
                       ICVBASE_ALGORITHM_EXECUTION_ERROR);

    face_targets.face_num = MAX_DETECT_OBJECT_NUM > nms_res.size()
                                ? nms_res.size()
                                : MAX_DETECT_OBJECT_NUM;
    int32_t is_face_index = 0;
    for (int32_t i = 0; i < face_targets.face_num; ++i) {
        const float score = nms_res[i].score;
        float xmin, ymin, xmax, ymax;
        xmin = (nms_res[i].rect.x) * ori_img_.cols;
        ymin = (nms_res[i].rect.y) * ori_img_.rows;
        xmax = (nms_res[i].rect.x + nms_res[i].rect.width) * ori_img_.cols;
        ymax = (nms_res[i].rect.y + nms_res[i].rect.height) * ori_img_.rows;
        if (xmax < COORD_BOUND || ymax < COORD_BOUND) {
            srlog_info("invalid face : rect = [%d, %d, %d, %d], score = %f "
                       "abondon ...\n",
                       int32_t(xmin), int32_t(ymin), int32_t(xmax - xmin),
                       int32_t(ymax - ymin), score);
            continue;
        }
        // abandon the small face
        int32_t min_face_size_h_thres =
            param_inst_.get_iCVFaceDetect_min_face_size_h_thres();
        int32_t min_face_size_w_thres =
            param_inst_.get_iCVFaceDetect_min_face_size_w_thres();
        if (((xmax - xmin) <= min_face_size_w_thres) ||
            ((ymax - ymin) <= min_face_size_h_thres)) {
            srlog_info("small face : rect = [%d, %d, %d, %d], score = %f "
                       "abondon ...\n",
                       int32_t(xmin), int32_t(ymin), int32_t(xmax - xmin),
                       int32_t(ymax - ymin), score);
            continue;
        }

        xmin = clip_coord(xmin, ori_img_.cols);
        ymin = clip_coord(ymin, ori_img_.rows);
        xmax = clip_coord(xmax, ori_img_.cols);
        ymax = clip_coord(ymax, ori_img_.rows);

        face_targets.faces[is_face_index].face_rect_with_info.rect.x = xmin;
        face_targets.faces[is_face_index].face_rect_with_info.rect.y = ymin;
        face_targets.faces[is_face_index].face_rect_with_info.rect.w =
            xmax - xmin;
        face_targets.faces[is_face_index].face_rect_with_info.rect.h =
            ymax - ymin;
        face_targets.faces[is_face_index].face_rect_with_info.conf_score =
            score;
        face_targets.faces[is_face_index].face_rect_with_info.id = 10086;
        is_face_index++;
    }
    face_targets.face_num = is_face_index;
    return ret;
}

int32_t MmyoloFaceDetectWithEngine::set_param(const char *para,
                                              const char *value) {
    int32_t ret = ICVBASE_NO_ERROR;
    bool bflag = param_inst_.setConfigValue(para, value);
    if (bflag) {
        return ICVBASE_NO_ERROR;
    }

    int32_t param_id = iCVFaceDetect_None;
    for (; param_id < iCVFaceDetect_Count; param_id++) {
        if (std::strcmp(para, iCVFaceDetect_param_str[param_id]) == 0) {
            break;
        }
    }
    srlog_error_return((param_id < iCVFaceDetect_Count),
                       ("%s | invalid param , param = %s, value = %s",
                        __FUNCTION__, para, value),
                       ICVBASE_INPUT_ERROR);
    return ret;
}

int32_t MmyoloFaceDetectWithEngine::get_param(const char *para, char *value,
                                              int32_t len) {
    int32_t ret = ICVBASE_NO_ERROR;
    std::string str_val;
    bool bflag = param_inst_.getConfigValue(para, str_val);
    if (bflag) {
        if (len < str_val.size()) {
            srlog_error("realsize : {} out of range {}", str_val.size(), len);
            return ICVBASE_MEMORY_ERROR;
        }
        memcpy(value, str_val.c_str(), str_val.size());
        return ICVBASE_NO_ERROR;
    }

    int32_t param_id = iCVFaceDetect_None;
    for (; param_id < iCVFaceDetect_Count; param_id++) {
        if (std::strcmp(para, iCVFaceDetect_param_str[param_id]) == 0) {
            break;
        }
    }
    srlog_error_return((param_id < iCVFaceDetect_Count),
                       ("%s | invalid param , param = %s, value = %s",
                        __FUNCTION__, para, value),
                       ICVBASE_INPUT_ERROR);
    return ret;
}
