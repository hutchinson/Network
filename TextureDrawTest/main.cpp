#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <OpenGL/gl3.h>

#define GLSL(src) "#version 150 core\n" #src

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
  
  GLuint vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader2, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader2);
  
  // Fragment shader
  const char *fragmentShaderSrc =\
  GLSL(
       out vec4 outColor;
       
       void main()
       {
         vec2 resolution = vec2(800.0, 800.0);
         vec2 position = gl_FragCoord.xy;
         
         vec2 coord = position / resolution;

         outColor = vec4(0.0f, coord.y, 0.0f, 1.0f);
       }
  );

  // Fragment shader
  const char *fragmentMergeShaderSrc =\
  GLSL(
       out vec4 outColor;
       
       uniform sampler2D texFramebuffer;
       
       void main()
       {
         vec2 resolution = vec2(800.0, 800.0);
         vec2 position = gl_FragCoord.xy;

         vec2 coord = position / resolution;
         
         if(coord.x < 0.5)
           outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
         else
           outColor = texture(texFramebuffer, coord);
       }
  );
  
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShader);

  GLuint fragmentMergeShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentMergeShader, 1, &fragmentMergeShaderSrc, NULL);
  glCompileShader(fragmentMergeShader);
  
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

  GLuint shaderMergeProgram = glCreateProgram();
  glAttachShader(shaderMergeProgram, vertexShader2);
  glAttachShader(shaderMergeProgram, fragmentMergeShader);
  glBindFragDataLocation(shaderMergeProgram, 0, "outColor");
  glLinkProgram(shaderMergeProgram);
  
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  
  GLint vertexPositionAttr = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(vertexPositionAttr);
  glVertexAttribPointer(vertexPositionAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

  GLint vertexPositionAttr2 = glGetAttribLocation(shaderMergeProgram, "position");
  glEnableVertexAttribArray(vertexPositionAttr2);
  glVertexAttribPointer(vertexPositionAttr2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  
  //////////////////////////////////////////////////////////////////////////////
  // Create frame buffer
  GLuint frameBuffer;
  glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  
  // Create texture to hold color buffer
  GLuint texColorBuffer;
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
  //////////////////////////////////////////////////////////////////////////////
  
  
  while(!glfwWindowShouldClose(window))
  {
    // Do drawing
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw pass 1 to the texture buffer
    glUseProgram(shaderProgram);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(shaderMergeProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    
    glUniform1i(glGetUniformLocation(shaderMergeProgram, "texFramebuffer"), 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
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
