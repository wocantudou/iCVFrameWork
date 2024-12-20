import os
current_file_path = os.path.abspath(__file__)
current_directory = os.path.dirname(current_file_path)
pythonlib_path = os.path.join(current_directory, '../output/bin/x86_64_linux/')
cfg_file = os.path.join(current_directory, '..//output/cfg/iCVFrameWork/iCVFrameWork.cfg')
fd_file = os.path.join(current_directory, '..//output/resources/model/mnn/fd.mnn')
print("pythonlib_path所在的目录:", pythonlib_path)
import sys
sys.path.append(pythonlib_path)
import iCVFrameWorkPY as icv

ret = icv.iCVFrameWorkInitialize(cfg_file)
print("iCVFrameWorkInitialize ret: ", ret)

res_type = icv.RESTYPE.RESTYPE_FACE_DETECT_CPU
ret = icv.iCVFrameWorkResourceAdd(res_type, fd_file)
print("iCVFrameWorkResourceAdd ret: ", ret)
ret = icv.iCVFrameWorkResourceDelete(res_type)
print("iCVFrameWorkResourceDelete ret: ", ret)

ret = icv.iCVFrameWorkUninitialize()
print("iCVFrameWorkUninitialize ret: ", ret)
