#pragma once
#include "opencv2/opencv.hpp"
#include "ort_wrapper.h" //onnx runtime

class iCVBaseEngine {
  public:
    virtual ~iCVBaseEngine() {}

  public:
    virtual int32_t set_param(const char *para, const char *value) = 0;
    virtual int32_t get_param(const char *para, char *value, int32_t len) = 0;

  protected:
    virtual int32_t preprocess_impl(const RESTYPE res_type,
                                    const cv::Mat &input_mat, void *target_data,
                                    void *reserved) = 0;
    virtual int32_t postprocess_impl(const RESTYPE res_type, void *target_data,
                                     void *reserved) = 0;
    int32_t forward_impl(const RESTYPE res_type, void *reserved) {
        int32_t ret = ICVBASE_NO_ERROR;
        ret = DnnWrapperInst()->inference(
            res_type, input_data_buffers_, input_data_shapes_,
            output_data_buffers_, output_data_shapes_);
        return ret;
    }

    int32_t preprocess(const RESTYPE res_type, const cv::Mat &input_mat,
                       void *target_data, void *reserverd = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("preprocess( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_ENGINE");
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        iCVProfilerProfiler(TAG);
        ret = preprocess_impl(res_type, input_mat, target_data, reserverd);
        return ret;
    }

    int32_t forward(const RESTYPE res_type, void *reserverd = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("forward( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_ENGINE");
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        iCVProfilerProfiler(TAG);
        ret = forward_impl(res_type, reserverd);
        return ret;
    }

    int32_t postprocess(const RESTYPE res_type, void *target_data,
                        void *reserverd = NULL) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("postprocess( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_ENGINE");
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        iCVProfilerProfiler(TAG);
        ret = postprocess_impl(res_type, target_data, reserverd);
        return ret;
    }

  public:
    int32_t prepare_input_data_buffer(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("postprocess( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_ENGINE");
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        iCVProfilerProfiler(TAG);
        ret = DnnWrapperInst()->get_input_shape(res_type, input_data_shapes_);
        srlog_error_return(!ret,
                           ("DnnWrapperInst()->get_input_shape( {} ) failed.",
                            static_cast<int>(res_type)),
                           ret);
        ret = DnnWrapperInst()->get_output_shape(res_type, output_data_shapes_);
        srlog_error_return(!ret,
                           ("DnnWrapperInst()->get_output_shape( {} ) failed.",
                            static_cast<int>(res_type)),
                           ret);
        int32_t input_data_size = input_data_shapes_.size();
        srlog_verify_para(input_data_size > 0, ICVBASE_SHAPE_ERROR);
        input_data_buffers_.resize(input_data_size);
        for (int32_t idx = 0; idx < input_data_buffers_.size(); ++idx) {
            int32_t data_buffer_size = input_data_shapes_.at(idx).count();
            srlog_verify_para(data_buffer_size > 0, ICVBASE_SHAPE_ERROR);
            input_data_buffers_.at(idx).resize(data_buffer_size, 0.f);
        }
        return ret;
    }

    int32_t prepare_input_output_data_buffer(const RESTYPE res_type) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_error_return(
            (0 != DnnWrapperInst()->restype_name_map_.count(res_type)),
            ("postprocess( {} ) failed.", static_cast<int>(res_type)),
            ICVBASE_INPUT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_ENGINE");
        std::string TAG = DnnWrapperInst()->restype_name_map_.at(res_type) +
                          "_" + __FUNCTION__;
        iCVProfilerProfiler(TAG);
        ret = DnnWrapperInst()->get_input_shape(res_type, input_data_shapes_);
        srlog_error_return(!ret,
                           ("DnnWrapperInst()->get_input_shape( {} ) failed.",
                            static_cast<int>(res_type)),
                           ret);
        ret = DnnWrapperInst()->get_output_shape(res_type, output_data_shapes_);
        srlog_error_return(!ret,
                           ("DnnWrapperInst()->get_output_shape( {} ) failed.",
                            static_cast<int>(res_type)),
                           ret);
        const int32_t max_shapes_size =
            (std::max)(input_data_shapes_.size(), output_data_shapes_.size());
        srlog_verify_para(max_shapes_size > 0, ICVBASE_SHAPE_ERROR);

        std::vector<int32_t> new_input_data_size_vec(max_shapes_size, 0);
        std::transform(
            input_data_shapes_.begin(), input_data_shapes_.end(),
            new_input_data_size_vec.begin(),
            [](const DnnWrapperClass::Dims &shape) { return shape.count(); });
        std::vector<int32_t> new_output_data_size_vec(max_shapes_size, 0);
        std::transform(
            output_data_shapes_.begin(), output_data_shapes_.end(),
            new_output_data_size_vec.begin(),
            [](const DnnWrapperClass::Dims &shape) { return shape.count(); });
        input_output_data_buffers_.resize(max_shapes_size);
        for (int32_t idx = 0; idx < max_shapes_size; ++idx) {
            int32_t data_buffer_size =
                (std::max)(new_input_data_size_vec.at(idx),
                           new_output_data_size_vec.at(idx));
            srlog_verify_para(data_buffer_size > 0, ICVBASE_SHAPE_ERROR);
            input_output_data_buffers_.at(idx).resize(data_buffer_size, 0.f);
        }

        return ret;
    }

  public:
    using DataBuf = std::vector<std::vector<float>>;
    using DataShape = std::vector<DnnWrapperClass::Dims>;
    DataBuf input_data_buffers_;
    DataBuf output_data_buffers_;
    DataBuf input_output_data_buffers_;
    DataShape input_data_shapes_;
    DataShape output_data_shapes_;
};

