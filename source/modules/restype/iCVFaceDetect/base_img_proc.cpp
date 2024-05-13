#include "base_img_proc.h"


namespace DNNCVLIB 
{
	#define DIST_COEFFICIENTS_SIZE			(5)
	#define CAMERA_MATRIX_SIZE				(9)
	#define ALIGN_POINTS_SIZE			    (68)


	std::vector<std::string> split(std::string str, const std::string & pattern)
	{
		std::string::size_type pos = str.rfind(pattern);
		if (pos + pattern.size() != str.size())
		{
			str += pattern;
		}
		std::vector<std::string> result;
		std::string::size_type size = str.size();
		for (std::string::size_type i = 0; i < size; ++i)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				result.push_back(str.substr(i, pos - i));
				i = pos + pattern.size() - 1;
			}
		}
		return result;
	}

    int resize_uniform(const cv::Mat& src, cv::Mat& dst, cv::Size dst_size, cv::Rect2f& effect_area)
	{
		int w = src.cols;
		int h = src.rows;
		int dst_w = dst_size.width;
		int dst_h = dst_size.height;
		
		dst = cv::Mat(cv::Size(dst_w, dst_h), CV_8UC3, cv::Scalar(0));

		float ratio_src = w * 1.0 / h;
		float ratio_dst = dst_w * 1.0 / dst_h;

		int tmp_w = 0;
		int tmp_h = 0;
		if (ratio_src > ratio_dst) 
		{
			tmp_w = dst_w;
			tmp_h = floor((dst_w * 1.0 / w) * h);
		}
		else if (ratio_src < ratio_dst) 
		{
			tmp_h = dst_h;
			tmp_w = floor((dst_h * 1.0 / h) * w);
		}
		else {
			cv::resize(src, dst, dst_size);
			effect_area.x = 0;
			effect_area.y = 0;
			effect_area.width = dst_w;
			effect_area.height = dst_h;
			return 0;
		}

		cv::Mat tmp;
		cv::resize(src, tmp, cv::Size(tmp_w, tmp_h));

		if (tmp_w != dst_w) 
		{
			int index_w = floor((dst_w - tmp_w) / 2.0);
			for (int i = 0; i < dst_h; i++) {
				memcpy(dst.data + i * dst_w * 3 + index_w * 3, tmp.data + i * tmp_w * 3, tmp_w * 3);
			}
			effect_area.x = index_w;
			effect_area.y = 0;
			effect_area.width = tmp_w;
			effect_area.height = tmp_h;
		}
		else if (tmp_h != dst_h) 
		{
			int index_h = floor((dst_h - tmp_h) / 2.0);
			memcpy(dst.data + index_h * dst_w * 3, tmp.data, tmp_w * tmp_h * 3);
			effect_area.x = 0;
			effect_area.y = index_h;
			effect_area.width = tmp_w;
			effect_area.height = tmp_h;
		}
		return 0;
	}

	int warpPerspective_uniform(const cv::Mat& src, cv::Mat& dst, cv::Size dst_size, cv::Mat& warp_matrix)
	{
		cv::warpPerspective(src, dst, warp_matrix, dst_size);

		return 0;
	}

	void calc_warpPerspective_mat(const cv::Size& src_size, const cv::Size& dst_size, cv::Mat& warp_matrix, cv::Rect2f& effect_area, bool keep_ratio)
	{
		int src_w = src_size.width;
		int src_h = src_size.height;

		int dst_w = dst_size.width;
		int dst_h = dst_size.height;

		cv::Mat_<float>&& ResizeM = cv::Mat_<float>::eye(3, 3);
		if(keep_ratio)
		{
			float ratio = 1.0;

			float ratio_src = src_w * 1.0 / src_h;
			float ratio_dst = dst_w * 1.0 / dst_h;

			if (ratio_src > ratio_dst) 
			{
				ratio = dst_w * 1.0 / src_w;
			}
			else if (ratio_src < ratio_dst) 
			{
				ratio = dst_h * 1.0 / src_h;
			}
			
			cv::Mat_<float>&& C = cv::Mat_<float>::eye(3, 3);
			cv::Mat_<float>&& R = cv::Mat_<float>::eye(3, 3);
			cv::Mat_<float>&& T = cv::Mat_<float>::eye(3, 3);

			C(0, 2) = -src_w * 0.5;
			C(1, 2) = -src_h * 0.5;

			R(0, 0) *= ratio;
			R(1, 1) *= ratio;

			T(0, 2) = dst_w * 0.5;
			T(1, 2) = dst_h * 0.5;

			ResizeM = T * R * C;
		}
		else
		{
			ResizeM(0, 0) *= dst_w * 1.0f / src_w;
			ResizeM(1, 1) *= dst_h * 1.0f / src_h;
		}
		
		warp_matrix = ResizeM.clone();

		effect_area.x = ResizeM(0, 2);
		effect_area.y = ResizeM(1, 2);
		effect_area.width = src_size.width * ResizeM(0, 0);
		effect_area.height = src_size.height * ResizeM(1, 1);
	}
   	
	void get_rotation_mat(const std::vector<cv::Point2f>& align_points, const cv::Mat &camera_matrix_mat, const cv::Mat & dist_coefficients_mat, const cv::Mat & tmpl_landmark_3d_mat_, cv::Mat& landmark_2d_mat, cv::Mat &rotation_mat, cv::Mat & model3d)
	{
		float rotation_vec[] = { 0.0, 0.0, 0.0 };
		float translation_vec[] = { 0.0, 0.0, 1.0 };

		cv::Mat rotation_vec_mat = cv::Mat(3, 1, CV_32FC1, rotation_vec);
		cv::Mat translation_vec_mat = cv::Mat(3, 1, CV_32FC1, translation_vec);

		const int point_size = align_points.size();
		float tmp_align_points[point_size * 2];
		for (int i = 0; i < point_size; ++i)
		{
			tmp_align_points[2 * i] = align_points[i].x;
			tmp_align_points[2 * i + 1] = align_points[i].y;
		}
		landmark_2d_mat = cv::Mat(point_size, 2, CV_32FC1, tmp_align_points).clone();
		
		cv::solvePnP(tmpl_landmark_3d_mat_, landmark_2d_mat, camera_matrix_mat, dist_coefficients_mat, rotation_vec_mat, translation_vec_mat, true, cv::SOLVEPNP_ITERATIVE);

		cv::Rodrigues(rotation_vec_mat, rotation_mat);

		model3d = tmpl_landmark_3d_mat_ * rotation_mat.t() + cv::Mat::ones(point_size, 1, CV_32FC1) * translation_vec_mat.t();

	}

	void get_projection_matrix(int distance_norm, cv::Mat &mouth_center, cv::Mat &rotation_mat, cv::Mat &camera_matrix_mat, cv::Mat &norm_camera_matrix_mat,cv::Mat &projection_matrix,cv::Mat&normalizing_rot)
	{
		float distance = cv::norm(mouth_center);
		cv::Mat z_axis = mouth_center / distance;
		cv::Mat head_x_axis = rotation_mat.colRange(0, 1).t();
		cv::Mat y_axis = z_axis.cross(head_x_axis) / cv::norm(z_axis.cross(head_x_axis));
		cv::Mat x_axis = y_axis.cross(z_axis) / cv::norm(y_axis.cross(z_axis));
		cv::vconcat(x_axis, y_axis, normalizing_rot);
		cv::vconcat(normalizing_rot, z_axis, normalizing_rot);
		cv::Mat camera_matrix_inv = camera_matrix_mat.inv();
		float scale[] = { 1, 0, 0, 0, 1, 0, 0, 0, distance_norm * 1.f/ 1000.f / distance };
		cv::Mat scale_mat = cv::Mat(3, 3, CV_32FC1, scale);
		cv::Mat conversion_matrix = scale_mat * normalizing_rot;
		projection_matrix = norm_camera_matrix_mat * conversion_matrix * camera_matrix_inv;
	}

	//https://www.learnopencv.com/rotation-matrix-to-euler-angles/
	// Calculates rotation matrix given euler angles.
	cv::Mat eulerAnglesToRotationMatrix(cv::Vec3f &theta)
	{
		// Calculate rotation about x axis
		cv::Mat R_x = (cv::Mat_<float>(3, 3) <<
			1, 0, 0,
			0, cos(theta.val[0]), -sin(theta.val[0]),
			0, sin(theta.val[0]), cos(theta.val[0])
			);

		// Calculate rotation about y axis
		cv::Mat R_y = (cv::Mat_<float>(3, 3) <<
			cos(theta.val[1]), 0, sin(theta.val[1]),
			0, 1, 0,
			-sin(theta.val[1]), 0, cos(theta.val[1])
			);

		// Calculate rotation about z axis
		cv::Mat R_z = (cv::Mat_<float>(3, 3) <<
			cos(theta.val[2]), -sin(theta.val[2]), 0,
			sin(theta.val[2]), cos(theta.val[2]), 0,
			0, 0, 1);
		// Combined rotation matrix
		cv::Mat R = R_z * R_y * R_x;

		return R;

	}

	
	bool isRotationMatrix(cv::Mat &R)
	{
		cv::Mat Rt;
		cv::transpose(R, Rt);
		cv::Mat shouldBeIdentity = Rt * R;
		cv::Mat I = cv::Mat::eye(3, 3, shouldBeIdentity.type());

		return  norm(I, shouldBeIdentity) < 1e-6;
	}

	// Calculates rotation matrix to euler angles
	// The result is the same as MATLAB except the order
	// of the euler angles ( x and z are swapped ).
	cv::Vec3f rotationMatrixToEulerAngles(cv::Mat &R)
	{
		//assert(isRotationMatrix(R));
		float sy = sqrt(R.at<float>(0, 0) * R.at<float>(0, 0) + R.at<float>(1, 0) * R.at<float>(1, 0));

		bool singular = sy < 1e-6; // If

		float x, y, z;
		if (!singular)
		{
			x = atan2(R.at<float>(2, 1), R.at<float>(2, 2));
			y = atan2(-R.at<float>(2, 0), sy);
			z = atan2(R.at<float>(1, 0), R.at<float>(0, 0));
		}
		else
		{
			x = atan2(-R.at<float>(1, 2), R.at<float>(1, 1));
			y = atan2(-R.at<float>(2, 0), sy);
			z = 0;
		}
		return cv::Vec3f(x, y, z);

	}

	//berner detection
	float berner_detection(const cv::Mat &gary_image)
	{
		float score = 0;
		for (int i = 0; i < gary_image.rows - 2; i++)
		{
			for (int j = 0; j<gary_image.cols - 2; j++)
			{
				float bias = (float(gary_image.ptr<uchar>(i + 2)[j]) - float(gary_image.ptr<uchar>(i)[j])) / 255.0;
				score += pow(bias, 2);
			}
		}
		return score;
	}

	float icao_brightness_detection(const cv::Mat &gray_face)
	{
		int face_h = gray_face.rows;
		int face_w = gray_face.cols;

		float drop_pixels_num = face_h * face_w * 0.01f;
		///定义函数需要的一些变量
		int grayImgNum = 1;                                 //图像数
		int grayChannels = 0;                              //需要计算的通道号 单通道只有 通道号为0
		cv::Mat grayHist;                                   //灰度图输出直方图
		const int grayHistDim = 1;                          //直方图维数
		const int grayHistSize = 256;                      //直方图每一维度bin个数
		float grayRanges[2] = { 0, 256 };                   //灰度值的统计范围
		const float *grayHistRanges[1] = { grayRanges };    //灰度值统计范围指针
		bool grayUniform = true;                            //是否均匀
		bool grayAccumulate = false;                        //是否累积

		//计算灰度图像的直方图
		cv::calcHist(&gray_face, grayImgNum, &grayChannels, cv::Mat()
			, grayHist, grayHistDim, &grayHistSize,
			grayHistRanges, grayUniform, grayAccumulate);


		int idx = 0;
		float d = drop_pixels_num;
		while (d > 0)
		{
			if (grayHist.at<float>(idx, 0) >= d)
			{
				grayHist.at<float>(idx, 0) -= d;
				d = 0;
			}
			else
			{
				d -= grayHist.at<float>(idx, 0);
				grayHist.at<float>(idx, 0) = 0;
				idx++;
			}
		}

		idx = grayHistSize - 1;
		d = drop_pixels_num;
		while (d > 0)
		{
			if (grayHist.at<float>(idx, 0) >= d)
			{
				grayHist.at<float>(idx, 0) -= d;
				d = 0;
			}
			else
			{
				d -= grayHist.at<float>(idx, 0);
				grayHist.at<float>(idx, 0) = 0;
				idx--;
			}
		}

		float numerator = 0.f;
		float denominator = 0.f;
		for (int i = 0; i < grayHistSize; ++i)
		{
			numerator += grayHist.at<float>(i, 0) * (i + 1);
			denominator += grayHist.at<float>(i, 0);
		}

		float i1 = numerator / denominator;

		idx = 0;
		while (grayHist.at<float>(idx, 0) == 0)
		{
			idx ++;
		}
		int min_level = idx;

		idx = grayHistSize - 1;
		while (grayHist.at<float>(idx, 0) == 0)
		{
			idx--;
		}
		int max_level = idx;
		float i2 = (max_level + min_level) / 2.f;

		float m = (i1 + i2) / 2.f;

		float score = 1 - fabs(grayHistSize / 2 - m) / (grayHistSize / 2);
		return score;
	}

	int sub_mean_and_divide_std(CHANNEL_SEPARATION_TYPE chl_sepr_type, cv::Mat& img, float* image_data, const float* mean_data, const float* std_data)
	{
		//NHWC
		if(BGRBGRBGRTORGBRGBRGB==chl_sepr_type)
		{
			float *forward_nhwc     = image_data;
			int    data_hw_size     = img.rows * img.cols;
			int    channel_size		= img.channels();
			unsigned char *src_data = (unsigned char *)img.data;
			float mean_b=0, mean_g=0, mean_r=0;
			if(mean_data)
			{
				if (channel_size > 1)
				{
					mean_r = mean_data[0];
					mean_g = mean_data[1];
				}
				mean_b = mean_data[2];
			}

			float std_b=1, std_g=1, std_r=1;
			if (std_data)
			{
				if (channel_size > 1)
				{
					if (0 == std_data[0])
					{
						printf("IFLYDSM_ERROR_INVALID_PARA_VALUE=19");
						return 19;
					}
					std_r = std_data[0];
					if (0 == std_data[1])
					{
						printf("IFLYDSM_ERROR_INVALID_PARA_VALUE=19");
						return 19;
					}
					std_g = std_data[1];
				}
				if (0 == std_data[2])
				{
					printf("IFLYDSM_ERROR_INVALID_PARA_VALUE=19");
					return 19;
				}
				std_b = std_data[2];
			}

			for(int i=0;i<data_hw_size;i++)
			{
				unsigned char *each_channel_data = src_data+i*channel_size;
				unsigned char *p_r 				 = each_channel_data+2;//r
				unsigned char *p_g 				 = each_channel_data+1;//g
				unsigned char *p_b 				 = each_channel_data+0;//b

				*forward_nhwc++ = (static_cast<float>(*p_r) - mean_r) / std_r;
				*forward_nhwc++ = (static_cast<float>(*p_g) - mean_g) / std_g;
				*forward_nhwc++ = (static_cast<float>(*p_b) - mean_b) / std_b;
			}
			return 0;
		}
		//HCHW
		if (!img.empty())
		{
			int size = img.rows * img.cols * img.channels();
			int dst_width = img.cols;
			int channels = img.channels();

			float* ptr_image_r = image_data;
			float* ptr_image_g = image_data + size / 3;
			float* ptr_image_b = image_data + size / 3 * 2;

			if (BGRBGRBGRTOBBBGGGRRR == chl_sepr_type || RGBRGBRGBTOBBBGGGRRR == chl_sepr_type)
			{
				ptr_image_b = image_data;
				ptr_image_g = image_data + size / 3;
				ptr_image_r = image_data + size / 3 * 2;
			}
			
			float mean_b, mean_g, mean_r;
			if (mean_data)
			{
				if (channels > 1)
				{
					mean_r = mean_data[0];
					mean_g = mean_data[1];
				}
				mean_b = mean_data[2];
			}
			else
			{
				mean_b = mean_g = mean_r = 0;
			}

			float std_b, std_g, std_r;
			if (std_data)
			{
				if (channels > 1)
				{
					if (0 == std_data[0])
					{
						printf("IFLYDSM_ERROR_INVALID_PARA_VALUE=19");
						return 19;
					}
					std_r = std_data[0];
					if (0 == std_data[1])
					{
						printf("IFLYDSM_ERROR_INVALID_PARA_VALUE=19");
						return 19;
					}
					std_g = std_data[1];
				}
				if (0 == std_data[2])
					{
						printf("IFLYDSM_ERROR_INVALID_PARA_VALUE=19");
						return 19;
					}
				std_b = std_data[2];
			}
			else
			{
				std_b = std_g = std_r = 1;
			}

			for (int i = 0; i < img.rows; i++)
			{
				unsigned char* data = img.ptr<unsigned char>(i);
				for (int j = 0; j < img.cols; j++)
				{
					if (BGRBGRBGRTORRRGGGBBB == chl_sepr_type || BGRBGRBGRTOBBBGGGRRR == chl_sepr_type)
					{
						if (channels > 1)
						{
							*ptr_image_b++ = (static_cast<float>(*data++) - mean_b) / std_b;
							*ptr_image_g++ = (static_cast<float>(*data++) - mean_g) / std_g;
						}
						*ptr_image_r++ = (static_cast<float>(*data++) - mean_r) / std_r;
					}
					else
					{
						if (channels > 1)
						{
							*ptr_image_r++ = (static_cast<float>(*data++) - mean_r) / std_r;
							*ptr_image_g++ = (static_cast<float>(*data++) - mean_g) / std_g;
						}
						*ptr_image_b++ = (static_cast<float>(*data++) - mean_b) / std_b;
					}
					
				}
			}
		}
		return 0;
	}

	//for gaze and eye
	void calc_3d_coordinate(cv::Mat point2d, cv::Mat point3d, cv::Mat camera_matrix_mat, cv::Mat& new_point3d)
	{
		float fx = camera_matrix_mat.at<float>(0, 0);
		float fy = camera_matrix_mat.at<float>(1, 1);
		float u0 = camera_matrix_mat.at<float>(0, 2);
		float v0 = camera_matrix_mat.at<float>(1, 2);
		new_point3d.at<float>(0, 0) = (point2d.at<float>(0, 0) - u0) * point3d.at<float>(0, 2) / fx;
		new_point3d.at<float>(0, 1) = (point2d.at<float>(0, 1) - v0) * point3d.at<float>(0, 2) / fy;
		new_point3d.at<float>(0, 2) = point3d.at<float>(0, 2);
	}


