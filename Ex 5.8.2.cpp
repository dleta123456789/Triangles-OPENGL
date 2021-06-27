#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;

/*
*Store the source code for vertix shader and store it in a c string.
*/
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
/*
*store the souce code for fragment shader here as a c string
* could change the color of pixels here as  well
* Fragment color is in the RGBA format
* Red,Green,Blue,Alpha
*/
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";



/*
*	Callback function to resize the GLFW window.
*	Prototype for the function is here:
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//add body  for the function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
/*
* Function for Input Control
* Getkey will take window along witha key as input.
* Function will be named processInput to handle all such code and keep code organzied.
* This function should be near the top of the loop as it should be the first task carried out by the program.
* If we it is below in the order then, the function will run after carrying out all other tasks above it.
*/
void processInput(GLFWwindow* window)
{
	/*
	* we are checking if user has used escape key.
	* if pressed we will close the window.
	*/

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

//main code file
int main()
{
	//Initaite GLFW and configure it
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Window Object and check if its working.
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//
	//Initialize GLAD and check if its working. 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	////////////*******END OF SAFTEY CHECKS****************///////////////

	/*
	Rendering Window size
	X0,Y0,X1,Y1
	First two are the parameters of lower left corner
	and the last two are the width and height of window in pixels
	*/
	glViewport(0, 0, 800, 600);
	//calling the resize function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//----------------------------------- Shader Code here: Vertex,Fragment nad then link shaders
	/*
	*Create a shader object and create the shader with Create Shader
	*/
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	/*
	*After creating shader we need to attach source code with the shader object
	* Parameters:
	* vertex shader object
	* number of strings  and arrays being passed as source code
	* actual source code
	*  its the array of string length which we hve kept null
	*/
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	/*
	*Should add a check for compile time errors.
	* Have a integer to check if it compile or not
	* create a info log  charater array
	* check compilation succeded with GlShaderiv
	* If it failed then we need to retrieve error message with GetShaderInfoLog
	* and output the error
	*/
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	/*
	*çompile the fragment shader.
	* Similar to vertix shader
	* create a fragment object
	* create the fragment shader
	* add it to shader source
	* compile it
	*/
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	/*
	* Also check for errors here!
	* Same method for checking erros for fragment like with vertexshader.
	*/
	int fragmentsuccess;
	char fragmentinfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentsuccess);
	if (!fragmentsuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentinfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			fragmentinfoLog << std::endl;
	}
	/*
	* Create a shader program
	* create a shaderprogram object
	* and then store the id of the newly created object
	*/
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	/*
	* Attach the compiled shaders
	* first we attach the vertexshader
	* then the fragment
	* and then link the progrm to the shaderobject
	*/
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	/*
	* Check if shaderprogram compiled or not
	* we will use shaderinfolog instead of getshaderIV
	*/
	int shaderprogramsuccess;
	char shaderprograminfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderprogramsuccess);
	if (!shaderprogramsuccess)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, shaderprograminfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			shaderprograminfoLog << std::endl;
	}
	/*
	*Delete the shaders as we will no longer need them.
	*Its good practice to do so.
	*/

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/*
	*We have now stored all the data needed in GPU memory
	* Only need to help OpenGL interpret the results.
	*/

	/*
	*If we want to see the verticesw only then we can use wirefram to see how the objects are connected
	* We can use polygon mode to see that
	* Parameters:
	* if we want to apply it to front and back
	* if we want to fill or draw lines
	* Comment it out to go back to normal mode
	*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//----------------------------------- Vertex and config data here 

	//Comments removed for ease of use in excercise

	float vertice1[] = {
		0.1, 0.4f, 0.0f, // top right
		0.0f, 0.0f, 0.0f, // bottom right
		0.2f, 0.0f, 0.0f, // bottom left
		//Triangle 2
		0.4, 0.4f, 0.0f, // top right
		0.3f, 0.0f, 0.0f, // bottom right
		0.5f, 0.0f, 0.0f, // bottom left
	};
	float vertice2[] = {
		0.4, 0.4f, 0.0f, // top right
		0.3f, 0.0f, 0.0f, // bottom right
		0.5f, 0.0f, 0.0f, // bottom left
	};

	unsigned int VBO[2];
	glGenBuffers(2, VBO);
	unsigned int VAO[2];
	glGenVertexArrays(2, VAO);

	//---Config for First triangle
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice1), vertice1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//VAO set up
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice2), vertice2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	/*
	* Code to only allow explict closing.
	* It is also the render loop
	*/
	while (!glfwWindowShouldClose(window))
	{
		/*
		*input
		*/
		processInput(window);

		/*
		* render
		* MAke sure that clearcolor is before anything else as it will be creating the background
		*/

		//glclear will clear the buffer with the collor given in glclearColor. It is also coloring the background piels
		//The parameters are RGBA meaning Red,Green,Blue and Alpha
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Draw Vertix

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/*
		* GL Draw Array has been commented out as we are going to use EBO which will
		remove the overhead of having same vertices for many triangles.
		*/
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);




		/*
		*check call events
		*/
		//swapbuffers will swap the color buffer
		glfwSwapBuffers(window);
		//Pollevents check if window has been closed or not.
		glfwPollEvents();
	}
	//terminate GLFW at end of program
	glfwTerminate();
	return 0;
}

