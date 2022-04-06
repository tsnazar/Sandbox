# Sandbox
An OpenGL application with a couple of basic rendering demos.

## Instalation 

## Main Menu
On application launch you should see an ImGui widget with several buttons. Each button corresponds to some demo, after clicking on it you will be able to see the demo.

![photo_2022-04-06_12-50-43](https://user-images.githubusercontent.com/70116589/161948612-75d3a554-138f-485e-8646-c85840218a91.jpg)

If you want to go back to the main menu from the demo press arrow button in ImGui widget.

![image](https://user-images.githubusercontent.com/70116589/161950234-b7b14a53-e9d8-4e18-b818-a942f57b828e.png)

## Demos 

### TestShadowMap
This demo implements ShadomMap using texture as depth buffer.

![image](https://user-images.githubusercontent.com/70116589/161950031-83eb0113-92d5-467b-aa50-169adc4ee5c4.png)

### TestPointShadows
This demo implements ShadowMap for point light using cubemap as depth buffer to achieve 360 degrees shadow casting. Light is located at (0,0,0).

![image](https://user-images.githubusercontent.com/70116589/161950864-e910f324-4c21-431d-bfa5-d758b101444b.png)

### CornellBox
Simple CornellBox scene. Shadows implemented with the same method as shadows in TestShadowMap and here you can choose which light approximation method to use: either Phong
or Blinn-Phong. Additionaly gamma correction is enabled.

![image](https://user-images.githubusercontent.com/70116589/161952079-3850b732-63bc-44f3-8dc3-7ee6397187c9.png)

![image](https://user-images.githubusercontent.com/70116589/161952111-9444c958-8bc6-4686-ae96-8da597228b96.png)

### OrthoTest
Just a test for 2D orthographic camera and events handling(cube changes color on mouse button click).

![image](https://user-images.githubusercontent.com/70116589/161952360-32cdad9b-df32-4be3-b04e-1d18056c69e7.png)

### TestLightA
Diffuse and specular map. Phong model.

![image](https://user-images.githubusercontent.com/70116589/161953279-de109c95-4c56-41c0-a601-def776c85112.png)

### TestLightB
Same TestLightA with multiple cubes and light attenuation.

![image](https://user-images.githubusercontent.com/70116589/161953545-1f7c0483-166e-4c10-9417-2cfe8252d381.png)

### TestLightC
Same TestLightB but camera is spotlight source. 

![image](https://user-images.githubusercontent.com/70116589/161953657-d8248e72-1bd9-4e54-8ed6-2159fbc44c32.png)

### TestLightD
Multiple objects, multiple point light sources.

![image](https://user-images.githubusercontent.com/70116589/161953777-0c687ff2-901a-458a-889a-ebcaa97c7ba6.png)
