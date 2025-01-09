#include "iCVBase_errcode.h"
#include "iCVFrameWork_api.h"
#include "iCVFrameWork_types.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>

namespace py = pybind11;

/**
 * @brief Python binding module for iCVFrameWork
 *
 * This module provides Python bindings for the iCVFrameWork C++ API using
 * pybind11. It exposes the core functionality including:
 * - Enum types (RESTYPE, ICV_BASE_IMG_FMT, ICV_BASE_DATA_STAT)
 * - Data structures (ICVBaseRect2f, ICVBasePoint2f, etc.)
 * - API functions (Initialize, CreateInst, SetParameter, etc.)
 *
 */
PYBIND11_MODULE(iCVFrameWorkPY, m) {
    m.doc() = R"pbdoc(
        Python bindings for iCVFrameWork computer vision framework

        This module provides Python interface to iCVFrameWork, a high-performance
        computer vision framework. It supports:
        - Face detection and tracking
        - Face alignment
        - Image processing
        - Multi-modal data processing (image, audio, text)

        Example usage:
            import iCVFrameWorkPY as icv

            # Initialize framework
            icv.iCVFrameWorkInitialize("config.json")

            # Create face detection instance
            ret, inst = icv.iCVFrameWorkCreateInst(icv.RESTYPE_FACE_DETECT_CPU)

            # Process image
            import numpy as np
            img = np.random.rand(480, 640, 3).astype(np.uint8)
            result = np.zeros((100,), dtype=np.float32)
            ret = icv.iCVFrameWorkGetResult(inst, img, result)

            # Clean up
            icv.iCVFrameWorkDestroyInst(inst)
            icv.iCVFrameWorkUninitialize()
    )pbdoc";

    // Bind RESTYPE enum
    py::enum_<RESTYPE>(m, "RESTYPE")
        .value("RESTYPE_NONE", RESTYPE_NONE)
        .value("RESTYPE_FACE_DETECT_CPU", RESTYPE_FACE_DETECT_CPU)
        .value("RESTYPE_FACE_DETECT_NPU", RESTYPE_FACE_DETECT_NPU)
        .value("RESTYPE_FACE_ALIGNMENT_CPU", RESTYPE_FACE_ALIGNMENT_CPU)
        .value("RESTYPE_FACE_ALIGNMENT_NPU", RESTYPE_FACE_ALIGNMENT_NPU)
        .value("MODTYPE_FACE_SINGLETRACK_CPU", MODTYPE_FACE_SINGLETRACK_CPU)
        .value("MODTYPE_FACE_SINGLETRACK_NPU", MODTYPE_FACE_SINGLETRACK_NPU)
        .value("RESTYPE_COUNT", RESTYPE_COUNT)
        .export_values();

    // Bind ICV_BASE_IMG_FMT enum
    py::enum_<ICV_BASE_IMG_FMT>(m, "ICV_BASE_IMG_FMT")
        .value("ICV_BASE_IMG_FMT_NONE", ICV_BASE_IMG_FMT_NONE)
        .value("ICV_BASE_IMG_FMT_GRAY", ICV_BASE_IMG_FMT_GRAY)
        .value("ICV_BASE_IMG_FMT_RGB", ICV_BASE_IMG_FMT_RGB)
        .value("ICV_BASE_IMG_FMT_BGR", ICV_BASE_IMG_FMT_BGR)
        .value("ICV_BASE_IMG_FMT_BGRA", ICV_BASE_IMG_FMT_BGRA)
        .value("ICV_BASE_IMG_FMT_YUV420_NV12", ICV_BASE_IMG_FMT_YUV420_NV12)
        .value("ICV_BASE_IMG_FMT_YUV420_NV21", ICV_BASE_IMG_FMT_YUV420_NV21)
        .value("ICV_BASE_IMG_FMT_COUNT", ICV_BASE_IMG_FMT_COUNT)
        .export_values();

    // Bind ICV_BASE_DATA_STAT enum
    py::enum_<ICV_BASE_DATA_STAT>(m, "ICV_BASE_DATA_STAT")
        .value("ICV_BASE_DATA_STAT_NONE", ICV_BASE_DATA_STAT_NONE)
        .value("ICV_BASE_DATA_STAT_IMG", ICV_BASE_DATA_STAT_IMG)
        .value("ICV_BASE_DATA_STAT_AUD", ICV_BASE_DATA_STAT_AUD)
        .value("ICV_BASE_DATA_STAT_TXT", ICV_BASE_DATA_STAT_TXT)
        .value("ICV_BASE_DATA_STAT_IMG_AUD", ICV_BASE_DATA_STAT_IMG_AUD)
        .value("ICV_BASE_DATA_STAT_IMG_TXT", ICV_BASE_DATA_STAT_IMG_TXT)
        .value("ICV_BASE_DATA_STAT_IMG_AUD_TXT", ICV_BASE_DATA_STAT_IMG_AUD_TXT)
        .value("ICV_BASE_DATA_STAT_COUNT", ICV_BASE_DATA_STAT_COUNT)
        .export_values();

    py::class_<ICVBaseRect2f>(m, "ICVBaseRect2f")
        .def(py::init<>())
        .def_readwrite("x", &ICVBaseRect2f::x)
        .def_readwrite("y", &ICVBaseRect2f::y)
        .def_readwrite("w", &ICVBaseRect2f::w)
        .def_readwrite("h", &ICVBaseRect2f::h);

    py::class_<ICVBaseRect2fWithInfo>(m, "ICVBaseRect2fWithInfo")
        .def(py::init<>())
        .def_readwrite("rect", &ICVBaseRect2fWithInfo::rect)
        .def_readwrite("conf_score", &ICVBaseRect2fWithInfo::conf_score)
        .def_readwrite("id", &ICVBaseRect2fWithInfo::id);

    py::class_<ICVBasePoint2f>(m, "ICVBasePoint2f")
        .def(py::init<>())
        .def_readwrite("x", &ICVBasePoint2f::x)
        .def_readwrite("y", &ICVBasePoint2f::y);

    py::class_<ICVBaseNumericFloat>(m, "ICVBaseNumericFloat")
        .def(py::init<>())
        .def_readwrite("valid", &ICVBaseNumericFloat::valid)
        .def_readwrite("val", &ICVBaseNumericFloat::val);

    py::class_<ICVBasePoint2fWithInfo>(m, "ICVBasePoint2fWithInfo")
        .def(py::init<>())
        .def_readwrite("point", &ICVBasePoint2fWithInfo::point)
        .def_readwrite("visiable_info", &ICVBasePoint2fWithInfo::visiable_info)
        .def_readwrite("conf_score", &ICVBasePoint2fWithInfo::conf_score);

    py::class_<ICVBasePoint3f>(m, "ICVBasePoint3f")
        .def(py::init<>())
        .def_readwrite("x", &ICVBasePoint3f::x)
        .def_readwrite("y", &ICVBasePoint3f::y)
        .def_readwrite("z", &ICVBasePoint3f::z);

    py::class_<ICVBaseAngle2d>(m, "ICVBaseAngle2d")
        .def(py::init<>())
        .def_readwrite("pitch", &ICVBaseAngle2d::pitch)
        .def_readwrite("yaw", &ICVBaseAngle2d::yaw);

    py::class_<ICVBaseAngle3d>(m, "ICVBaseAngle3d")
        .def(py::init<>())
        .def_readwrite("pitch", &ICVBaseAngle3d::pitch)
        .def_readwrite("yaw", &ICVBaseAngle3d::yaw)
        .def_readwrite("roll", &ICVBaseAngle3d::roll);

    py::class_<ICVBaseImgDataIn>(m, "ICVBaseImgDataIn")
        .def(py::init<>())
        .def_readwrite("data", &ICVBaseImgDataIn::data)
        .def_readwrite("w", &ICVBaseImgDataIn::w)
        .def_readwrite("h", &ICVBaseImgDataIn::h)
        .def_readwrite("data_byte_size", &ICVBaseImgDataIn::data_byte_size)
        .def_readwrite("fmt", &ICVBaseImgDataIn::fmt)
        .def_readwrite("next", &ICVBaseImgDataIn::next);

    py::class_<ICVBaseAudDataIn>(m, "ICVBaseAudDataIn")
        .def(py::init<>())
        .def_readwrite("data", &ICVBaseAudDataIn::data)
        .def_readwrite("data_byte_size", &ICVBaseAudDataIn::data_byte_size)
        .def_readwrite("sample_rate", &ICVBaseAudDataIn::sample_rate)
        .def_readwrite("sample_bits", &ICVBaseAudDataIn::sample_bits)
        .def_readwrite("channel_num", &ICVBaseAudDataIn::channel_num)
        .def_readwrite("next", &ICVBaseAudDataIn::next);

    py::class_<ICVBaseTxtDataIn>(m, "ICVBaseTxtDataIn")
        .def(py::init<>())
        .def_readwrite("data", &ICVBaseTxtDataIn::data)
        .def_readwrite("data_byte_size", &ICVBaseTxtDataIn::data_byte_size)
        .def_readwrite("next", &ICVBaseTxtDataIn::next);

    py::class_<ICVBaseDataIn>(m, "ICVBaseDataIn")
        .def(py::init<>())
        .def_readwrite("data_stat", &ICVBaseDataIn::data_stat)
        .def_readwrite("img", &ICVBaseDataIn::img)
        .def_readwrite("aud", &ICVBaseDataIn::aud)
        .def_readwrite("txt", &ICVBaseDataIn::txt);

    py::class_<ICVFrameWorkFace>(m, "ICVFrameWorkFace")
        .def(py::init<>())
        .def_readwrite("face_rect_with_info",
                       &ICVFrameWorkFace::face_rect_with_info);

    py::class_<ICVFrameWorkFaces>(m, "ICVFrameWorkFaces")
        .def(py::init<>())
        .def_property_readonly("faces",
                               [](const ICVFrameWorkFaces &self) {
                                   return std::vector<ICVFrameWorkFace>(
                                       self.faces, self.faces + self.face_num);
                               })
        .def_readwrite("face_num", &ICVFrameWorkFaces::face_num);

    // Bind functions
    m.def(
        "iCVFrameWorkInitialize",
        [](const std::string &cfg_path, void *reserved) {
            if (cfg_path.empty()) {
                throw std::invalid_argument("Config path cannot be empty");
            }
            return iCVFrameWorkInitialize(cfg_path.c_str(), reserved);
        },
        py::arg("cfg_path"), py::arg("reserved") = nullptr,
        R"pbdoc(
            Initialize the iCVFrameWork framework.

            Args:
                cfg_path (str): Path to configuration file
                reserved: Reserved for future use, can be None

            Returns:
                int: Error code (0 for success)

            Raises:
                ValueError: If config path is empty
                RuntimeError: If initialization fails
        )pbdoc");

    m.def(
        "iCVFrameWorkUninitialize", []() { return iCVFrameWorkUninitialize(); },
        "Uninitialize the iCVFrameWork.");

    m.def(
        "iCVFrameWorkCreateInst",
        [](RESTYPE res_type) {
            if (res_type < RESTYPE_NONE || res_type >= RESTYPE_COUNT) {
                throw std::invalid_argument("Invalid resource type");
            }
            iCVFrameWork_INST inst = nullptr;
            int32_t ret = iCVFrameWorkCreateInst(&inst, res_type);
            if (ret != 0) {
                throw std::runtime_error("Failed to create instance");
            }
            return std::make_pair(ret, inst);
        },
        py::arg("res_type"),
        R"pbdoc(
            Create a new instance of specified resource type.

            Args:
                res_type (RESTYPE): Resource type to create

            Returns:
                tuple: (error_code, instance_handle)

            Raises:
                ValueError: If resource type is invalid
                RuntimeError: If instance creation fails
        )pbdoc");

    m.def(
        "iCVFrameWorkDestroyInst",
        [](iCVFrameWork_INST inst) { return iCVFrameWorkDestroyInst(&inst); },
        py::arg("inst"));

    m.def("iCVFrameWorkResetInst", &iCVFrameWorkResetInst, py::arg("inst"));

    m.def(
        "iCVFrameWorkSetParameter",
        [](iCVFrameWork_INST inst, const std::string &param,
           const std::string &value) {
            return iCVFrameWorkSetParameter(inst, param.c_str(), value.c_str());
        },
        py::arg("inst"), py::arg("param"), py::arg("value"));

    m.def(
        "iCVFrameWorkGetParameter",
        [](iCVFrameWork_INST inst, const std::string &param, int32_t len) {
            std::string value(len, '\0');
            int32_t ret =
                iCVFrameWorkGetParameter(inst, param.c_str(), &value[0], len);
            return std::make_pair(ret, std::string(value.c_str()));
        },
        py::arg("inst"), py::arg("param"), py::arg("len"));

    m.def(
        "iCVFrameWorkGetResult",
        [](iCVFrameWork_INST inst, const ICVBaseDataIn &in_data,
           ICVFrameWorkFaces &out_data) {
            int32_t ret = iCVFrameWorkGetResult(inst, (void *)&in_data,
                                                (void *)&out_data);
            return ret;
        },
        py::arg("inst"), py::arg("in_data"), py::arg("out_data"));

    m.def(
        "iCVFrameWorkResourceAdd",
        [](RESTYPE res_type, const std::string &res_path) {
            const char *path = (res_path == "" ? NULL : res_path.c_str());
            return iCVFrameWorkResourceAdd(res_type, path);
        },
        py::arg("res_type"), py::arg("res_path"));

    m.def(
        "iCVFrameWorkResourceAddFromMem",
        [](RESTYPE res_type, const py::bytes &data) {
            std::string data_str = data;
            const void *data_ptr = data_str.data();
            int32_t len = data_str.size();
            return iCVFrameWorkResourceAddFromMem(res_type, data_ptr, len);
        },
        py::arg("res_type"), py::arg("data"));

    m.def("iCVFrameWorkResourceDelete", &iCVFrameWorkResourceDelete,
          py::arg("res_type"));

    m.def("iCVFrameWorkGetExpiredTime", []() {
        const char *expired_time;
        int32_t ret = iCVFrameWorkGetExpiredTime(&expired_time);
        return std::make_pair(ret,
                              std::string(expired_time ? expired_time : ""));
    });

    m.def(
        "iCVFrameWorkGetResVersion",
        [](RESTYPE res_type) {
            int32_t version_num;
            int32_t ret = iCVFrameWorkGetResVersion(res_type, &version_num);
            return std::make_pair(ret, version_num);
        },
        py::arg("res_type"));

    m.def("iCVFrameWorkGetEngineVersion", []() {
        const char *engine_version;
        int32_t ret = iCVFrameWorkGetEngineVersion(&engine_version);
        return std::make_pair(
            ret, std::string(engine_version ? engine_version : ""));
    });
}
