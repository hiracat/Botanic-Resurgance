#include "main.h"
#include "TextColors.h"

int main() {

    try {
        std::cout << ANSI_BLINK << "hello world" << ANSI_RESET << std::endl;
        ;
        GLFWwindow* window{};
        VkInstance instance{};
        initWindow(&window);
        initVulkan(&instance);
        mainLoop(&window);
        cleanup(&window, &instance);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void initWindow(GLFWwindow** window) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Botanic Resurgance", nullptr, nullptr);
}

void initVulkan(VkInstance* instance) {
    createInstance(instance);
}

void createInstance(VkInstance* instance) {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Botanic-Resurgance";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_MAKE_VERSION(1, 3, 264);
    appInfo.pNext = VK_NULL_HANDLE;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}
void mainLoop(GLFWwindow** window) {
    while (!glfwWindowShouldClose(*window)) {
        glfwPollEvents();
    }
}

void cleanup(GLFWwindow** window, VkInstance* instance) {
    vkDestroyInstance(*instance, nullptr);

    glfwDestroyWindow(*window);

    glfwTerminate();
}
