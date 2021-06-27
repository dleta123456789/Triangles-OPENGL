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

	/*
	*create  vertice array
	* X,Y,Z coordinates of each vertice
	* Range is from -1.0f to 1.0f on all three of these
	* With three vertices we will have created a traingle
	* With four we will be creating a rectangle
	*
	*/
	float vertices[] = {
		0.2f, 0.4f, 0.0f, // top right
		0.0f, 0.0f, 0.0f, // bottom right
		0.4f, 0.0f, 0.0f, // bottom left
		//second triangle
		0.6f, 0.4f, 0.0f, // top left
		0.4f, 0.0f, 0.0f,
		0.8f, 0.0f, 0.0f,
	};

	/*
	* Indices will be used to point the correct points in the vertices array
	* Without it, we cannot tell which point is for what triangle
	* We start from 0 as 0 is the first element in the array.
	*/
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	/*
	*Create a Vertex Buffer Object which will store the vertices in GPU memory.
	* GlgenBuffer will return list of intergers not used as buffer names.
	* so we are assigning a single id to VBO
	*/

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	/*
	*GlBindBuffers will bind a  buffer object to the target.
	* First parameter is the target while VBO is the id for the buffer
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*
	* Now we need to copy the vertix data into buffer memory
	* Parameters are:
	* buffer to which vertex data will be copied to
	* size of the vetrix
	* the actual data we need to send
	* How GPU should manage the given data.
	* Methods of manging the dat by the GPU are:
	* Stream_Draw: Set data once and used by GPU a few times
	* Static Draw: Set data once and used many times
	* Dynamic Draw:Data changeda lot and used many times.
	*
	* As we are creating a traingle which stays in the same place,we should use static draw
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	/*
	*Create the Vertx array object
	* As we are using coreOpenGL we nned to use this or OpenGL will not work at all
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	/*
	* Bomd the VAO
	*/
	glBindVertexArray(VAO);

	/*
	*copy vertex array into a buffer for OpenGl to use
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	* Now we need to create the EBO object and then give it a id. Similar to VBO and VAO
	* Order should be VBO, VAO and then EBO
	*/
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	/*
	* Just like with VBO we are going to bind and then add it to the Buffer.
	* diffrence is that we are going to use Gl_element array buffer instead of GL_Array Buffer.
	*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);
	/*
	*set vertex attributes pointers
	*We have to tell  OpenGL how to interpret the  vertex data we have provided to it and then enable it
	* Parameters:
	* The vertex attribute we want to configure, we have set it to 0 as we did in vertx shader
	* Size of vertex attribute which isa vector3 i.e x,y,z values which are in vertex
	* Type of data
	* Check if data should be normalized or not. Its needed if we are inputing integer dat type,int,byte in which case we set it to GL_TRUE.
	We dont need it as data is in floating
	* Next is stride. As we know the data is simply 3 times the size of float,we can specify it.
	We can also give it as 0 in which case OpenGL will do itself. putting 0 only works if values are thighly packed.
	* Last is Void*. Its the offset of where the position data begins in the buffer. As it is at start we give it zero
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		glBindVertexArray(VAO);
		/*
		* GL Draw Array has been commented out as we are going to use EBO which will
		remove the overhead of having same vertices for many triangles.
		*GlDrawArrays parameteres:
		*primative type
		*starting index
		* vertices to draw
		*/
		glDrawArrays(GL_TRIANGLES, 0, 6);


		/*
		*We are going to use GL Draw element to get draw the shape
		* Paramenters:
		* The mode in which we want to draw which is triangle.
		Do note that every object which is made is made with the help of triangles.
		Triangles cannot be  non planar.Meaning its does not exist in a single plane
		We are going to render 2d images even if the object itself is a 3d as we are looking thorugh a screen.
		secoondly its more effiecent the less computation we do to compute a single object,
		we can display objects faster and in larger number.
		* We specify the number of elements which are the 6 indices we had.
		* We specify the type of indices which is unisgingned int.
		* Last is the offset in EBO or not using EBO so we set it to 0 as we have no offset
		*/
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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

