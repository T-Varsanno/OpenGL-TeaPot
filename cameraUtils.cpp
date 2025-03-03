#include <GL/glut.h>
#include <iostream>
#include "cameraUtils.h"
#include "globals.h" 
#include "pnp.h"

void drawCameraViewTriangle(const glm::vec3& position, const glm::vec3& direction, int flag) {
    glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp)) * 0.1f;
    glm::vec3 up = glm::normalize(glm::cross(right, direction)) * 0.1f;

    glDisable(GL_LIGHTING);
    if (flag == 1) {
        glColor3f(0.824f, 0.016f, 0.176f); //currentChosen , red
    }
    else if(flag == 0){
        glColor3f(0.373f, 0.62f, 0.627f); // others , blue
    }
    else if (flag == 2 || flag == 3) {
        glColor3f(0.941f, 0.502f, 0.251f); // estimated , orange
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(position.x, position.y, position.z);
    glVertex3f(position.x + direction.x * 0.4f + right.x, position.y + direction.y * 0.4f + right.y, position.z + direction.z * 0.4f + right.z);
    glVertex3f(position.x + direction.x * 0.4f - right.x, position.y + direction.y * 0.4f - right.y, position.z + direction.z * 0.4f - right.z);
    glEnd();

    // Draw black border if flag % 2 == 1
    if (flag % 2 == 1) {
        glColor3f(0.0f, 0.0f, 0.0f); // black color
        glBegin(GL_LINE_LOOP);
        glVertex3f(position.x, position.y, position.z);
        glVertex3f(position.x + direction.x * 0.4f + right.x, position.y + direction.y * 0.4f + right.y, position.z + direction.z * 0.4f + right.z);
        glVertex3f(position.x + direction.x * 0.4f - right.x, position.y + direction.y * 0.4f - right.y, position.z + direction.z * 0.4f - right.z);
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

void drawCameraPath() {
    if (cameraPath.size() < 2) return; // Need at least two points to draw a line

    glPushAttrib(GL_ENABLE_BIT); // Save current enable bit states
    glDisable(GL_LIGHTING); // Disable lighting to ensure the color is applied correctly

    glEnable(GL_BLEND); // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blend function

    glEnable(GL_LINE_SMOOTH); // Enable line smoothing
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Set the line smoothing hint to the highest quality

    glColor4f(0.0f, 0.0f, 0.545f, 0.8f); // Set line color with alpha for blending
    glLineWidth(3.0f); // Set line width

    glBegin(GL_LINE_STRIP);
    for (const auto& pos : cameraPath) {
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();

    glPopAttrib(); // Restore previous enable bit states
}

struct CameraState saveCameraState() {
    CameraState state = { cameraPos, cameraFront, yaw, pitch };

    // Capture the right viewport (camera view)
    int width = WIDTH / 2;
    int height = HEIGHT;
    std::vector<unsigned char> image(static_cast<size_t>(3) * static_cast<size_t>(width) * static_cast<size_t>(height));
    glReadPixels(width, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image.data());
    state.image = std::move(image);


    std::cout << "Camera state saved. Total saved states: " << mySavedCaptures.captures.size() << std::endl;
    std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
    std::cout << "Camera Front: (" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;
    std::cout << "Yaw: " << yaw << std::endl;
    std::cout << "Pitch: " << pitch << std::endl;

    return state;
}


void loadNextCameraState() {
    if (!mySavedCaptures.captures.empty()) {
        // Load the next camera state
        currentCameraStateIndex = (currentCameraStateIndex) % mySavedCaptures.captures.size();
        CameraState state = mySavedCaptures.captures[currentCameraStateIndex];
        cameraPos = state.position;
        cameraFront = state.front;
        yaw = state.yaw;
        pitch = state.pitch;

        // Recalculate the camera front vector based on the loaded yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);

        std::cout << "Camera state " << currentCameraStateIndex + 1 << " loaded." << std::endl;
        std::cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
        std::cout << "Camera Front: (" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;
        std::cout << "Yaw: " << yaw << std::endl;
        std::cout << "Pitch: " << pitch << std::endl;
    }
    else {
        std::cout << "No saved camera states to load." << std::endl;
    }
}


void updateCamera() {
    const float cameraSpeed = 0.05f;
    constexpr float rotationSpeed = glm::radians(0.5f);
    bool positionChanged = false;

    if (keyState.W) {
        cameraPos += cameraSpeed * cameraFront;
        positionChanged = true;
    }
    if (keyState.S) {
        cameraPos -= cameraSpeed * cameraFront;
        positionChanged = true;
    }
    if (keyState.A) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        positionChanged = true;
    }
    if (keyState.D) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        positionChanged = true;
    }
    if (keyState.Left) {
        yaw -= glm::degrees(rotationSpeed);
    }
    if (keyState.Right) {
        yaw += glm::degrees(rotationSpeed);
    }
    if (keyState.Up) {
        pitch += glm::degrees(rotationSpeed);
    }
    if (keyState.Down) {
        pitch -= glm::degrees(rotationSpeed);
    }

    // Constrain the pitch to prevent flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate the new camera front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    if (positionChanged && !pickingState) {
        cameraPath.push_back(cameraPos); // Save the new camera position
    }
}

void loadCameraWithParam(glm::vec3 calculatedPos, float calcYaw, float calcPitch) {
    cameraPos = calculatedPos;
    yaw = calcYaw;
    pitch = calcPitch;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);

    std::cout << "Camera Front: (" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")" << std::endl;
    cameraPnpSol = saveCameraState();
}

// sends the data the pnp via estimateCameraPose function, translates the output into a cameraState struct we can use
struct CameraState calculateEstimate() { 
    struct CameraState estimated;
    //PNP
    std::pair<glm::vec3, glm::vec2> result = estimateCameraPose();
    glm::vec3 first = result.first;
    glm::vec2 second = result.second;
    glm::vec3 front;
    front.x = cos(glm::radians(second.y)) * cos(glm::radians(second.x));
    front.y = sin(glm::radians(second.y));
    front.z = cos(glm::radians(second.y)) * sin(glm::radians(second.x));
    glm::vec3 cameraFront1 = glm::normalize(front);
    estimated.position = first;
    estimated.front = cameraFront1;
    estimated.pitch = second.y;
    estimated.yaw = second.x;
    return estimated;
}