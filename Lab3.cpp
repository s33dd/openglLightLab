#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>

#define MAX_POS 20
#define MIN_POS -20
#define MIN_HEIGHT 3

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glLoadIdentity();
    float widtHeightRatio = (float)width / (float)height;
    glFrustum(-widtHeightRatio, widtHeightRatio, -1, 1, 1, 100);
}

void processExit(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void DrawFloor() {
    float vertices[] = {
    -1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
    };

    glVertexPointer(3, GL_FLOAT, 0, &vertices);
    glNormal3f(0, 0, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    for (int i = MIN_POS / 2; i < MAX_POS / 2; i++) {
        for (int j = MIN_POS / 2; j < MAX_POS / 2; j++) {
            glPushMatrix();
                if ((i+j) % 2 == 0) {
                    glColor3f(1.0f, 0.5f, 0.0f);
                }
                else {
                    glColor3f(1.0f, 0.0f, 1.0f);
                }
                glTranslatef(i * 2, j * 2, -0.5);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glPopMatrix();
        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawAxises() {
    float xAxis[] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f
    };
    float yAxis[] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    };
    float zAxis[] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(10);
    glVertexPointer(3, GL_FLOAT, 0, &xAxis);
    for (int i = 0; i < MAX_POS; i++) {
        glPushMatrix();
            //glTranslatef(MIN_POS, MAX_POS, 0);
            glTranslatef(i, 0, 0);
            glDrawArrays(GL_LINES, 0, 2);
        glPopMatrix();
    }
    glVertexPointer(3, GL_FLOAT, 0, &yAxis);
    for (int i = 0; i < MAX_POS; i++) {
        glPushMatrix();
           //glTranslatef(MIN_POS, MIN_POS, 0);
            glTranslatef(0, i, 0);
            glDrawArrays(GL_LINES, 0, 2);
        glPopMatrix();
    }
    glVertexPointer(3, GL_FLOAT, 0, &zAxis);
    for (int i = 0; i < MAX_POS; i++) {
        glPushMatrix();
            //glTranslatef(MIN_POS, MAX_POS, 1);
            glTranslatef(0, 0, i);
            glDrawArrays(GL_LINES, 0, 2);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

std::vector<int> MoveCamera(int &rho, int &theta, int &phi) {
    //Min and max values;
    int phiMax = 90;
    int phiMin = 0;
    int rhoMin = 0;
    int thetaMax = 90;
    int thetaMin = 0;

    if (rho < rhoMin) {
        rho = rhoMin;
    }
    if (theta > thetaMax) {
        theta = thetaMax;
    }
    if (theta < thetaMin) {
        theta = thetaMin;
    }
    if (phi > phiMax) {
        phi = phiMax;
    }
    if (phi < phiMin) {
        phi = phiMin;
    }
    //Translate degrees into radians
    double thetaRad = theta * M_PI / 180;
    double phiRad = phi * M_PI / 180;
    //Translate spherical into carthesian
    GLfloat xPos = (GLfloat)rho * sin(phiRad) * cos(thetaRad);
    GLfloat yPos = (GLfloat)(rho * sin(thetaRad) * sin(phiRad));
    GLfloat zPos = (GLfloat)(rho * cos(phiRad));
    glRotatef(-phi, 1, 0, 0);
    glRotatef(-(theta + thetaMax), 0, 0, 1);
    glTranslatef(-xPos, -yPos, -zPos);
	std::vector<int> coords;
	coords.push_back(xPos);
	coords.push_back(yPos);
	coords.push_back(zPos);
	return coords;
    //std::cout << "x:" << xPos << " y:" << yPos << " z:" << zPos << std::endl;
}

void DrawLight() {
    glDisable(GL_LIGHTING);
    float vertices[] = {
        0.0f, 0.0f, 0.0f, //0
        0.0f, 1.0f, 0.0f, //1
        1.0f, 1.0f, 0.0f, //2
        1.0f, 0.0f, 0.0f, //3
        0.0f, 0.0f, 1.0f, //4
        0.0f, 1.0f, 1.0f, //5
        1.0f, 1.0f, 1.0f, //6
        1.0f, 0.0f, 1.0f  //7
    };
    GLuint firstEdge[] = {
        0, 1, 2,
        2, 3, 0
    };
    GLuint secondEdge[] = {
        4, 5, 6,
        6, 7, 4
    };
    GLuint thirdEdge[] = {
        7, 6, 2,
        7, 2, 3
    };
    GLuint fourthEdge[] = {
        0, 1, 5,
        5, 4, 0
    };
    GLuint fifthEdge[] = {
        1, 2, 6,
        6, 5, 1
    };
    GLuint sixthEdge[] = {
        0, 3, 7,
        7, 4, 0
    };
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
            glColor3f(255 / 255.0f, 255 / 255.0f, 255 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, firstEdge);

            glColor3f(255 / 255.0f, 255 / 255.0f, 255 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, secondEdge);

            glColor3f(255 / 255.0f, 255 / 255.0f, 255 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, thirdEdge);

            glColor3f(255 / 255.0f, 255 / 255.0f, 0 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, fourthEdge);

            glColor3f(255 / 255.0f, 171 / 255.0f, 255 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, fifthEdge);

            glColor3f(255 / 255.0f, 255 / 255.0f, 255 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, sixthEdge);
        glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
}

std::vector<int> MoveLight(int &xPos, int &yPos, int &zPos) {
    if (xPos < 0) {
        xPos = 0;
    }
    if (xPos > MAX_POS) {
        xPos = MAX_POS;
    }
    if (yPos < 0) {
        yPos = 0;
    }
    if (yPos > MAX_POS) {
        yPos = MAX_POS;
    }
    if (zPos < 0) {
        zPos = 0;
    }
    if (zPos > MAX_POS) {
        zPos = MAX_POS;
    }
    glPushMatrix();
        glTranslatef(xPos, yPos, zPos);
        float lightPosition[] = { 0, 0, 1, 1 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        DrawLight();
    glPopMatrix();
    std::vector<int> coords;
    coords.push_back(xPos);
    coords.push_back(yPos);
    coords.push_back(zPos);
    return coords;
};

void DrawCube() {
    float vertices[] = {
    0.0f, 0.0f, 0.0f, //0
    0.0f, 1.0f, 0.0f, //1
    1.0f, 1.0f, 0.0f, //2
    1.0f, 0.0f, 0.0f, //3
    0.0f, 0.0f, 1.0f, //4
    0.0f, 1.0f, 1.0f, //5
    1.0f, 1.0f, 1.0f, //6
    1.0f, 0.0f, 1.0f  //7
    };
    GLuint firstEdge[] = {
        0, 1, 2,
        2, 3, 0
    };
    GLuint secondEdge[] = {
        4, 5, 6,
        6, 7, 4
    };
    GLuint thirdEdge[] = {
        7, 6, 2,
        7, 2, 3
    };
    GLuint fourthEdge[] = {
        0, 1, 5,
        5, 4, 0
    };
    GLuint fifthEdge[] = {
        1, 2, 6,
        6, 5, 1
    };
    GLuint sixthEdge[] = {
        0, 3, 7,
        7, 4, 0
    };
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
            glTranslatef(5.0f, 5.0f, 0);
            glScalef(5.0f, 5.0f, 5.0f);

            glNormal3f(0, 0, 1);
            glColor3f(2/255.0f, 18/255.0f, 232/255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, firstEdge);

            glColor3f(13 / 255.0f, 231 / 255.0f, 247 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, secondEdge);

            glNormal3f(1, 0, 0);
            glColor3f(255 / 255.0f, 255 / 255.0f, 255 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, thirdEdge);

            glColor3f(229 / 255.0f, 255 / 255.0f, 0 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, fourthEdge);

            glNormal3f(0, 1, 0);
            glColor3f(51 / 255.0f, 171 / 255.0f, 171 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, fifthEdge);

            glColor3f(0 / 255.0f, 255 / 255.0f, 137 / 255.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, sixthEdge);
        glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
}

int main(void) {

    int rho = 28, theta = 36, phi = 64;
    int lightX = 0, lightY = 0, lightZ = 0;
	std::vector<int> coords;

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Amongus", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    
    //imgui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        processExit(window);

        glClearColor(46/255.0f, 202/255.0f, 52/255.0f, 255/255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glPushMatrix();
            coords = MoveCamera(rho, theta, phi);
            DrawFloor();
            DrawAxises();
            DrawCube();
            MoveLight(lightX, lightY, lightZ);
        glPopMatrix();

        /*ImGui::Begin("Menu");
            ImGui::Text("rho");
            ImGui::InputInt("##rho", &rho);
            ImGui::Text("theta");
            ImGui::InputInt("##theta", &theta);
            ImGui::Text("phi");
            ImGui::InputInt("##phi", &phi);
			ImGui::Text("X: %d, Y: %d, Z: %d", coords[0], coords[1], coords[2]);
        ImGui::End();*/

        ImGui::Begin("Light");
            ImGui::Text("xLight");
            ImGui::InputInt("##xLight", &lightX);
            ImGui::Text("yLight");
            ImGui::InputInt("##yLight", &lightY);
            ImGui::Text("zLight");
            ImGui::InputInt("##zLight", &lightZ);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}