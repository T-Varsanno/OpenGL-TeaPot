#pragma once

#ifndef PNP_H
#define PNP_H
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


std::vector<cv::Point2f> convertToCvPoint2f(const std::vector<glm::vec2>& points);
std::vector<cv::Point3f> convertToCvPoint3f(const std::vector<glm::vec3>& points);
std::pair<glm::vec3, glm::vec2> estimateCameraPose();

#endif // PNP_H
