#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>
#include "renderUtils.h"
#include "globals.h"
#include "drawFunctions.h"
#include "cameraUtils.h"
#include "pnp.h"


void renderScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (currentMode) {
    case Mode::LOAD_STATE: renderLoadingMode();
        break;
    default: renderNormalMode();
    }

    drawDividerLine();
    glutSwapBuffers(); // Use the passed window parameter
}



/*******************************************    NormalMode    ***************************************************/
void renderNormalMode() {
    renderLeftDebugViewport();
    //draws the triangles
    if (!mySavedCaptures.captures.empty()) {
        for (size_t i = 0; i < mySavedCaptures.captures.size(); ++i) {
            int flag = (currentCameraStateIndex == i);
            drawCameraViewTriangle(mySavedCaptures.captures[i].position, mySavedCaptures.captures[i].front, flag);
        }
    }
    renderRightNormalViewport();
}


void renderLeftDebugViewport() {
    // Left view (Global view)
    glViewport(0, 0, WIDTH / 2, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 globalView = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), objectPos, cameraUp);
    glLoadMatrixf(glm::value_ptr(globalView));

    renderPoints();



    // Apply object transformations for normal mode
    glPushMatrix();
    glTranslatef(objectPos.x, objectPos.y, objectPos.z);
    glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
    glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);
    drawTeapot();
    glPopMatrix();

    // Draw the camera path 
    drawCameraPath();
    
    setUpText(WIDTH / 4 - 50, "debugMode");
}

void renderRightNormalViewport() {

    setupRightViewport();
    // Apply object transformations for normal mode
    renderPoints();


    glPushMatrix();
    glTranslatef(objectPos.x, objectPos.y, objectPos.z);
    glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
    glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);
    drawTeapot();
    glPopMatrix();

    setUpText(WIDTH / 4 - 45, "userMode");
}

/*******************************************    LoadingMode    ***************************************************/
void renderLoadingMode() {
    if (!captureflag) {     // if we havent captured all estimated images yet
        captureEstimateImages();
        captureflag = 1;
    }

    renderLeftDebugViewport();

    //draw all the triangles, including estimates
    if (!mySavedCaptures.captures.empty()) {
        for (size_t i = 0; i < mySavedCaptures.captures.size(); ++i) {
            int flag = (currentCameraStateIndex == i);
            drawCameraViewTriangle(mySavedCaptures.captures[i].position, mySavedCaptures.captures[i].front, flag);
            if (mySavedCaptures.estimates.size() > i) {
                drawCameraViewTriangle(mySavedCaptures.estimates[i].position, mySavedCaptures.estimates[i].front,flag+2);
            }
        }
    }

    renderRightLoadingViewport();
}


void renderRightLoadingViewport() {

    std::vector<unsigned char>& image2 = mySavedCaptures.estimates[currentCameraStateIndex].image;
    std::vector<unsigned char>& image1 = mySavedCaptures.captures[currentCameraStateIndex].image; // Assuming secondImage is defined elsewhere

    // Set viewport to the right half of the window
    glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    


    // Save the current projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH / 2, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Set raster position to the lower-left corner of the viewport
    glRasterPos2i(0, 0);

    // Draw the first image
    glDrawPixels(WIDTH / 2, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image1.data());

    // Disable depth test and enable blending
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw the second image with an orange tint and transparency
    // Create an RGBA image from the RGB image to include the alpha channel
    std::vector<unsigned char> image2_rgba(WIDTH / 2 * HEIGHT * 4); // 4 channels: R, G, B, A
    for (size_t i = 0; i < image2.size(); i += 3) {
        image2_rgba[i * 4 / 3] = std::min(static_cast<int>(image2[i] * 1.5), 255); // Increase red channel
        image2_rgba[i * 4 / 3 + 1] = std::min(static_cast<int>(image2[i + 1] * 1.2), 255); // Increase green channel
        image2_rgba[i * 4 / 3 + 2] = image2[i + 2]; // Blue channel remains unchanged
        image2_rgba[i * 4 / 3 + 3] = 128; // Set transparency to 50%
    }

    // Set raster position again for the second image
    glRasterPos2i(0, 0);

    glDrawPixels(WIDTH / 2, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, image2_rgba.data());

    // Restore the projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable depth test if it was previously enabled
    glEnable(GL_DEPTH_TEST);
    setUpText(WIDTH / 4 - 45, "userMode");
}


/*******************************************    HelperFunctions    ***************************************************/

//function to draw the divider line between viewports
void drawDividerLine() {
    // Draw a vertical line in the middle
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // Set line color to white
    glBegin(GL_LINES);
    glVertex2f(WIDTH / 2, 0);
    glVertex2f(WIDTH / 2, HEIGHT);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//right viewport default setup
void setupRightViewport() {
    // Right view (Camera view)
    glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glLoadMatrixf(glm::value_ptr(cameraView));
}

//sphere drawing function for point insertion
void drawSpheres(const glm::vec3& threeDPoint, glm::vec3& color) {
    // Define the properties of the sphere
    float sphereRadius = 0.01f; // Adjust radius as needed
    int slices = 16; // Number of slices (segments) for the sphere
    int stacks = 16; // Number of stacks (segments) for the sphere

    glDisable(GL_LIGHTING);
    // Save current matrix state
    glPushMatrix();

    // Translate to the sphere's center position relative to the teapot
    glTranslatef(threeDPoint.x, threeDPoint.y, threeDPoint.z);

    // Draw the sphere using OpenGL commands
    glColor3f(color.x, color.y, color.z); // Set sphere color
    glutSolidSphere(sphereRadius, slices, stacks);

    // Restore previous matrix state
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

//renders the pre-determained points of interest
void renderPoints() {
    for (int i = 0; i < pointsVec.size(); i++) {
        drawSpheres(pointsVec[i], colors[i]);
    }
}

//captures the images for all the estimates (Moves to each location and takes a picture - done before rendering viewpoints)
void captureEstimateImages() {
    
    // captures the image for each saved estimate
    for (int i = 0; i < mySavedCaptures.estimates.size();i++) {

        //setup viewpoint
        glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
        // Clear the right viewport
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        struct CameraState temp = mySavedCaptures.estimates[i];
        glm::mat4 cameraView = glm::lookAt(temp.position, temp.position + temp.front, cameraUp);
        glLoadMatrixf(glm::value_ptr(cameraView));
        glPushMatrix();

        // Apply object transformations for normal mode (same as in renderRightNormalViewport)
        glTranslatef(objectPos.x, objectPos.y, objectPos.z);
        glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
        glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);

        // Draw teapot (if needed for picking)
        drawTeapot();

        // Render picked spheres on the teapot's surface
        glDisable(GL_LIGHTING); // Disable lighting for sphere color consistency
        glEnable(GL_LIGHTING); // Re-enable lighting after rendering spheres

        // Restore previous matrix state
        glPopMatrix();
        int width = WIDTH / 2;
        int height = HEIGHT;
        std::vector<unsigned char> image(static_cast<size_t>(3) * static_cast<size_t>(width) * static_cast<size_t>(height));
        glReadPixels(width, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image.data());
        mySavedCaptures.estimates[i].image = image;
    }

}

//text rendering helper function 
void renderText(float x, float y, const char* text, void* font) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(font, *text);
        text++;
    }
}

//displays and sets up text for each viewport (default_font : helvetica , location : 20 in y axis)
void setUpText(int x, const char* text) {
    // set up needed to position text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH / 2, HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.6f); // Set text color to white with 60% transparency

    
    renderText(x, 20, text, GLUT_BITMAP_HELVETICA_18); // Center text horizontally

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}