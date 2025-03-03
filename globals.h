#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

// Window dimensions
extern const int WIDTH;
extern const int HEIGHT;

// Camera transformation
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

// Camera rotation
extern float yaw;
extern float pitch;

// Saved camera state
extern glm::vec3 savedCameraPos;
extern glm::vec3 savedCameraFront;
extern float savedYaw;
extern float savedPitch;

// Object transformation
extern glm::vec3 objectPos;
extern float objectRotationX;
extern float objectRotationY;

// Image storage
extern std::vector<unsigned char> image;

struct CameraState {
    glm::vec3 position;
    glm::vec3 front;
    float yaw;
    float pitch;
    std::vector<unsigned char> image;
};

struct KeyState {
    bool W = false;
    bool A = false;
    bool S = false;
    bool D = false;
    bool Left = false;
    bool Right = false;
    bool Up = false;
    bool Down = false;
};


extern KeyState keyState;


// Saved camera states
extern std::vector<CameraState> savedCameraStates;
extern int currentCameraStateIndex;
extern bool loadState;
extern bool pickingState;
extern bool chooseState;

extern std::vector<glm::vec3> cameraPath;

extern CameraState lastState;
extern CameraState lastPickingState;
extern CameraState cameraPnpSol;

// Click data structure
struct ClickData {
    std::vector<glm::vec2> leftSideClicks;
    std::vector<glm::vec3> rightSideClicks;
    int numLeft = 0;
    int numRight = 0;

    void addLeftClick(double xpos, double ypos) {
        numLeft++;
        leftSideClicks.emplace_back(xpos, ypos);
        //std::cout << "Saved click at: (" << xpos << ", " << ypos << ")" << std::endl;
    }

    void addRightClick(const glm::vec3& clickWorldPos) {
        if (numRight >= numLeft) {
            std::cout << "Pick 2D points before corresponding 3D point!" << std::endl;
            return;
        }
        numRight++;
        rightSideClicks.push_back(clickWorldPos);
        //std::cout << "Picked point in 3D space: (" << clickWorldPos.x << ", " << clickWorldPos.y << ", " << clickWorldPos.z << ")" << std::endl;
    }
};

struct savedCapturesWithEstimate {
    std::vector<struct CameraState> captures;
    std::vector<struct CameraState> estimates;
};

extern ClickData myClickData;

extern savedCapturesWithEstimate mySavedCaptures;



// Color definitions
extern glm::vec3 colors[21];

enum class Mode {
    NONE,
    PICKING,
    LOAD_STATE,
    CHOOSE
};

extern Mode currentMode;

extern bool captureflag;

extern bool updated;


extern std::vector<CameraState> saveEstimatedPnp;

extern std::vector<glm::vec3> pointsVec;


#endif // GLOBALS_H
