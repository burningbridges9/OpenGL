

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// ���������
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

// ������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// ��������
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: ������������� � ����������������
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw �������� ����
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // ������� GLFW ��������� ���� �����
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: �������� ���� ���������� �� OpenGL-�������
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ���������������� ����������� ��������� OpenGL
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // �������������� ����� ��������� ���������
    // -------------------------
    Shader shader("../9.2.geometry_shader.vs", "../9.2.geometry_shader.fs", "../9.2.geometry_shader.gs");

    // �������� ������
    // -----------
    Model nanosuit("../resources/objects/backpack/backpack.obj");

    // ���� ����������
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // ���������� ����� ������ �� �������� ��� ������� �����
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ��������� �����
        // -----
        processInput(window);

        // ���������
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ���������������� ������ �������������
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();;
        glm::mat4 model = glm::mat4(1.0f);
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        // ���������� � �������������� ������ ���������� ������� � ���� uniform-����������
        shader.setFloat("time", glfwGetTime());

        // ��������� ������
        nanosuit.Draw(shader);

        // glfw: ����� ���������� ��������� � ������� �������. ����� ������� �����\������� (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ����. ��������), ���������� ������ �������
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����������, ��� ������� ������������� ����� �������� ����; �������� ��������,
    // ��� ������ � ������ ����� ����������� ������, ��� ������� �� retina -��������.
    glViewport(0, 0, width, height);
}

// glfw: ������ ���, ����� ������������ ����, ���������� ������ callback-�������
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // �����������, ��� ��� Y-���������� ���� ����� �����

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: ������ ���, ����� �������������� �������� ����, ���������� ������ callback-�������
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

