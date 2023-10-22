#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;

void initWindow(GLFWwindow** window);
void createInstance(VkInstance* instance);
void initVulkan(VkInstance* instance);
void mainLoop(GLFWwindow** window);
void cleanup(GLFWwindow** window, VkInstance* instance);
