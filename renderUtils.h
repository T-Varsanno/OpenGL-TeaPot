#pragma once
#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <GLFW/glfw3.h>

void renderScene();


void renderNormalMode();

void renderLeftDebugViewport();
void renderRightNormalViewport();


void renderLoadingMode();

void renderRightLoadingViewport();





void drawDividerLine();

void setupRightViewport();

void drawSpheres(const glm::vec3& threeDPoint, glm::vec3& color);

void captureEstimateImages();



void renderPoints();

void renderText(float x, float y, const char* text, void* font);

void setUpText(int x, const char* text);


#endif // RENDER_UTILS_H