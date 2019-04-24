#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//设定窗口的大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    //初始化GLFW
    // ------------------------------
    glfwInit();
    //OpenGL的主版本号和次版本号，此处都为3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //使用core模式的OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//mac平台需要添加的规则
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //
#endif

    //创建窗口，前两个参数是宽和高，第三个参数是窗口标题
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //通知GLFW将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //向GLFW注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //初始化GLAD
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    //渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))  //glfwWindowShouldClose函数检查GLFW是否被要求退出，是的话就返回true，循环结束
    {
        //调用输入控制函数
        // -----
        processInput(window);

        //清屏，确保新的渲染迭代开始时看不到上一次迭代的渲染结果
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   //设置清屏所用颜色
        glClear(GL_COLOR_BUFFER_BIT);   //清空屏幕颜色缓冲

        //
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);    //交换前缓冲和后缓冲，开启双缓冲，避免闪屏
        glfwPollEvents();   //检查有没有触发事件（输入，鼠标移动等），更新窗口状态并调用对应的回调函数
    }

    //释放资源
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

//输入控制函数，当ESC键被按下时通过glfwSetWindowShouldClose把属性设置为true用来关闭GLFW
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//回调函数，当窗口大小被改调整时调用
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //glviewport函数用来设置窗口的dimension为输入值
    glViewport(0, 0, width, height);
}