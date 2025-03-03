# OpenGL-TeaPot

# OpenGL Project: 3D Coordinate Computation from 2D Captures

## Overview
This project is built using **OpenGL** and computes **3D coordinates** of a capture location based on **2D images**. The application features **two viewports**:

- **Left Screen (Debug Mode)** – Provides a **global debugging** view of the scene.
- **Right Screen (User Mode)** – The main user interaction screen, where camera movement and capture functions take place.

## Dependencies
This project relies on the following libraries:
- **GLFW3, GLUT, FreeGLUT** – for window management and input handling
- **GLM** – for mathematical computations
- **OpenCV** – for solving the **Perspective-n-Point (PnP)** problem

## Modes of Operation
The program operates in **two modes**:

- **Flight Mode** (default)
- **Picking Mode**

### Flight Mode
In this mode, the program renders a **3D teapot** and allows the user to:
- **Move** using `W, A, S, D` *(first-person camera movement)*.
- **Rotate** the camera using the **arrow keys**.
- **Capture images** by pressing **`B`**.
- **Automatically estimate** the camera's location based on captured **2D images** and pre-determined **3D sphere locations**.

### Picking Mode
- Activated by pressing **`R`** *(after capturing images)*.
- Allows users to **navigate**, via the arrow keys, through previously captured images and their estimated locations, which are **overlaid** for comparison.
- Pressing **`R`** again **exits** Picking Mode and returns to **Flight Mode**.
