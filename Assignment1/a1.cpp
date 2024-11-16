// Assignment on building a 2D House
// By Naimur Rahman
// Roll No: 1907031

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 600;
float rotateAngle = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Assignment 1 by 1907031", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //House Points
    float vertices[] = {

        //1 roof
         0.4f, 0.62467f, 0.0f,
         0.2f,  0.167f, 0.0f,
        -0.7f, 0.167f, 0.0f,
        -0.45f, 0.75f, 0.0f,
         0.45f, 0.75f, 0.0f,
         0.7f, 0.167f, 0.0f,
         0.6f,  0.167f, 0.0f,

        //2 left wall
        -0.6f, 0.167f, 0.0f,
        -0.6f, -0.667f, 0.0f,
         0.14f, -0.667f, 0.0f,
         0.14f, 0.167f, 0.0f,

        //3 right wall
         0.14f, -0.667f, 0.0f,
         0.60f, -0.667f, 0.0f,
         0.60f, 0.167f, 0.0f,
         0.4f, 0.62467f, 0.0f,
         0.2f,  0.167f, 0.0f,
         0.14f, 0.167f, 0.0f,

        //4 window left frame
        -0.46f, 0.0f, 0.0f,
        -0.34f, 0.0f, 0.0f,
        -0.34f, -0.2f, 0.0f,
        -0.46f, -0.2f, 0.0f,

        //5 window left
        -0.45f, -0.0167f, 0.0f,
        -0.35f, -0.0167f, 0.0f,
        -0.35f, -0.1833f, 0.0f,
        -0.45f, -0.1833f, 0.0f,

        //6 door frame
        -0.2f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, -0.667f, 0.0f,
        -0.2f, -0.667f, 0.0f,

        //7 door
        -0.18f, -0.033f, 0.0f,
        -0.02f, -0.033f, 0.0f,
        -0.02f, -0.667f, 0.0f,
        -0.18f, -0.667f, 0.0f,

        //8 window left frame
         0.34f, 0.0f, 0.0f,
         0.46f, 0.0f, 0.0f,
         0.46f, -0.2f, 0.0f,
         0.34f, -0.2f, 0.0f,

        //9 window right
         0.35f, -0.0167f, 0.0f,
         0.45f, -0.0167f, 0.0f,
         0.45f, -0.1833f, 0.0f,
         0.35f, -0.1833f, 0.0f,

        //10 chimni left long
         0.0f, 0.5f, 0.0f,
         0.05f, 0.5f, 0.0f,
         0.05f, 0.833f, 0.0f,
         0.0f, 0.833f, 0.0f,

        //11 chimni right long
         0.05f, 0.5f, 0.0f,
         0.10f, 0.667f, 0.0f,
         0.10f, 0.833f, 0.0f,
         0.05f, 0.833f, 0.0f,

        //12 chimni left top
        -0.02f, 0.833f, 0.0f,
         0.05f, 0.833f, 0.0f,
         0.05f, 0.867f, 0.0f,
        -0.02f, 0.867f, 0.0f,

        //13 chimni right top
         0.05f, 0.833f, 0.0f,
         0.12f, 0.833f, 0.0f,
         0.12f, 0.867f, 0.0f,
         0.05f, 0.867f, 0.0f,


    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations
        /*glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(translate_X, translate_Y, 0.0f));
        trans = glm::rotate(trans, glm:: radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans,glm::vec3(scale_X, scale_Y, 1.0));*/
        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        translationMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        //modelMatrix = rotationMatrix * scaleMatrix;

        // get matrix's uniform location and set matrix
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        
        // Draw each object with a different color
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
        
        //glDrawArrays(GL_LINES, 0, 6);
        //glDrawArrays(GL_LINE_STRIP, 0, 6);
        //glDrawArrays(GL_LINE_LOOP, 0, 6);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glUniform4f(colorLoc, 1.0f, 0.620f, 0.592f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
        glUniform4f(colorLoc, 0.867f, 0.867f, 0.867f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 7, 4);
        glUniform4f(colorLoc, 0.996f, 0.996f, 0.996f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 11, 6);
        glUniform4f(colorLoc, 0.639f, 0.345f, 0.125f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 17, 4);
        glUniform4f(colorLoc, 0.929f, 0.757f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 21, 4);
        glUniform4f(colorLoc, 0.639f, 0.345f, 0.125f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 25, 4);
        glUniform4f(colorLoc, 0.737f, 0.424f, 0.145f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 29, 4);
        glUniform4f(colorLoc, 0.639f, 0.345f, 0.125f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 33, 4);
        glUniform4f(colorLoc, 0.929f, 0.757f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 37, 4);
        glUniform4f(colorLoc, 0.867f, 0.867f, 0.874f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 41, 4);
        glUniform4f(colorLoc, 0.992f, 0.996f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 45, 4);
        glUniform4f(colorLoc, 0.624f, 0.353f, 0.110f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 49, 4);
        glUniform4f(colorLoc, 0.757f, 0.416f, 0.133f, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 53, 4);
        
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //translation
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        translate_X += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        translate_X -= 0.001f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        translate_Y += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        translate_Y -= 0.001f;

    // Scale up when "Q" key is pressed
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        scale_X += 0.001f;
        scale_Y += 0.001f;
    }

    // Scale down when "E" key is pressed
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        scale_X -= 0.001f;
        scale_Y -= 0.001f;

        if (scale_X < 0.1f) scale_X = 0.1f;
        if (scale_Y < 0.1f) scale_Y = 0.1f;
    }

    // Rotate clockwise when "D" key is pressed
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        rotateAngle -= 0.5f;

    // Rotate counterclockwise when "A" key is pressed
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        rotateAngle += 0.5f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
