# Phantom
A 3D Renderer

## Requirements
- Qt 6.5.0 msvc2019_64
- Qt VS Tool

## Usage
1. Build the VS project
2. Open `./x64/Debug/Phantom.exe`
3. Change UI language here:
![image](https://user-images.githubusercontent.com/77724175/236731688-daa780e9-8baf-4b66-b65e-1394c3ad4358.png)

4. Open a .obj file:
![image](https://user-images.githubusercontent.com/77724175/236731767-fcd110b2-8e0e-487c-9316-1b1b0627636e.png)
After opening the file, the 3D model will show in the "3D Objects" list.
![image](https://user-images.githubusercontent.com/77724175/236731822-35247fac-82b3-4bbb-bd70-ace9336e428b.png)

5. When the cursor takes the form of a hand, use the left button to drag and change the view direction, and the right button to drag and move the camera. Employ the mouse wheel for zooming.
6. Switch to FPS camera mode:
![image](https://user-images.githubusercontent.com/77724175/236733187-f32b2c64-f9d2-48ef-b7ac-a907b3ab5286.png)
Click the scene to bring it into focus and press <kbd>Ctrl</kbd> to capture the cursor (press <kbd>Ctrl</kbd> again to release the cursor). With the cursor captured, the camera behaves like a first-person shooter (FPS) camera, allowing you to move the camera using the <kbd>W</kbd><kbd>S</kbd><kbd>A</kbd><kbd>D</kbd> or arrow keys, hold <kbd>Shift</kbd> to accelerate, and use the mouse wheel to zoom in and out.

7. Let there be light:
![image](https://user-images.githubusercontent.com/77724175/236735947-7f453004-ca14-4751-bff4-778ae6ff3cff.png)
Configure parallel light:
![image](https://user-images.githubusercontent.com/77724175/236736041-6d83e7bf-5d3f-492a-9f06-8c8fbd2eca67.png)
Hold a point light source:
![image](https://user-images.githubusercontent.com/77724175/236736175-bc070030-cb05-43cb-a9f1-c02253242ed6.png)
Move the camera to place the light source:
![image](https://user-images.githubusercontent.com/77724175/236736271-e55bf3f1-7a10-4fea-a60e-a41bc213fc09.png)
Configure the point light source:
![image](https://user-images.githubusercontent.com/77724175/236736326-6c2f4afb-57c4-45c1-b9d6-a358182a4576.png)
Unhold the light source, and move to another position to observe it.
![image](https://user-images.githubusercontent.com/77724175/236736418-3c55a79a-8e09-4671-b085-a8c3566ffb80.png)
Do the same thing to other light sources:
![image](https://user-images.githubusercontent.com/77724175/236736671-7e6e88eb-dc3f-4407-800e-f98d721a8b27.png)

8. Configure the 3D object:
![image](https://user-images.githubusercontent.com/77724175/236736731-e984621b-c289-47c1-8f32-74820a5bdede.png)
Metallic, roughness, and distinction coefficient are used for PBR.
![image](https://user-images.githubusercontent.com/77724175/236736858-f86ce9c5-a509-4677-83e5-0dc05b4999a6.png)

## Problems
- Only support triangle meshes
- No surface texture support
- 3D models are unmovable