/***************************new for image quality*************************************/
double calculate_contrast_score(cv::Mat& img) {
    cv::Mat gray_img;
    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);

    cv::Mat blurred_img;
    GaussianBlur(gray_img, blurred_img, cv::Size(5, 5), 0);

    cv::Mat edge;
    Canny(blurred_img, edge, 50, 150);

    cv::Scalar mean_value = cv::mean(edge);

    double contrast_score = mean_value.val[0] / 255.0 * 10;

    return contrast_score;
}

double calculate_saturation_score(cv::Mat& img) {
    cv::Mat hsv_img;
    cv::cvtColor(img, hsv_img, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv_img, channels);

    cv::Scalar mean_value = cv::mean(channels[1]);

    double saturation_score = mean_value.val[0] / 255.0 * 10;

    return saturation_score;
}

double calculate_clarity_score(cv::Mat& img) {
    cv::Mat gray_img;
    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);

    cv::Mat blurred_img;
    cv::GaussianBlur(gray_img, blurred_img, cv::Size(5, 5), 0);

    cv::Mat laplacian_img;
    cv::Laplacian(blurred_img, laplacian_img, CV_16S, 3);

    cv::Scalar mean_value = cv::mean(cv::abs(laplacian_img));

    double clarity_score = mean_value.val[0] / 255.0 * 10;

    return clarity_score;
}


double quantitative_evaluation(cv::Mat& img) {
    cv::Mat gray_img;
    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
    cv::Scalar mean_value = cv::mean(gray_img);

    double brightness_score = mean_value.val[0] / 255.0 * 10;

    double contrast_score = calculate_contrast_score(img);

    double saturation_score = calculate_saturation_score(img);

    double clarity_score = calculate_clarity_score(img);

    double score = (brightness_score + contrast_score + saturation_score + clarity_score) / 4;

    return score;
}


}