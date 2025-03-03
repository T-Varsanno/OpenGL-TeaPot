#include "keyCallback.h"
#include "globals.h"
#include "cameraUtils.h"
#include <iostream>
#include <GL/freeglut_std.h>
#include "pnp.h"
#include "renderUtils.h"

int getColorIndex(unsigned char r, unsigned char g, unsigned char b);

//void handlePickingMode(unsigned char key) {
//    if (key == 'p' || key == 'P') {
//        if (mySavedCaptures.captures.size() == 0) {
//            std::cout << "Error: No pictures have been taken !" << std::endl;
//            return;
//        }
//        pickingState = !pickingState;
//        if (pickingState) {
//            std::cout << "Entering camera state picking mode." << std::endl;
//            lastState.position = cameraPos;
//            lastState.front = cameraFront;
//            lastState.yaw = yaw;
//            lastState.pitch = pitch;
//        }
//        else {
//            std::cout << "Exiting camera state picking mode." << std::endl;
//            cameraPos = lastState.position;
//            cameraFront = lastState.front;
//            yaw = lastState.yaw;
//            pitch = lastState.pitch;
//
//            // Recalculate the camera front vector based on the loaded yaw and pitch
//            glm::vec3 front;
//            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//            front.y = sin(glm::radians(pitch));
//            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//            cameraFront = glm::normalize(front);
//
//            currentMode = Mode::NONE;
//        }
//    }
//    if (key == 'c' || key == 'C') { // i am in picking mode and pressed C
//        if (myClickData.numLeft == 0) {
//            std::cout << "cannot choose, not points have been picked." << std::endl;
//            return;
//        }
//        else if (myClickData.numLeft != myClickData.numRight) {
//            std::cout << "number of 2d and 3d points do not match! pick the same number of points." << std::endl;
//            return;
//        }
//        else if (myClickData.numLeft < 4) {
//            std::cout << "Number of 2d and 3d points must be more than 4 for position estimation. Pick more points." << std::endl;
//            return;
//        }
//        currentMode = Mode::CHOOSE;
//        handleChoosestateMode(key);
//    }
//}
//
//void handleChoosestateMode(unsigned char key) {
//    if (key == 'c' || key == 'C') {
//        chooseState = !chooseState;
//        if (!chooseState) {
//            captureflag = 0;
//        }
//    }
//}

// handles key presses in the load mode
void handleLoadStateMode(unsigned char key) {
    if (key == 'r' || key == 'R') {
       
        loadState = !loadState;
        if (loadState) {
            std::cout << "Entering camera state load mode." << std::endl;
            lastState.position = cameraPos;
            lastState.front = cameraFront;
            lastState.yaw = yaw;
            lastState.pitch = pitch;
            loadNextCameraState();
        }
        else {
            std::cout << "Exiting camera state load mode." << std::endl;
            cameraPos = lastState.position;
            cameraFront = lastState.front;
            yaw = lastState.yaw;
            pitch = lastState.pitch;

            // Recalculate the camera front vector based on the loaded yaw and pitch
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);
            currentMode = Mode::NONE; // Exit load state mode
            captureflag = 0;
        }

    }

}

//main keypress logic for Non-movement keys
void keyCallback(unsigned char key, int x, int y) {
    // Handle mode selection
    if (currentMode == Mode::NONE) {    

        if (key == 'r' || key == 'R') {     // enter from normal mode to load mode

            if (mySavedCaptures.captures.size() == 0) {     // if we havent taken a picture yet
                std::cout << "Error: No pictures have been taken !" << std::endl;
                return;
            }
            currentMode = Mode::LOAD_STATE;
            std::cout << "Switching to load state mode." << std::endl;
            handleLoadStateMode(key);
            return;
        }
    }
    else if (currentMode == Mode::LOAD_STATE) {
        handleLoadStateMode(key);
        if (!loadState) {
            currentMode = Mode::NONE; // Exit load state mode
        }
        return;
    }
    if (key == 'b' || key == 'B') {
        mySavedCaptures.captures.push_back(saveCameraState());
        SaveEstimatedCapture();
        clearPointsPicked();
    }
}

    
//regular keys on botton press
void updateKeyState(unsigned char key, int x, int y) {
    if (loadState) {// we send here in order to disable movement
        keyCallback(key, x, y);
        return;
    }
    switch (key) {
    case 'b': case 'B': case 'r': case 'R': keyCallback(key, x, y); break;
    case 'w': case 'W': keyState.W = true; break;
    case 'a': case 'A': keyState.A = true; break;
    case 's': case 'S': keyState.S = true; break;
    case 'd': case 'D': keyState.D = true; break;
    }
}

