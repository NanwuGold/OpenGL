#ifndef PROCESSINPUT_H_
#define PROCESSINPUT_H_

#include <GLFW/glfw3.h>

namespace FXAA
{
    class processInput
    {
    public:
        void operator()(GLFWwindow *window)
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }
        }
    };

    namespace
    {
        processInput processor;
    }
}

#endif
