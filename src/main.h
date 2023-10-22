#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;
struct App {
    GLFWwindow* window{};
    VkInstance instance{};
    VkDebugUtilsMessengerEXT debugMessenger{};

#ifdef NDEBUG
    static constexpr bool enableValidationLayers = false;
#else
    static constexpr bool enableValidationLayers = true;
#endif
};

void initWindow(GLFWwindow** window);
void createInstance(VkInstance* instance, const std::vector<const char*>& validationLayers);
void initVulkan(App* app);
void mainLoop(GLFWwindow** window);
void cleanup(App* app);
bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
void setupDebugMessenger(App* app);
std::vector<const char*> getRequiredExtentions();
