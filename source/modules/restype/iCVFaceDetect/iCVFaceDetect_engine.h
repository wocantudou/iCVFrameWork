#ifndef __FACE_DETECT_WITH_ENGINE_H__
#define __FACE_DETECT_WITH_ENGINE_H__

#include "iCVBase_def.h"
#include "iCVFaceDetect_cfg.h"
#include "iCVFrameWork_types.h"

typedef struct _ResizedPaddingInfo {
    int32_t orginal_img_hw_[2] = {0};
    int32_t resized_padding_img_hw_[2] = {0};
    int32_t valid_img_hw_[2] = {0};
    float expansion_ratio_hw_[2] = {0.f};
    int32_t padding_tblr_[4] = {0};
} ResizedPaddingInfo;

class FaceDetectWithEngine : public iCVBaseEngine {
  public:
    FaceDetectWithEngine(const char *workdir) { param_inst_.init(workdir); }
    ~FaceDetectWithEngine() {}

  private:
    int32_t letterbox(const cv::Mat &src_img, int32_t img_size_w,
                      int32_t img_size_h, cv::Mat &dst1,
                      ResizedPaddingInfo &resize_padding_info);
    int32_t load_image_static(const cv::Mat &img_mat, int32_t model_input_w,
                              int32_t model_input_h, cv::Mat &resize_mat);
    float clip_coord(float cord, const int32_t img_size);
    int32_t rtm_preprocess(const cv::Mat &img_mat, int32_t model_input_w,
                           int32_t model_input_h, cv::Mat &resize_img,
                           ResizedPaddingInfo &resize_padding_info);

  public:
    int32_t detect_face(const RESTYPE res_type, const cv::Mat &img,
                        ICVFrameWorkFaces &face_target);

  public:
    int32_t set_param(const char *para, const char *value);
    int32_t get_param(const char *para, char *value, int32_t len);
    int32_t reset();

  protected:
    int32_t preprocess_impl(const RESTYPE res_type, const cv::Mat &input_mat,
                            void *target_data, void *reserved) override;

    int32_t postprocess_impl(const RESTYPE res_type, void *target_data,
                             void *reserved) override;

  private:
    CFG_iCVFaceDetect param_inst_;
    static const std::map<const std::string, const int> fd_heads_strides_;
    ResizedPaddingInfo resize_padding_info_;
    cv::Mat ori_img_;
};

#endif //__FACE_DETECT_WITH_ENGINE_H__
