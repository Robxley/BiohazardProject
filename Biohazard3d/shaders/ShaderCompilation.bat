@echo off
set GLSL_COMPILER=%VULKAN_SDK%\Bin\glslangValidator;

%GLSL_COMPILER% -V default.vert
%GLSL_COMPILER% -V default.frag
pause