
//
// Create by RangiLyu
// 2020 / 10 / 2
//

#ifndef __NANODET_DECODE_HPP__
#define __NANODET_DECODE_HPP__
#include <opencv2/core/core.hpp>
#include <vector>

//#define CLS_PRED_LEN		(1)
#define DIS_PRED_LEN (32)
#define MMYOLO_DIS_PRED_LEN (4)
#define NUM_CLASS (1) // 80

#define INPUT_SIZE (160)
#define REG_MAX (7)

typedef struct _ClsPred {
    float cls_preds[NUM_CLASS];
    // std::vector<float> cls_preds;
} ClsPred;

typedef struct _DisPred {
    float dis_preds[DIS_PRED_LEN];
    // std::vector<float> dis_preds;
} DisPred;

typedef struct _MmyoloDisPred {
    float dis_preds[MMYOLO_DIS_PRED_LEN];
    // std::vector<float> dis_preds;
} MmyoloDisPred;

typedef struct _HeadInfo {
    std::string cls_layer;
    std::string dis_layer;
    int stride;
} HeadInfo;

typedef struct _BoxInfo {
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;
} BoxInfo;

inline float fast_exp(float x) {
    union {
        uint32_t i;
        float f;
    } v{};
    v.i = (1 << 23) * (1.4426950409 * x + 126.93490512f);
    return v.f;
}

template <typename _Tp>
int activation_function_softmax(const _Tp *src, _Tp *dst, int length) {
    const _Tp alpha = *std::max_element(src, src + length);
    _Tp denominator{0};

    for (int i = 0; i < length; ++i) {
        dst[i] = fast_exp(src[i] - alpha);
        denominator += dst[i];
    }

    for (int i = 0; i < length; ++i) {
        dst[i] /= denominator;
    }

    return 0;
}

BoxInfo MmyolodisPred2Bbox(int input_h, int input_w, const float *&dfl_det,
                           int label, float score, int x, int y, int stride) {
    float ct_x = x * stride;
    float ct_y = y * stride;
    std::vector<float> dis_pred;
    dis_pred.resize(4);
    for (int i = 0; i < 4; i++) {
        float dis = dfl_det[i] * stride;
        dis_pred[i] = dis;
    }
    float xmin = (std::max)(ct_x - dis_pred[0], .0f);
    float ymin = (std::max)(ct_y - dis_pred[1], .0f);
    float xmax = (std::min)(ct_x + dis_pred[2], (float)input_w);
    float ymax = (std::min)(ct_y + dis_pred[3], (float)input_h);

    return BoxInfo{xmin, ymin, xmax, ymax, score, label};
}

void mmyolo_decode_infer(int input_h, int input_w,
                         std::vector<ClsPred> &cls_pred_vec,
                         std::vector<MmyoloDisPred> &dis_pred_vec, int stride,
                         float threshold,
                         std::vector<std::vector<BoxInfo>> &results) {
    int feature_h = input_h / stride;
    int feature_w = input_w / stride;
    for (int idx = 0; idx < feature_h * feature_w; idx++) {
        const float *scores = cls_pred_vec[idx].cls_preds;
        int row = idx / feature_w;
        int col = idx % feature_w;
        float score = 0;
        int cur_label = 0;
        for (int label = 0; label < NUM_CLASS; label++) {
            if (scores[label] > score) {
                score = scores[label];
                cur_label = label;
            }
        }
        if (score > threshold) {
            const float *bbox_pred = dis_pred_vec[idx].dis_preds;
            results[cur_label].push_back(
                MmyolodisPred2Bbox(input_h, input_w, bbox_pred, cur_label,
                                   score, col, row, stride));
        }
    }
}

#endif //__NANODET_DECODE_HPP__
