#ifndef __FD_NMS_HPP__
#define __FD_NMS_HPP__

// https ://github.com/Nuzhny007/Non-Maximum-Suppression

#include <opencv2/opencv.hpp>
//#include <assert.h>

typedef struct _NMSOutData {
    cv::Rect2f rect;
    float score;
} NMSOutData;

/**
 * @brief nms
 * Non maximum suppression
 * @param src_rects
 * @param res_rects
 * @param iou_thres
 * @param neighbors
 */
inline int nms(const std::vector<cv::Rect2f> &src_rects,
               std::vector<cv::Rect2f> &res_rects, float iou_thres,
               int neighbors = 0) {
    int ret = 0;
    res_rects.clear();

    const size_t size = src_rects.size();
    if (!size) {
        // printf("no_face_detect ...\n");
        return ret;
    }

    // Sort the bounding boxes by the bottom - right y - coordinate of the
    // bounding box
    std::multimap<int, size_t> idxs;
    for (size_t i = 0; i < size; ++i) {
        idxs.insert(std::pair<int, size_t>(src_rects[i].br().y, i));
    }

    // keep looping while some indexes still remain in the indexes list
    while (idxs.size() > 0) {
        // grab the last rectangle
        auto last_elem = --std::end(idxs);
        const cv::Rect2f &main_rect = src_rects[last_elem->second];

        int neigbors_cnt = 0;

        idxs.erase(last_elem);

        for (auto pos = std::begin(idxs); pos != std::end(idxs);) {
            // grab the current rectangle
            const cv::Rect2f &query_rect = src_rects[pos->second];

            float int_area = (main_rect & query_rect).area();
            float union_area = main_rect.area() + query_rect.area() - int_area;
            float overlap = int_area / union_area;

            // if there is sufficient overlap, suppress the current bounding box
            if (overlap > iou_thres) {
                pos = idxs.erase(pos);
                ++neigbors_cnt;
            } else {
                ++pos;
            }
        }
        if (neigbors_cnt >= neighbors) {
            res_rects.push_back(main_rect);
        }
    }
    return ret;
}

/**
 * @brief nms2
 * Non maximum suppression with detection scores
 * @param src_rects
 * @param scores
 * @param res_rects
 * @param iou_thres
 * @param neighbors
 */
inline int nms2(const std::vector<cv::Rect2f> &src_rects,
                const std::vector<float> &scores, float iou_thres,
                int max_init_nms_cnt, std::vector<NMSOutData> &nms_res,
                int neighbors = 0, float min_scores_sum = 0.f) {
    int ret = 0;
    nms_res.clear();

    const size_t size = src_rects.size();
    if (0 == size) {
        // printf("no_face_detect ...\n");
        return ret;
    }

    if (src_rects.size() != scores.size()) {
        std::cerr << "src_rects.size() = " << src_rects.size()
                  << " != scores.size() = " << scores.size() << std::endl;
        return -1;
    }

    // Sort the bounding boxes by the detection score
    std::multimap<float, size_t> idxs;
    for (size_t i = 0; i < size; ++i) {
        idxs.insert(std::pair<float, size_t>(scores[i], i));
    }

    if (size > max_init_nms_cnt) {
        int del_nms_cnt = size - max_init_nms_cnt;
        int del_idx = 0;
        // for (std::multimap<float, size_t>::iterator iter = idxs.begin(); iter
        // != idxs.end(); )
        for (auto iter = std::begin(idxs); iter != std::end(idxs);) {
            if (del_idx >= del_nms_cnt) {
                break;
            }
            iter = idxs.erase(iter);
            del_idx++;
        }
    }

    // keep looping while some indexes still remain in the indexes list
    while (idxs.size() > 0) {
        // grab the last rectangle
        auto last_elem = --std::end(idxs);
        const cv::Rect2f &main_rect = src_rects[last_elem->second];

        int neigbors_cnt = 0;
        float rect1_score = last_elem->first;
        float scores_sum = last_elem->first;

        idxs.erase(last_elem);

        for (auto pos = std::begin(idxs); pos != std::end(idxs);) {
            // grab the current rectangle
            const cv::Rect2f &query_rect = src_rects[pos->second];

            float int_area = (main_rect & query_rect).area();
            float union_area = main_rect.area() + query_rect.area() - int_area;
            float overlap = int_area / union_area;

            // if there is sufficient overlap, suppress the current bounding box
            if (overlap > iou_thres) {
                scores_sum += pos->first;
                pos = idxs.erase(pos);
                ++neigbors_cnt;
            } else {
                ++pos;
            }
        }
        if (neigbors_cnt >= neighbors && scores_sum >= min_scores_sum) {
            NMSOutData nms_out_data;
            nms_out_data.rect = main_rect;
            nms_out_data.score = rect1_score;
            nms_res.push_back(nms_out_data);
        }
    }
    return ret;
}
#endif //__FD_NMS_HPP__