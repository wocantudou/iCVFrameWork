#include "ort_wrapper.h"
namespace DNN_WRAPPER {
const std::unordered_map<RESTYPE, const std::string>
    OnnxRTWrapper::restype_name_map_ = {
        {RESTYPE_FACE_DETECT_CPU, "RESTYPE_FACE_DETECT_CPU"},
        {RESTYPE_FACE_DETECT_NPU, "RESTYPE_FACE_DETECT_NPU"},
        {RESTYPE_FACE_ALIGNMENT_CPU, "RESTYPE_FACE_ALIGNMENT_CPU"},
        {RESTYPE_FACE_ALIGNMENT_NPU, "RESTYPE_FACE_ALIGNMENT_NPU"}};
}
