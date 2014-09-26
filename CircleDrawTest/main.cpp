#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <OpenGL/gl3.h>

#define GLSL(src) "#version 150 core\n" #src

struct Circle
{
  float radius;
  float coord[2];
  float rgba[3];
};

void createCircles(std::vector<Circle> &circles)
{
  circles.push_back(
  {
    0.5f,
    0.5f, 0.5f,
    1.0f, 0.0f, 0.0f
  } );

  circles.push_back(
  {
    0.25f,
    0.35f, 0.35f,
    0.0f, 0.0f, 1.0f
  } );
}

void drawCircles(GLuint shaderProgram, std::vector<Circle> &circles)
{
  glUseProgram(shaderProgram);

  GLint radius = glGetUniformLocation(shaderProgram, "radius");
  GLint circlePos = glGetUniformLocation(shaderProgram, "circlePos");
  GLint circleColor = glGetUniformLocation(shaderProgram, "circleColor");
  
  for(std::vector<Circle>::iterator circle = circles.begin();
      circle != circles.end();
      ++circle)
  {
    glUniform1f(radius, circle->radius);
    glUniform2f(circlePos, circle->coord[0], circle->coord[1]);
    glUniform3f(circleColor, circle->rgba[0], circle->rgba[1], circle->rgba[2]);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
  
  glUseProgram(0);
}

int main(int argc, const char * argv[])
{
  if(glfwInit() != GL_TRUE)
  {
    std::cout << "Could not initalise GLFW" << std::endl;
    return EXIT_FAILURE;
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  GLFWwindow *window;
  window = glfwCreateWindow(800, 800, "Draw Circle Test", NULL, NULL);
  if(!window)
  {
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  
  // Create circle data.
  std::vector<Circle> circles;
  createCircles(circles);
  
  // Create vertex array
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Vertex Buffer
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  float vertices[] = {
    -1.0f,  1.0f,     // top left
     1.0f,  1.0f,     // top right
     1.0f, -1.0f,     // bottom right
    -1.0f, -1.0f,     // bottom left
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Element buffer
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  
  GLuint elements [] = {
    0, 1, 2,
    2, 3, 0
  };
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  
  // Vertex shader
  const char *vertexShaderSrc =\
  GLSL(
       in vec2 position;
                                        
       void main()
       {
         gl_Position = vec4(position, 0.0, 1.0);
       }
  );
  
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);
  
  // Fragment shader
  const char *fragmentShaderSrc =\
  GLSL(
       uniform float radius;
       uniform vec2 circlePos;
       uniform vec3 circleColor;
       
       out vec4 outColor;
       
       void main()
       {
         float rr = radius * radius;
         vec2 resolution = vec2(800.0, 800.0);
         vec2 position = gl_FragCoord.xy / resolution;
         
         float xPos = position.x;
         float yPos = position.y;
         
         float x = (xPos - circlePos.x) * (xPos - circlePos.x);
         float y = (yPos - circlePos.y) * (yPos - circlePos.y);
         
         float step = (smoothstep(0.0, rr, (x + y)) / radius);
         
         if( (x + y) < rr )
         {
           outColor = vec4(circleColor, 0.0f) + (step * vec4(1.0f, 1.0f, 1.0f, 0.0f));
         }
         else
         {
           outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
         }
       }
  );
  
  std::cout << "Vertex Source: " << vertexShaderSrc << std::endl;
  std::cout << "Fragment Source: " << fragmentShaderSrc << std::endl;
  
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShader);

  GLint compileStatus;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
  
  if(compileStatus == GL_FALSE)
  {
    GLint logLength;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    
    char *log = (char*)malloc(logLength + 1);
    glGetShaderInfoLog(fragmentShader, logLength, NULL, log);
    
    std::cout << log << std::endl;
    
    free((void*)log);
    return EXIT_FAILURE;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  
  GLint vertexPositionAttr = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(vertexPositionAttr);
  glVertexAttribPointer(vertexPositionAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);
  
  while(!glfwWindowShouldClose(window))
  {
    // Do drawing
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawCircles(shaderProgram, circles);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glfwTerminate();
  
  return EXIT_SUCCESS;
}
