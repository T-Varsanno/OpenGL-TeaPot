#pragma once
#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void drawCameraViewTriangle(const glm::vec3& position, const glm::vec3& direction, int flag);
void drawCameraPath();
struct CameraState saveCameraState();
void loadNextCameraState();
void updateCamera();
void loadCameraWithParam(glm::vec3 calculatedPos, float calcYaw, float calcPitch);
struct CameraState calculateEstimate();


#endif // CAMERA_UTILS_H