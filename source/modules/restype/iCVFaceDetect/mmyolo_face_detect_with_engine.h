#ifndef __FACE_DETECT_WITH_ENGINE_H__
#define __FACE_DETECT_WITH_ENGINE_H__

#include "iCVBase_def.h"
#include "iCVFaceDetect_cfg.h"
#include "iCVFrameWork_types.h"
#include <opencv2/opencv.hpp>

typedef struct _ResizedPaddingInfo {
    int32_t orginal_img_hw_[2] = {0};
    int32_t resized_padding_img_hw_[2] = {0};
    int32_t valid_img_hw_[2] = {0};
    float expansion_ratio_hw_[2] = {0.f};
    int32_t padding_tblr_[4] = {0};
} ResizedPaddingInfo;

class MmyoloFaceDetectWithEngine : public iCVBaseEngine {
  public:
    MmyoloFaceDetectWithEngine(const char *workdir) {
        param_inst_.init(workdir);
    }
    ~MmyoloFaceDetectWithEngine() {}

  private:
    int letterbox(const cv::Mat &src_img, int img_size_w, int img_size_h,
                  cv::Mat &dst1, ResizedPaddingInfo &resize_padding_info);
    int load_image_static(const cv::Mat &img_mat, int model_input_w,
                          int model_input_h, cv::Mat &resize_mat);
    float clip_coord(float cord, const int32_t img_size);
    int rtm_preprocess(const cv::Mat &img_mat, int model_input_w,
                       int model_input_h, cv::Mat &resize_img,
                       ResizedPaddingInfo &resize_padding_info);

  public:
    int detect_face(const RESTYPE res_type, const cv::Mat &img,
                    ICVFaces &face_target);

  public:
    int set_param(const char *para, const char *value);
    int get_param(const char *para, char *value, int32_t len);
    int reset();

  protected:
    int32_t preprocess_impl(const RESTYPE res_type, const cv::Mat &input_mat,
                            void *target_data, void *reserved) override;

    int32_t postprocess_impl(const RESTYPE res_type, void *target_data,
                             void *reserved) override;

  private:
    CFG_iCVFaceDetect param_inst_;
    // static const std::vector<int> fd_heads_strides_;
    static const std::map<const std::string, const int> fd_heads_strides_;
    ResizedPaddingInfo resize_padding_info_;
    cv::Mat ori_img_;
};

#endif //__FACE_DETECT_WITH_ENGINE_H__
