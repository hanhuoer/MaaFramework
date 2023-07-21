#pragma once

#include "Common/MaaConf.h"
#include "Utils/NoWarningCVMat.h"

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <meojson/json.hpp>

#include "Vision/VisionTypes.h"

#define MAA_PIPELINE_RES_NS MAA_RES_NS::Pipeline
#define MAA_PIPELINE_RES_NS_BEGIN \
    namespace MAA_PIPELINE_RES_NS \
    {
#define MAA_PIPELINE_RES_NS_END }

MAA_PIPELINE_RES_NS_BEGIN

namespace Recognition
{
enum class Type
{
    Invalid = 0,
    DirectHit,
    TemplateMatch,
    OCR,
    Custom,
};

using Param = std::variant<std::monostate, MAA_VISION_NS::DirectHitParam, MAA_VISION_NS::TemplMatchingParam,
                           MAA_VISION_NS::OcrParam, MAA_VISION_NS::CustomParam>;
} // namespace Recognition

namespace Action
{
enum class Type
{
    Invalid = 0,
    DoNothing,
    Click,
    Swipe,
    Key,
    StartApp,
    StopApp,
    Custom,
    // InputText, // TODO
};

enum class Target
{
    Invalid = 0,
    Self,
    PreTask,
    Region,
};

using TargetParam = std::variant<std::monostate, std::string, cv::Rect>;

struct ClickParam
{
    Target target = Target::Self;
    TargetParam target_param;
};

struct SwipeParam
{
    Target begin = Target::Self;
    TargetParam begin_param;
    Target end = Target::Self;
    TargetParam end_param;

    uint duration = 200;
};

struct KeyParam
{
    std::vector<int> keys;
};

struct AppParam
{
    std::string package;
};

struct CustomParam
{
    std::string name;
    json::value custom_param;
};

using Param = std::variant<std::monostate, ClickParam, SwipeParam, KeyParam, AppParam, CustomParam>;
} // namespace Action

struct WaitFreezesParam
{
    std::chrono::milliseconds time = std::chrono::milliseconds(0);

    Action::Target target = Action::Target::Self;
    Action::TargetParam target_param;

    double threshold = 0.95;
    int method = MAA_VISION_NS::TemplMatchingParam::kDefaultMethod;
};

struct TaskData
{
    using NextList = std::vector<std::string>;

    std::string name;
    bool is_sub = false;

    Recognition::Type rec_type = Recognition::Type::DirectHit;
    Recognition::Param rec_param = MAA_VISION_NS::DirectHitParam {};

    bool cache = false;

    Action::Type action_type = Action::Type::DoNothing;
    Action::Param action_param;
    NextList next;

    std::chrono::milliseconds timeout = std::chrono::milliseconds(20 * 1000);
    NextList timeout_next;

    uint times_limit = UINT_MAX;
    NextList runout_next;

    std::chrono::milliseconds pre_delay = std::chrono::milliseconds(200);
    std::chrono::milliseconds post_delay = std::chrono::milliseconds(500);

    WaitFreezesParam pre_wait_freezes;
    WaitFreezesParam post_wait_freezes;

    bool notify = false;
};

MAA_PIPELINE_RES_NS_END
