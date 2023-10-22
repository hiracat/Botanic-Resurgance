#include "main.h"
#include "TextColors.h"
#include <cstdint>
#include <cstring>
#include <exception>
#include <vector>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

int main() {
    App app;
    try {
        std::cout << ANSI_BLINK << "hello world" << ANSI_RESET << std::endl;
        initWindow(&app.window);
        initVulkan(&app);
        mainLoop(&app.window);
        cleanup(&app);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void initWindow(GLFWwindow** window) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Botanic Resurgance", nullptr, nullptr);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << ANSI_COLOR_CYAN << "validation layer: " << pCallbackData->pMessage << ANSI_RESET << std::endl;
    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void setupDebugMessenger(App* app) {
    if (!App::enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(app->instance, &createInfo, nullptr, &app->debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void initVulkan(App* app) {
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    createInstance(&app->instance, validationLayers);
    setupDebugMessenger(app);
}

void createInstance(VkInstance* instance, const std::vector<const char*>& validationLayers) {
    if (App::enableValidationLayers && !checkValidationLayerSupport(validationLayers)) {
        throw std::runtime_error("validation layers requrested but not available");
    }
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Botanic-Resurgance";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_MAKE_VERSION(1, 3, 264);
    appInfo.pNext = VK_NULL_HANDLE;

    auto extentions = getRequiredExtentions();

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extentions.size();
    createInfo.ppEnabledExtensionNames = extentions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    if (App::enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

    } else {

        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
    if (vkCreateInstance(&createInfo, nullptr, instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
    uint32_t layerCount{0};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName)) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> getRequiredExtentions() {
    uint32_t glfwExtentionCount{0};
    const char** glfwExtentions;
    glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtentionCount);

    std::vector<const char*> extentions(glfwExtentions, glfwExtentions + glfwExtentionCount);

    if (App::enableValidationLayers) {
        extentions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extentions;
}

void mainLoop(GLFWwindow** window) {
    while (!glfwWindowShouldClose(*window)) {
        glfwPollEvents();
    }
}

void cleanup(App* app) {
    if (App::enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(app->instance, app->debugMessenger, nullptr);
    }
    vkDestroyInstance(app->instance, nullptr);

    glfwDestroyWindow(app->window);
    glfwTerminate();
}
