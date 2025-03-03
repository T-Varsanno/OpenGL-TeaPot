#include "pnp.h"
#include "globals.h"

// Define your virtual camera intrinsic parameters
float fov = 45.0f; // Field of view in degrees
float aspectRatio = static_cast<float>(WIDTH / 2) / HEIGHT;
float fovRad = glm::radians(fov);
float fy = (HEIGHT / 2.0f) / tan(fovRad / 2.0f);
float fx = fy * aspectRatio;
const float cx = WIDTH / 4;     //center x of right side viewpoint
const float cy = HEIGHT / 2;    //center y of right side viewpoint


// Convert glm::vec2 to cv::Point2f
std::vector<cv::Point2f> convertToCvPoint2f(const std::vector<glm::vec2>& points) {
    std::vector<cv::Point2f> cvPoints;
    for (const auto& point : points) {
        cvPoints.emplace_back(point.x, point.y);
    }
    return cvPoints;
}

// Convert glm::vec3 to cv::Point3f
std::vector<cv::Point3f> convertToCvPoint3f(const std::vector<glm::vec3>& points) {
    std::vector<cv::Point3f> cvPoints;
    for (const auto& point : points) {
        cvPoints.emplace_back(point.x, point.y, point.z);
    }
    return cvPoints;
}

// takes 2 vectors of 2d and 3d points from the myClickData struct, returns the camera position, yaw and pitch after PNP
std::pair<glm::vec3, glm::vec2> estimateCameraPose() {
    std::vector<cv::Point2f> imagePoints = convertToCvPoint2f(myClickData.leftSideClicks);
    std::vector<cv::Point3f> objectPoints = convertToCvPoint3f(myClickData.rightSideClicks);

    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
    cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F); // Assuming no distortion

    cv::Mat rvec, tvec;
    bool success = cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec, false, cv::SOLVEPNP_EPNP); // cv::SOLVEPNP_EPNP or cv::SOLVEPNP_ITERATIVE in the last argument
    if (!success) {
        std::cerr << "solvePnP failed to find a solution." << std::endl;
        return { glm::vec3(0.0f), glm::vec2(0.0f, 0.0f) };
    }

    cv::Mat R;
    cv::Rodrigues(rvec, R);
    cv::Mat R2;
    cv::Rodrigues(rvec, R2);
    cv::Mat adjustY = (cv::Mat_<double>(3, 3) << 1, 0, 0,
        0, -1, 0,
        0, 0, 1);
    R = adjustY * R;

    glm::mat3 rotationMatrix(
        R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2),
        R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2),
        R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2)
    );

    glm::vec3 translation(
        tvec.at<double>(0),
        tvec.at<double>(1),
        tvec.at<double>(2)
    );

    glm::vec3 cameraPosition = -glm::transpose(rotationMatrix) * translation;
    glm::vec3 front;
    front.x = rotationMatrix[2][0];
    front.y = rotationMatrix[2][1];
    front.z = rotationMatrix[2][2];
    front = glm::normalize(front);

    float yaw1 = glm::degrees(atan2(front.z, front.x));
    float pitch1 = glm::degrees(asin(front.y));

    if (cameraPosition.z < 0) {
        glm::mat3 rotationMatrix1(
            R2.at<double>(0, 0), R2.at<double>(0, 1), R2.at<double>(0, 2),
            R2.at<double>(1, 0), R2.at<double>(1, 1), R2.at<double>(1, 2),
            R2.at<double>(2, 0), R2.at<double>(2, 1), R2.at<double>(2, 2)
        );

        glm::vec3 translation(
            tvec.at<double>(0),
            tvec.at<double>(1),
            tvec.at<double>(2)
        );

        cameraPosition = -glm::transpose(rotationMatrix1) * translation;
        
        front.x = rotationMatrix1[2][0];
        front.y = rotationMatrix1[2][1];
        front.z = rotationMatrix1[2][2];
        front = glm::normalize(front);

        yaw1 = glm::degrees(atan2(front.z, front.x));
        pitch1 = glm::degrees(asin(front.y));
    }
    // Calculate yaw and pitch from the rotation matrix

    std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
    std::cout << "Yaw: " << yaw1 << std::endl;
    std::cout << "Pitch: " << pitch1 << std::endl;

    return { cameraPosition, glm::vec2(yaw1, pitch1) };
}