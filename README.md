# Rendering Engine Based on OpenGL
## 已实现功能
* 资源载入：包含着色器、纹理、模型、材质资源，保存在resources文件夹下
* 场景载入：包含场景名称、相机参数、场景内的Game Objects的名称、使用的模型与材质，及其几何参数，支持实例化渲染。
* 场景渲染：根据设置的Active Scene，渲染场景到画面中。
* 后处理效果：参考Unity实现后处理Volume，包含辉光效果和暗角效果。
* UI设置参数：使用ImGUI，对当前画面的背景颜色、相机参数、后处理效果参数进行实时调整。

## TODO
* [ ] 更多的后处理效果
  - [ ] 景深(Depth of Field)
  - [ ] ......
* [ ] 背景颜色及天空盒
* [ ] 更加复杂的场景，包含灯光、PBR材质、透明物体等
* [ ] ......(**灵感快来!!!**)

## 实现效果
![TinyEngine](https://github.com/yinb1426/OpenGL-Tiny-Engine/blob/main/resources/textures/TinyEngine.png)

## 使用说明
......
