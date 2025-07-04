// 定义默认配置
// 格式DEFINE_DEFAULT_CONFIG(Type, Section, Key, DefaultValue)
DEFINE_DEFAULT_CONFIG_ITEM(int, iCVFrameWork, None, 0) // begin
DEFINE_DEFAULT_CONFIG_ITEM(std::string, iCVFrameWork, LogPath,
                           "../../log/iCVFrameWork.log")
DEFINE_DEFAULT_CONFIG_ITEM(std::string, iCVFrameWork, FaceDetectResPath,
                           "../../resources/model/mnn/fd.mnn")
DEFINE_DEFAULT_CONFIG_ITEM(int, iCVFrameWork, AIEngineHardWareType, 0)
DEFINE_DEFAULT_CONFIG_ITEM(int, iCVFrameWork, AIEngineNumThread, 1)
DEFINE_DEFAULT_CONFIG_ITEM(int, iCVFrameWork, AIEngineMaxBatchSize, 1)
DEFINE_DEFAULT_CONFIG_ITEM(int, iCVFrameWork, AIEngineDeviceID, 0)
DEFINE_DEFAULT_CONFIG_ITEM(int, iCVFrameWork, Count, 0) // end