#pragma once
#ifndef KEY_CALLBACK_H
#define KEY_CALLBACK_H

#include <GLFW/glfw3.h>



void updateKeyState(unsigned char key, int x, int y);
void updateKeyUp(unsigned char key, int x, int y);
void updateSpecialKeyState(int key, int x, int y);
void updateSpecialKeyUp(int key, int x, int y);
void keyCallback(unsigned char key, int x, int y);
//void handleChoosestateMode(unsigned char key);
void clearPointsPicked();
void SaveEstimatedCapture();

#endif // KEY_CALLBACK_H