class iCVBaseDef {
  public:
    virtual ~iCVBaseDef() {}

  public:
    virtual int32_t init(const char *workdir) = 0;
    virtual int32_t fini() = 0;
    virtual int32_t set_param(const char *para, const char *value) = 0;
    virtual int32_t get_param(const char *para, char *value, int32_t len) = 0;
    virtual int32_t process(const void *in, void *out) = 0;
    virtual int32_t get_result(void *out) = 0;

  public:
    virtual int32_t reset_inst() {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_crit("Not Implement ...");
        return ret;
    }

    virtual int32_t set_input(const void *in, cv::Mat &input_mat) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_verify_ptr(in, ICVBASE_INPUT_ERROR);
        srlog_perf(LOG_PROF_TAG, "BASE_DEF");
        const ICVBaseDataIn *in_ptr = static_cast<const ICVBaseDataIn *>(in);
        if (in_ptr->data_stat & ICV_BASE_DATA_STAT_IMG) {
            srlog_verify_ptr(in_ptr->img.data, ICVBASE_INPUT_ERROR);
            srlog_verify_para(in_ptr->img.w > 1, ICVBASE_INPUT_ERROR);
            srlog_verify_para(in_ptr->img.h > 1, ICVBASE_INPUT_ERROR);
            srlog_verify_para(in_ptr->img.data_byte_size > 1,
                              ICVBASE_INPUT_ERROR);
            srlog_verify_para((in_ptr->img.fmt > ICV_BASE_IMG_FMT_NONE) &&
                                  (in_ptr->img.fmt < ICV_BASE_IMG_FMT_COUNT),
                              ICVBASE_INPUT_ERROR);
            ret = format_convert(in_ptr->img.data, in_ptr->img.w, in_ptr->img.h,
                                 in_ptr->img.data_byte_size, in_ptr->img.fmt,
                                 input_mat);
            srlog_error_return(!ret,
                               ("format_convert( {} , {} , {} , {} ) failed.",
                                in_ptr->img.w, in_ptr->img.h,
                                in_ptr->img.data_byte_size,
                                static_cast<int>(in_ptr->img.fmt)),
                               ret);
            if (NULL != in_ptr->img.next) {
                // TODO
                // cv::Mat tmp_mat;
                // ret = set_input(static_cast<const void *>(in_ptr->img.next),
                //                 tmp_mat);
            }
        }
        if (in_ptr->data_stat & ICV_BASE_DATA_STAT_AUD) {
            srlog_crit("Not Implement ...");
            // TODO
        }
        if (in_ptr->data_stat & ICV_BASE_DATA_STAT_TXT) {
            srlog_crit("Not Implement ...");
            // TODO
        }
        return ret;
    }

    int32_t format_convert(const char *data, const int32_t width,
                           const int32_t height, const int32_t size,
                           ICV_BASE_IMG_FMT fmt, cv::Mat &output_mat) {
        int32_t ret = ICVBASE_NO_ERROR;
        srlog_perf(LOG_PROF_TAG, "BASE_DEF");
        switch (fmt) {
        case ICV_BASE_IMG_FMT_GRAY: {
            if (size != width * height) {
                ret = ICVBASE_SUPPORT_ERROR;
                break;
            }
            cv::Mat img_mat(height, width, CV_8UC1, (unsigned char *)data);
            cv::cvtColor(img_mat, output_mat, cv::COLOR_GRAY2BGR);
            break;
        }

        case ICV_BASE_IMG_FMT_RGB: {
            if (size != width * height * 3) {
                ret = ICVBASE_SUPPORT_ERROR;
                break;
            }
            cv::Mat img_mat(height, width, CV_8UC3, (unsigned char *)data);
            cv::cvtColor(img_mat, output_mat, cv::COLOR_RGB2BGR);
            break;
        }

        case ICV_BASE_IMG_FMT_BGR: {
            if (size != width * height * 3) {
                ret = ICVBASE_SUPPORT_ERROR;
                break;
            }
            cv::Mat img_mat(height, width, CV_8UC3, (unsigned char *)data);
            output_mat = img_mat;
            break;
        }

        case ICV_BASE_IMG_FMT_BGRA: {
            if (size != width * height * 4) {
                ret = ICVBASE_SUPPORT_ERROR;
                break;
            }
            cv::Mat img_mat(height, width, CV_8UC4, (unsigned char *)data);
            cv::cvtColor(img_mat, output_mat, cv::COLOR_BGRA2BGR);
            break;
        }

        case ICV_BASE_IMG_FMT_YUV420_NV12: {
            if (size != width * height * 3 / 2) {
                ret = ICVBASE_SUPPORT_ERROR;
                break;
            }
            cv::Mat img_mat(height * 3 / 2, width, CV_8UC1,
                            (unsigned char *)data);
            cv::cvtColor(img_mat, output_mat, cv::COLOR_YUV2BGR_NV12);
            break;
        }

        case ICV_BASE_IMG_FMT_YUV420_NV21: {
            if (size != width * height * 3 / 2) {
                ret = ICVBASE_SUPPORT_ERROR;
                break;
            }
            cv::Mat img_mat(height * 3 / 2, width, CV_8UC1,
                            (unsigned char *)data);
            cv::cvtColor(img_mat, output_mat, cv::COLOR_YUV2BGR_NV21);
            break;
        }
        default: {
            ret = ICVBASE_SUPPORT_ERROR;
        }
        }
        srlog_error_return(
            !ret, ("{} : Image format not supported.", static_cast<int>(fmt)),
            ret);
        return ret;
    }

  public:
    RESTYPE res_type_;
};
