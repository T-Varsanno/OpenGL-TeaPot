#include "globals.h"

// Window dimensions
const int WIDTH = 1152;
const int HEIGHT = 648;

// Camera transformation
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Camera rotation
float yaw = -90.0f; // Initialize yaw to -90.0 degrees to align with the initial cameraFront
float pitch = 0.0f;

// Saved camera state
glm::vec3 savedCameraPos;
glm::vec3 savedCameraFront;
float savedYaw;
float savedPitch;


// Object transformation
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
float objectRotationX = 0.0f;
float objectRotationY = 0.0f;

// Image storage
std::vector<unsigned char> image;

// Saved camera states
std::vector<CameraState> savedCameraStates;
int currentCameraStateIndex = 0;
bool loadState = false;
bool pickingState = false;
bool chooseState = false;

std::vector<glm::vec3> cameraPath;

CameraState lastState;
CameraState lastPickingState;
CameraState cameraPnpSol;

KeyState keyState;



ClickData myClickData;

savedCapturesWithEstimate mySavedCaptures;

glm::vec3 colors[21] = {
    {1.0f, 0.0f, 0.0f},      // 0 Red
    {0.0f, 1.0f, 0.0f},      // 1 Green
    {0.0f, 0.0f, 1.0f},      // 2 Blue
    {1.0f, 1.0f, 0.0f},      // 3 Yellow
    {1.0f, 0.0f, 1.0f},      // 4 Magenta
    {0.0f, 1.0f, 1.0f},      // 5 Cyan
    {0.8f, 0.4f, 0.1f},      // 6 Burnt Sienna (replaced Gray)
    {1.0f, 0.5f, 0.0f},      // 7 Orange
    {0.5f, 0.0f, 0.5f},      // 8 Purple
    {0.5f, 0.5f, 0.0f},      // 9 Olive
    {0.0f, 0.5f, 0.5f},      // 10 Teal
    {0.5f, 0.0f, 0.0f},      // 11 Maroon
    {0.0f, 0.5f, 0.0f},      // 12 Dark Green
    {0.0f, 0.0f, 0.5f},      // 13 Navy
    {1.0f, 0.75f, 0.0f},     // 14 Gold
    {0.75f, 0.0f, 1.0f},     // 15 Violet
    {1.0f, 0.6f, 0.8f},      // 16 Pink
    {0.6f, 1.0f, 0.8f},      // 17 Light Green
    {0.0f, 0.1f, 0.6f},      // 18 Sapphire (replaced Light Purple)
    {0.3f, 0.1f, 0.1f},      // 19 Dark Sienna (replaced Dark Gray)
    {0.0f, 0.8f, 0.8f}       // 20 Bright Turquoise
};


glm::vec3 topOfLid(0.0f, 0.8f, 0.0f);                   // 0
glm::vec3 topOfHandle(-1.45f, 0.23f, 0.0f);             // 1

glm::vec3 leftCenter(0.0f, 0.0f, -0.93f);               // 2
glm::vec3 rightCenter(0.0f, 0.0f, 0.93f);               // 3

glm::vec3 rightLeftUpper(-0.372f, 0.2737f, 0.73f);      // 4
glm::vec3 rightRightUpper(0.372f, 0.2737f, 0.73f);      // 5
glm::vec3 leftLeftUpper(-0.372f, 0.2737f, -0.73f);      // 6
glm::vec3 leftRightUpper(0.372f, 0.2737f, -0.73f);      // 7

glm::vec3 rightLeftLower(-0.52f, -0.34f, 0.85f);        // 8
glm::vec3 rightRightLower(0.52f, -0.34f, 0.85f);        // 9
glm::vec3 leftLeftLower(-0.52f, -0.34f, -0.85f);        // 10
glm::vec3 leftRightLower(0.52f, -0.34f, -0.85f);        // 11

glm::vec3 handleRightUpper(-0.795f, 0.29f, 0.112f);     // 12
glm::vec3 handleRightLower(-0.976f, -0.416f, 0.113f);   // 13
glm::vec3 handleLeftUpper(-0.795f, 0.29f, -0.112f);     // 14
glm::vec3 handleLeftLower(-0.976f, -0.416f, -0.113f);   // 15
glm::vec3 handleMiddle(-1.335f, -0.239f, 0.0f);         // 16

glm::vec3 tipOfSpout(1.715f, 0.45f, 0.0f);              // 17
glm::vec3 spoutBaseUpper(0.949f, -0.062f, 0.0f);        // 18
glm::vec3 spoutBaseLower(0.975f, -0.45f, 0.0f);         // 19   
glm::vec3 spoutBaseMiddle(1.41f, 0.158f, 0.0f);         // 20

std::vector<glm::vec3> pointsVec = {
    topOfLid,topOfHandle,leftCenter,rightCenter,rightLeftUpper,rightRightUpper,
    leftLeftUpper,leftRightUpper,rightLeftLower,rightRightLower,leftLeftLower,
    leftRightLower,handleRightUpper,handleRightLower,handleLeftUpper,handleLeftLower,
    handleMiddle,tipOfSpout,spoutBaseUpper,spoutBaseLower,spoutBaseMiddle
};

Mode currentMode = Mode::NONE;

bool captureflag = 0;
bool updated = 0;
std::vector<CameraState> saveEstimatedPnp;