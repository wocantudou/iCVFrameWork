#include "iCVBase_errcode.h"
#include "iCVFrameWork_api.h"
#include "iCVFrameWork_types.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(iCVFrameWorkPY, m) {
    m.def(
        "iCVFrameWorkInitialize",
        [](const std::string &cfg_path) {
            return iCVFrameWorkInitialize(cfg_path.c_str(), nullptr);
        },
        py::arg("cfg_path"));

    m.def("iCVFrameWorkUninitialize", &iCVFrameWorkUninitialize);

    m.def(
        "iCVFrameWorkCreateInst",
        [](RESTYPE res_type) {
            iCVFrameWork_INST inst;
            int32_t ret = iCVFrameWorkCreateInst(&inst, res_type);
            return std::make_pair(ret, inst);
        },
        py::arg("res_type"));

    m.def("iCVFrameWorkDestroyInst", &iCVFrameWorkDestroyInst, py::arg("inst"));

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
            return std::make_pair(ret, value);
        },
        py::arg("inst"), py::arg("param"), py::arg("len"));

    m.def(
        "iCVFrameWorkGetResult",
        [](iCVFrameWork_INST inst, const py::bytes &in_data,
           py::bytes &out_data) {
            std::string in_data_str = in_data;
            std::string out_data_str(out_data);

            const void *in_data_ptr = in_data_str.data();
            void *out_data_ptr = &out_data_str[0];

            int32_t ret =
                iCVFrameWorkGetResult(inst, in_data_ptr, out_data_ptr);
            out_data = py::bytes(out_data_str);
            return ret;
        },
        py::arg("inst"), py::arg("in_data"), py::arg("out_data"));

    m.def(
        "iCVFrameWorkResourceAdd",
        [](RESTYPE res_type, const std::string &res_path) {
            return iCVFrameWorkResourceAdd(res_type, res_path.c_str());
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
        return std::make_pair(ret, std::string(expired_time));
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
        return std::make_pair(ret, std::string(engine_version));
    });
}
