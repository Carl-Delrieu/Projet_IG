#include "common/defines.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool controlPolygon = false;
bool curveDisplay = false;
bool surfaceDisplay = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Projet IGAI", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); 
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Keyboard
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Mouse
    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    //light
    Light light0 = Light(0, glm::vec3(5.0f, 0.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    Light light1 = Light(1, glm::vec3(-4.0f, 0.0f, 9.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    Light light2 = Light(2, glm::vec3(-4.0f, 0.0f, -9.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

    std::vector<Light> lights = {light0, light1, light2};

    LightMesh* light_mesh = new LightMesh(lights, &camera);

    Shader* light_shader = new Shader("src/shaders/light/vertex.vs", "src/shaders/light/fragment.fs");


    // Bezier curve
    std::vector<glm::vec3> bezier_points = {
        {glm::vec3(1.0f, 0.0f, 0.0f)},
        {glm::vec3(0.0f, 1.0f, 0.0f)},
        {glm::vec3(0.0f, 0.0f, 1.0f)}
    };

    BezierCurve* bezier_curve = new BezierCurve(bezier_points, 100);
    
    BezierCurveMesh* curve_mesh = new BezierCurveMesh(bezier_curve->getCurve(), bezier_curve->getIndices(), &camera);
    BezierCurveControlPolygonMesh* c_pol_mesh = new BezierCurveControlPolygonMesh(bezier_curve->getControlPolygon(), &camera);
    
    Shader* curve_shader = new Shader("src/shaders/bezier_curve/vertex.vs", "src/shaders/bezier_curve/curve_fragment.fs");
    Shader* c_pol_shader = new Shader("src/shaders/bezier_curve/vertex.vs", "src/shaders/bezier_curve/control_polygon_fragment.fs");

    // Bezier surface
    std::vector<std::vector<glm::vec3>> surface_points = {
        {
            {glm::vec3(-1.0f, -1.0f, -1.0f)},
            {glm::vec3(0.0f, -1.0f, 1.0f)},
            {glm::vec3(1.0f, -1.0f, -1.0f)}
        },
        {
            {glm::vec3(-1.0f, 0.0f, 1.0f)},
            {glm::vec3(0.0f, 0.0f, -1.0f)},
            {glm::vec3(1.0f, 0.0f, 1.0f)}
        },
        {
            {glm::vec3(-1.0f, 1.0f, -1.0f)},
            {glm::vec3(0.0f, 1.0f, 1.0f)},
            {glm::vec3(1.0f, 1.0f, -1.0f)}
        }
    };

    BezierSurface* bezier_surface = new BezierSurface(surface_points, glm::vec3(0.0f, 0.0f, 1.0f));

    BezierSurfaceMesh* surface_mesh = new BezierSurfaceMesh(bezier_surface->getSurface(), bezier_surface->getColor(), lights, &camera);
    BezierSurfaceControlPolygonMesh* s_pol_mesh = new BezierSurfaceControlPolygonMesh(bezier_surface->getControlPolygon(), &camera);
    BezierSurfaceNormalMesh* s_nor_mesh = new BezierSurfaceNormalMesh(surface_mesh->getVertices(), surface_mesh->getNormals(), &camera);

    Shader* surface_shader = new Shader("src/shaders/bezier_surface/vertex.vs", "src/shaders/bezier_surface/surface_fragment.fs");
    Shader* s_pol_shader = new Shader("src/shaders/bezier_surface/vertex.vs", "src/shaders/bezier_surface/control_polygon_fragment.fs");
    Shader* s_nor_shader = new Shader("src/shaders/bezier_surface/vertex.vs", "src/shaders/bezier_surface/normal_fragment.fs");

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        //curve_mesh->draw(curve_shader, (float)SCR_HEIGHT, (float)SCR_WIDTH);
        //c_pol_mesh->draw(c_pol_shader, (float)SCR_HEIGHT, (float)SCR_WIDTH);
        
        surface_mesh->draw(surface_shader, (float)SCR_HEIGHT, (float)SCR_WIDTH);
        //s_pol_mesh->draw(s_pol_shader, (float)SCR_HEIGHT, (float)SCR_WIDTH);
        //s_nor_mesh->draw(s_nor_shader, (float)SCR_HEIGHT, (float)SCR_WIDTH);
        
        light_mesh->draw(light_shader, (float)SCR_HEIGHT, (float)SCR_WIDTH);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bezier_curve->~BezierCurve();

    curve_mesh->~BezierCurveMesh();
    c_pol_mesh->~BezierCurveControlPolygonMesh();
    
    curve_shader->~Shader();
    c_pol_shader->~Shader();

    bezier_surface->~BezierSurface();

    surface_mesh->~BezierSurfaceMesh();
    s_pol_mesh->~BezierSurfaceControlPolygonMesh();

    surface_shader->~Shader();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    // Close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.processKeyboard(DOWN, deltaTime);

    // Camera orientation
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.processKeyboard(LOOK_UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.processKeyboard(LOOK_DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.processKeyboard(LOOK_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.processKeyboard(LOOK_RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}