#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// 定义类型为字符串常量指针的着色器源码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 顶点着色器
    // ------------------------------------
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);    //创建着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     //将着色器源码附加到着色器上
    glCompileShader(vertexShader);     //编译着色器
    // 检测编译是否成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 片段着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 用着色器程序将顶点和片段着色器链接在一起
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检测链接是否成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 定义将要用来输入的一组顶点数组
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f,  
         0.0f,  0.5f, 0.0f  
    }; 
    // 创建VAO和VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);     //生成数量为1的ID为'VAO'的顶点数组对象
    glGenBuffers(1, &VBO);    //生成数量为1的ID为'VBO'的顶点缓冲对象
    // 绑定VAO
    glBindVertexArray(VAO);
    // 把用来输入的顶点复制到缓冲中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     //把'VBO'绑定到类型为GL_ARRAY_BUFFER的目标上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);   //参数一为目标缓冲的类型，参数二指定传输数据大小，参数三是要发送的数据，参数四指定显卡如何处理数据，此处为静态即数据不会或几乎不会改变
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //告诉OpenGL如何解析顶点数据
    glEnableVertexAttribArray(0);       //以顶点属性位置值作为参数，启用顶点属性(默认禁用)

    // 解绑VBO，因为glVertexAttribPointer已经把‘VBO’注册为定点属性绑定的VBO所以可以安全解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // 解绑VAO，可以通过解绑VAO来确保其他VAO的调用不会影响到这个VAO（这种情况很少发生）。修改其他的VAO需要调用glBindVertexArray所以通常当VAO或者VBO不再需要时也不用解绑
    glBindVertexArray(0); 

    // 去掉下面一行注释可以期用线框绘制模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制物体
        glUseProgram(shaderProgram);    //使用着色器程序
        glBindVertexArray(VAO);     // 当只有一个VAO的时候不用每次都绑定，这里是为了保证代码结构更加系统
        glDrawArrays(GL_TRIANGLES, 0, 3);   //绘制顶点数为3，顶点数组的起始索引为0的三角形
        // glBindVertexArray(0); // 不需要每次都解绑
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 可选，回收资源
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}