//regular keys on botton let go
void updateKeyUp(unsigned char key, int x, int y) {
    if (loadState) {
        return;
    }
    switch (key) {
    case 'b': case 'B': case 'r': case 'R': break;
    case 'w': case 'W': keyState.W = false; break;
    case 'a': case 'A': keyState.A = false; break;
    case 's': case 'S': keyState.S = false; break;
    case 'd': case 'D': keyState.D = false; break;
    }
}

//special key on botton press
void updateSpecialKeyState(int key, int x, int y) {
    if (loadState) {
        if (key == GLUT_KEY_RIGHT) {
            currentCameraStateIndex++;
            if (currentCameraStateIndex > mySavedCaptures.captures.size() - 1) {
                currentCameraStateIndex = 0;
            }
            updated = 1;
            //loadNextCameraState();
            return; // Early return to prevent further processing of this key event
        }
        else if (key == GLUT_KEY_LEFT) {
            currentCameraStateIndex--;
            if (currentCameraStateIndex < 0) {
                currentCameraStateIndex = (int)mySavedCaptures.captures.size() - 1;
            }
            //loadNextCameraState();
            updated = 1;
            return; // Early return to prevent further processing of this key event
        }
    }
    switch (key) {
    case GLUT_KEY_LEFT: keyState.Left = true; break;
    case GLUT_KEY_RIGHT: keyState.Right = true; break;
    case GLUT_KEY_UP: keyState.Up = true; break;
    case GLUT_KEY_DOWN: keyState.Down = true; break;
    }
}

//special key on botton let go
void updateSpecialKeyUp(int key, int x, int y) {
    if (loadState) {
        return; // Early return to prevent further processing of this key event
    }
    switch (key) {
    case GLUT_KEY_LEFT: keyState.Left = false; break;
    case GLUT_KEY_RIGHT: keyState.Right = false; break;
    case GLUT_KEY_UP: keyState.Up = false; break;
    case GLUT_KEY_DOWN: keyState.Down = false; break;
    }
}

//clears the myClickData struct to reset it for next capture
void clearPointsPicked() {
    myClickData.leftSideClicks.clear();
    myClickData.rightSideClicks.clear();
    myClickData.numLeft = 0;
    myClickData.numRight = 0;

}

//helper function to recieve color indexing 
int getColorIndex(unsigned char r, unsigned char g, unsigned char b) {
    for (int i = 0; i < 21; ++i) {
        // Match the color with some tolerance
        if (std::abs(colors[i].x * 255.0f - r) < 1.0f &&
            std::abs(colors[i].y * 255.0f - g) < 1.0f &&
            std::abs(colors[i].z * 255.0f - b) < 1.0f) {
            return i;
        }
    }
    return -1; // No match
}

// saves the estimated location in mySavedCaptures.estimates based on the automatic color picking
void SaveEstimatedCapture(){

    //setup params
    int width = WIDTH / 2;
    int height = HEIGHT;
    int numColors = static_cast<int>(sizeof(colors) / sizeof(colors[0]));
    std::vector<std::vector<std::pair<int, int>>> colorPositions(numColors);

    int i = mySavedCaptures.captures.size() - 1;    //the index of the last capture taken, (the one we just pressed b to take)
    std::vector<unsigned char> image = mySavedCaptures.captures[i].image;   //the image of the last capture

    // Iterate over each pixel in the image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            unsigned char r = image[index];
            unsigned char g = image[index + 1];
            unsigned char b = image[index + 2];

            int colorIndex = getColorIndex(r, g, b);
            if (colorIndex != -1) {
                colorPositions[colorIndex].emplace_back(x, y);
            }
        }
    }
    // Calculate and output the average x, y positions for each color
    for (int i = 0; i < numColors; ++i) {
        if (!colorPositions[i].empty()) {
            long long sumX = 0, sumY = 0;
            for (const auto& pos : colorPositions[i]) {
                sumX += pos.first;
                sumY += pos.second;
            }
            float avgX = static_cast<float>(sumX) / colorPositions[i].size();
            float avgY = static_cast<float>(sumY) / colorPositions[i].size();
            //std::cout << "Color " << i << " average position: (" << avgX << ", " << avgY << ")" << std::endl;
            
            myClickData.addLeftClick(avgX, height - avgY);  // the image and y axis in open gl are inverted , one goes up down the other goes down up
            myClickData.addRightClick(pointsVec[i]);
        }
        else {
            //std::cout << "Color " << i << " has no matching pixels." << std::endl;
        }
    }
    if (myClickData.numLeft < 4) {  //not enough points for pnp, removing captured location from mySavedCaptures. returning will clean myClickData
        std::cout << "Less than 4 points in the capture, please take a picture from a different angle/ location!" << std::endl;
        mySavedCaptures.captures.pop_back();
        return;
    }
    // calls the calculateEstimate function after loading the myClickData struct with the current data to be used in the PNP process.
    mySavedCaptures.estimates.push_back( calculateEstimate());

}