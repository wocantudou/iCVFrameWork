#ifndef __BASE_IMG_PROC_H__
#define __BASE_IMG_PROC_H__

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

namespace DNNCVLIB 
{
	enum CHANNEL_SEPARATION_TYPE
	{
		CHANNEL_SEPARATION_TYPE_BEGIN,
		BGRBGRBGRTORRRGGGBBB = 1,
		BGRBGRBGRTOBBBGGGRRR,
		RGBRGBRGBTORRRGGGBBB,
		RGBRGBRGBTOBBBGGGRRR,
		BGRBGRBGRTORGBRGBRGB,
		CHANNEL_SEPARATION_TYPE_END,
	};
	
	 /******************************************************************************************************************
     * Description: This function performs the resize operation in proportion to obtain the region of interest;
     *       Input: src->original image, dst->target image, dst_size->Length and width of target image,effect_area->
	 *              Effective area of the original image after scaling; 
     *      Return: ret,Used to specify whether the function call is correct
     * ******************************************************************************************************************/
    int resize_uniform(
		const cv::Mat& src,
		cv::Mat& dst, 
		cv::Size dst_size, 
		cv::Rect2f& effect_area);

	/******************************************************************************************************************
     * Description: This function performs the warpPerspective operation in proportion to obtain the region of interest;
     *       Input: src->original image, dst->target image, dst_size->height and width of target image,effect_area->
	 *              Effective area of the original image after scaling; 
     *      Return: ret,Used to specify whether the function call is correct
     * ******************************************************************************************************************/
	int warpPerspective_uniform(
		const cv::Mat& src,
	 	cv::Mat& dst, 
		cv::Size dst_size,
		cv::Mat& warp_matrix);

	void calc_warpPerspective_mat(
		const cv::Size& src_size, 
		const cv::Size& dst_size, 
		cv::Mat& warp_matrix,
		cv::Rect2f& effect_area,
		bool keep_ratio = true);

	void get_rotation_mat(
		const std::vector<cv::Point2f>& align_points,
		const cv::Mat & camera_matrix_mat,
		const cv::Mat & dist_coefficients_mat,
		const cv::Mat & tmpl_landmark_3d_mat_,
		cv::Mat & landmark_2d_mat,
		cv::Mat & rotation_mat,
		cv::Mat & model3d);

	void get_projection_matrix(
		int distance_norm,
		cv::Mat &mouth_center, 
		cv::Mat &rotation_mat, 
		cv::Mat &camera_matrix_mat, 
		cv::Mat &norm_camera_matrix_mat,
		cv::Mat &projection_matrix,
		cv::Mat &normalizing_rot);

	cv::Mat eulerAnglesToRotationMatrix(cv::Vec3f &theta);

	bool isRotationMatrix(cv::Mat &R);
	
	cv::Vec3f rotationMatrixToEulerAngles(cv::Mat &R);

	float berner_detection(const cv::Mat &gary_image);

	float icao_brightness_detection(const cv::Mat &gray_face);

	int sub_mean_and_divide_std(CHANNEL_SEPARATION_TYPE chl_sepr_type, cv::Mat& img, float* image_data, const float* mean_data, const float* std_data);

	void calc_3d_coordinate(cv::Mat point2d, cv::Mat point3d, cv::Mat camera_matrix_mat, cv::Mat& new_point3d);
	
	/***************************new for image quality*************************************/
	double quantitative_evaluation(cv::Mat& img);

	std::vector<std::string> split(std::string str, const std::string & pattern);
}

#endif //__BASE_IMG_PROC_H__