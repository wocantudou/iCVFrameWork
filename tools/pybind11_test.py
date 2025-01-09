import logging
from typing import Optional, Tuple, List
import os
import sys
from datetime import datetime
# current_file_path = os.path.abspath(__file__)
current_directory = os.path.dirname(os.path.abspath(__file__))
iCVFrameWorkPY_path = os.path.join(current_directory, '../output/bin/x86_64_linux/')
print("iCVFrameWorkPY_path所在的目录:", iCVFrameWorkPY_path)
import sys
sys.path.append(iCVFrameWorkPY_path)
import iCVFrameWorkPY as icv

# 配置日志
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)

def initialize_framework(config_file: str) -> Tuple[bool, Optional[int]]:
    """
    初始化iCV框架
    
    Args:
        config_file: 配置文件路径
        
    Returns:
        bool: 初始化是否成功
    """
    try:
        start_time = datetime.now()
        logger.info(f"Initializing framework with config: {config_file}")
        
        ret = icv.iCVFrameWorkInitialize(config_file)
        if ret != 0:
            logger.error(f"Failed to initialize framework, error code: {ret}")
            return False
            
        duration = (datetime.now() - start_time).total_seconds()
        logger.info(f"Framework initialized successfully in {duration:.2f} seconds")
        return True
        
    except Exception as e:
        logger.exception(f"Framework initialization failed: {str(e)}")
        return False

def version_info_framework() -> Tuple[bool, Optional[Tuple[str, str]]]:
    """
    测试版本信息获取功能
    
    Returns:
        bool: 测试是否成功
    """
    try:
        # 获取引擎版本
        ret, engine_version = icv.iCVFrameWorkGetEngineVersion()
        if ret != 0:
            logger.error(f"Failed to get engine version, error code: {ret}")
            return False
        logger.info(f"Engine version: {engine_version}")

        # 获取过期时间
        ret, expired_time = icv.iCVFrameWorkGetExpiredTime()
        if ret != 0:
            logger.error(f"Failed to get expired time, error code: {ret}")
            return False
        logger.info(f"Expired time: {expired_time}")

        return True
    except Exception as e:
        logger.error(f"Version info test failed: {str(e)}")
        return False


def test_framework(config_file: str, model_file: str, image_path: str) -> Tuple[bool, Optional[List[str]]]:
    """
    测试实例管理功能
    
    Args:
        config_file: 配置文件路径
        model_file: 模型文件路径
        
    Returns:
        bool: 测试是否成功
    """
    
    # 初始化框架
    if not initialize_framework(config_file):
        return False
    try:
        # 测试版本信息获取功能
        if not version_info_framework():
            return False
        logger.info("Framework version info successfully")
        # 加载资源
        ret = icv.iCVFrameWorkResourceAdd(icv.RESTYPE_FACE_DETECT_CPU, model_file)
        if ret != 0:
            logger.error(f"Failed to add resource, error code: {ret}")
            return False
        logger.info("Resource added successfully")

        # 获取资源版本
        ret, res_version = icv.iCVFrameWorkGetResVersion(icv.RESTYPE_FACE_DETECT_CPU)
        if ret != 0:
            logger.error(f"Failed to get resource version, error code: {ret}")
            return False
        logger.info(f"Resource version: {res_version}")

        # 创建实例
        ret, inst = icv.iCVFrameWorkCreateInst(icv.RESTYPE_FACE_DETECT_CPU)
        if ret != 0:
            logger.error(f"Failed to create instance, error code: {ret}")
            return False
        logger.info("Instance created successfully")

        # 设置参数
        ret = icv.iCVFrameWorkSetParameter(inst, "detect_thres", "0.6")
        if ret != 0:
            logger.error(f"Failed to set parameter, error code: {ret}")
            return False
        logger.info("Parameter set successfully")

        # 获取参数
        ret, value = icv.iCVFrameWorkGetParameter(inst, "detect_thres", 32)
        if ret != 0:
            logger.error(f"Failed to get parameter, error code: {ret}")
            return False
        logger.info(f"Got parameter value: {value}")

        # 重置实例
        ret = icv.iCVFrameWorkResetInst(inst)
        if ret != 0:
            logger.error(f"Failed to reset instance, error code: {ret}")
            return False
        logger.info("Instance reset successfully")

        # 读取图像
        import cv2
        img = cv2.imread(image_path)
        if img is None:
            logger.error(f"Failed to read image: {image_path}")
            return False
            
        # 准备输入数据
        in_data = icv.ICVBaseDataIn()
        in_data.data_stat = icv.ICV_BASE_DATA_STAT_IMG
        in_data.img.h = img.shape[0]
        in_data.img.w = img.shape[1]
        in_data.img.fmt = icv.ICV_BASE_IMG_FMT_BGR
        # 将numpy数组转换为字节字符串
        img_bytes = img.tobytes()
        # 直接将字节字符串赋值给data字段
        in_data.img.data = img_bytes
        in_data.img.data_byte_size = len(img_bytes)  # 获取字节字符串长度
        in_data.img.next = None
        
        # 初始化输出数据
        out_data = icv.ICVFrameWorkFaces()
        
        # 处理图像
        logger.info("iCVFrameWorkGetResult")
        ret = icv.iCVFrameWorkGetResult(inst, in_data, out_data)
        if ret != 0:
            logger.error(f"Failed to process image, error code: {ret}")
            return False
        logger.info("iCVFrameWorkGetResult successfully")    
        # 解析并显示结果
        if out_data.face_num > 0:
            logger.info(f"Detected {out_data.face_num} faces")
            
        else:
            logger.info("No faces detected")

        return True
    finally:
        # 确保框架被正确释放
        # 销毁实例
        ret = icv.iCVFrameWorkDestroyInst(inst)
        if ret != 0:
            logger.error(f"Failed to destroy instance, error code: {ret}")
            return False
        logger.info("Instance destroyed successfully")

        # 删除资源
        ret = icv.iCVFrameWorkResourceDelete(icv.RESTYPE_FACE_DETECT_CPU)
        if ret != 0:
            logger.error(f"Failed to delete resource, error code: {ret}")
            return False
        logger.info("Resource deleted successfully")
        # 释放框架
        ret = icv.iCVFrameWorkUninitialize()
        if ret != 0:
            logger.error(f"Failed to uninitialize framework, error code: {ret}")
        else:
            logger.info("Framework uninitialized successfully")


if __name__ == "__main__":
    # 示例配置文件路径
    config_file = os.path.join(current_directory, '..//output/cfg/iCVFrameWork/iCVFrameWork.cfg')
    # 示例模型文件路径
    model_file = os.path.join(current_directory, '..//output/resources/model/mnn/fd.mnn')
    # 示例图像路径
    image_file = os.path.join(current_directory, '..//output/resources/data/76.bmp')
    
    # 运行所有测试
    test_results = []
    
    # 测试框架
    test_results.append(("Test Framework", 
                        test_framework(config_file, model_file, image_file)))
