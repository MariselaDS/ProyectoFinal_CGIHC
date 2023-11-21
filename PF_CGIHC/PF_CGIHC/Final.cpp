/*-----------------------------------------------------------------*/
/*-----------		Proyecto Final   ------------------------------*/
/*---------------		2024-1   ----------------------------------*/
/*-----------------------------------------------------------------*/
/*-------   Integrantes del equipo:					---------------*/
/*-------   417006480  Dionicio Serna Marisela		---------------*/
/*-------   315138151  Hernández Sánchez Brenda		---------------*/
/*-----------------------------------------------------------------*/

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//Audio
#pragma comment(lib, "winmm")
#include <Windows.h>
#include "MMsystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// Cámara 
Camera camera(glm::vec3(0.0f, 40.0f, 260.0f));
float MovementSpeed = 0.5f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, 1.0f, 0.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f,
        circuito_auto = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

float tiempo;
//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 63
int i_max_steps = 1
;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 63;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].giroMonito = giroMonito;
	std::cout << "posicion x = " << posX << std::endl;
	std::cout << "posicion y = " << posY << std::endl;
	std::cout << "posicion z = " << posZ << std::endl;
	std::cout << "posicion giroMonito = " << giroMonito << std::endl;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion) 
	{
		if (circuito_auto == 0)    
		{
			movAuto_x += 1.0f;
			orienta = 0.0f;
			if (movAuto_x >= 29.0f)
				circuito_auto = 1;
		}
		if (circuito_auto == 1)
		{
			movAuto_z -= 1.0f;
			orienta = 90.0f;
			if (movAuto_z <= -17.0f)
				circuito_auto = 2;
		}
		if (circuito_auto == 2)
		{
			movAuto_x -= 1.0f;
			orienta = 180;
			if (movAuto_x <= 0.0f)
				circuito_auto = 3;
		}
		if (circuito_auto == 3)
		{
			movAuto_z += 1.0f;
			orienta = 270.0f;
			if (movAuto_z >= 0.0f)
				circuito_auto = 0;
		}
	}
	
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Museo", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
			"resources/skybox/left.jpg",
			"resources/skybox/top.jpg",
			"resources/skybox/bottom.jpg",
			"resources/skybox/front.jpg",
			"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/PisoArboles.obj");
	Model baby("resources/objects/babyface/babyface.obj");
	Model sullivan("resources/objects/sullivan/sullivan.obj");
	Model lago("resources/objects/Lago/lago.obj");
	Model fuente("resources/objects/Fuente/Fuen.obj");
	Model museo("resources/objects/museo/museo/museo.obj");
	Model techo("resources/objects/Techo/techo.obj");
	Model banca("resources/objects/Silla/Silla.obj");
	Model caballo("resources/objects/tiroAlBlanco/tiroalblanco.obj");
	Model stitch("resources/objects/Stitch/Stitch.obj");
	Model lilo("resources/objects/Lilo/lilo.obj");
	Model mcqueen("resources/objects/McQueen/mcqueenCarroceria.obj");
	Model llanta("resources/objects/McQueen/Wheel.obj");
	Model aladin("resources/objects/Aladin/DP_Aladdin_T.obj");
	Model nala("resources/objects/Nala/DP_Nala.obj");
	Model espantapajaros("resources/objects/espantapajaros/espantapajaros.obj");
	Model yubaba("resources/objects/yubaba/yubaba.obj");
	Model avion("resources/objects/porcoAvion/porcoAvion.obj");
	Model chihiro("resources/objects/Chihiro/SG_Chihiro.obj");
	Model ohmu("resources/objects/Ohmu/SG_Ohmu.obj");
	Model princesa("resources/objects/Princesa/SG_PrincesaMononoke.obj");
	Model bugs("resources/objects/BugsBunny/BugsBunny.obj");
	Model pato("resources/objects/PatoLucas/DaffyDuck.obj");
	Model coyote("resources/objects/Coyote/WB_Coyote.obj");
	Model correCaminos("resources/objects/CorreCaminos/WB_CorreCaminos.obj");
	Model taz("resources/objects/Taz/WB_Taz.obj");
	Model marvin("resources/objects/Marvin/WB_Marvin.obj");
	Model scooby("resources/objects/Scooby/WB_Scooby.obj");
	Model vilma("resources/objects/Vilma/WB_Vilma.obj");
	Model carroPicapiedras("resources/objects/CarroPicapiedras/WB_CarroPicapiedras.obj");
	Model pedro("resources/objects/Pedro/WB_Pedro.obj");
	ModelAnim animacionPersonaje("resources/objects/Personaje/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);


	//Inicialización de KeyFrames
	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].posZ = 0.0f;
	KeyFrame[0].giroMonito = -45.0f;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 1.0f;
	KeyFrame[1].posZ = 1.0f;
	KeyFrame[1].giroMonito = -45.0f;

	KeyFrame[2].posX = 1.0f;
	KeyFrame[2].posY = 2.0f;
	KeyFrame[2].posZ = 2.0f;
	KeyFrame[2].giroMonito = -45.0f;

	KeyFrame[3].posX = 2.0f;
	KeyFrame[3].posY = 3.0f;
	KeyFrame[3].posZ = 3.0f;
	KeyFrame[3].giroMonito = -45.0f;

	KeyFrame[4].posX = 3.0f;
	KeyFrame[4].posY = 4.0f;
	KeyFrame[4].posZ = 4.0f;
	KeyFrame[4].giroMonito = -45.0f;

	KeyFrame[5].posX = 4.0f;
	KeyFrame[5].posY = 3.0f;
	KeyFrame[5].posZ = 5.0f;
	KeyFrame[5].giroMonito = -45.0f;

	KeyFrame[6].posX = 5.0f;
	KeyFrame[6].posY = 2.0f;
	KeyFrame[6].posZ = 6.0f;
	KeyFrame[6].giroMonito = -45.0f;

	KeyFrame[6].posX = 6.0f;
	KeyFrame[6].posY = 1.0f;
	KeyFrame[6].posZ = 7.0f;
	KeyFrame[6].giroMonito = -45.0f;

	KeyFrame[7].posX = 7.0f;
	KeyFrame[7].posY = 0.0f;
	KeyFrame[7].posZ = 8.0f;
	KeyFrame[7].giroMonito = -45.0f;

	KeyFrame[8].posX = 7.0f;
	KeyFrame[8].posY = 1.0f;
	KeyFrame[8].posZ = 8.0f;
	KeyFrame[8].giroMonito = -45.0f;

	KeyFrame[9].posX = 8.0f;
	KeyFrame[9].posY = 2.0f;
	KeyFrame[9].posZ = 9.0f;
	KeyFrame[9].giroMonito = -45.0f;

	KeyFrame[10].posX = 9.0f;
	KeyFrame[10].posY = 3.0f;
	KeyFrame[10].posZ = 10.0f;
	KeyFrame[10].giroMonito = -45.0f;

	KeyFrame[11].posX = 10.0f;
	KeyFrame[11].posY = 4.0f;
	KeyFrame[11].posZ = 11.0f;
	KeyFrame[11].giroMonito = -45.0f;

	KeyFrame[12].posX = 11.0f;
	KeyFrame[12].posY = 3.0f;
	KeyFrame[12].posZ = 12.0f;
	KeyFrame[12].giroMonito = -45.0f;

	KeyFrame[13].posX = 12.0f;
	KeyFrame[13].posY = 2.0f;
	KeyFrame[13].posZ = 13.0f;
	KeyFrame[13].giroMonito = -45.0f;

	KeyFrame[14].posX = 13.0f;
	KeyFrame[14].posY = 1.0f;
	KeyFrame[14].posZ = 14.0f;
	KeyFrame[14].giroMonito = -45.0f;

	KeyFrame[15].posX = 14.0f;
	KeyFrame[15].posY = 0.0f;
	KeyFrame[15].posZ = 15.0f;
	KeyFrame[15].giroMonito = -45.0f;

	KeyFrame[16].posX = 14.0f;
	KeyFrame[16].posY = 1.0f;
	KeyFrame[16].posZ = 15.0f;
	KeyFrame[16].giroMonito = 45.0f;

	KeyFrame[17].posX = 15.0f;
	KeyFrame[17].posY = 2.0f;
	KeyFrame[17].posZ = 14.0f;
	KeyFrame[17].giroMonito = 45.0f;

	KeyFrame[18].posX = 16.0f;
	KeyFrame[18].posY = 3.0f;
	KeyFrame[18].posZ = 13.0f;
	KeyFrame[18].giroMonito = 45.0f;

	KeyFrame[19].posX = 17.0f;
	KeyFrame[19].posY = 4.0f;
	KeyFrame[19].posZ = 12.0f;
	KeyFrame[19].giroMonito = 45.0f;

	KeyFrame[20].posX = 18.0f;
	KeyFrame[20].posY = 3.0f;
	KeyFrame[20].posZ = 11.0f;
	KeyFrame[20].giroMonito = 45.0f;

	KeyFrame[21].posX = 19.0f;
	KeyFrame[21].posY = 2.0f;
	KeyFrame[21].posZ = 10.0f;
	KeyFrame[21].giroMonito = 45.0f;

	KeyFrame[22].posX = 20.0f;
	KeyFrame[22].posY = 1.0f;
	KeyFrame[22].posZ = 9.0f;
	KeyFrame[22].giroMonito = 45.0f;

	KeyFrame[23].posX = 21.0f;
	KeyFrame[23].posY = 0.0f;
	KeyFrame[23].posZ = 8.0f;
	KeyFrame[23].giroMonito = 45.0f;


	KeyFrame[24].posX = 21.0f;
	KeyFrame[24].posY = 1.0f;
	KeyFrame[24].posZ = 7.0f;
	KeyFrame[24].giroMonito = 45.0f;

	KeyFrame[25].posX = 22.0f;
	KeyFrame[25].posY = 2.0f;
	KeyFrame[25].posZ = 6.0f;
	KeyFrame[25].giroMonito = 45.0f;

	KeyFrame[26].posX = 23.0f;
	KeyFrame[26].posY = 3.0f;
	KeyFrame[26].posZ = 5.0f;
	KeyFrame[26].giroMonito = 45.0f;

	KeyFrame[27].posX = 24.0f;
	KeyFrame[27].posY = 4.0f;
	KeyFrame[27].posZ = 4.0f;
	KeyFrame[27].giroMonito = 45.0f;

	KeyFrame[28].posX = 25.0f;
	KeyFrame[28].posY = 3.0f;
	KeyFrame[28].posZ = 3.0f;
	KeyFrame[28].giroMonito = 45.0f;

	KeyFrame[29].posX = 26.0f;
	KeyFrame[29].posY = 2.0f;
	KeyFrame[29].posZ = 2.0f;
	KeyFrame[29].giroMonito = 45.0f;

	KeyFrame[30].posX = 27.0f;
	KeyFrame[30].posY = 1.0f;
	KeyFrame[30].posZ = 1.0f;
	KeyFrame[30].giroMonito = 45.0f;

	KeyFrame[31].posX = 28.0f;
	KeyFrame[31].posY = 0.0f;
	KeyFrame[31].posZ = 1.0f;
	KeyFrame[31].giroMonito = 45.0f;

	KeyFrame[32].posX = 28.0f;
	KeyFrame[32].posY = 1.0f;
	KeyFrame[32].posZ = 0.0f;
	KeyFrame[32].giroMonito = 135.0f;

	KeyFrame[33].posX = 27.0f;
	KeyFrame[33].posY = 2.0f;
	KeyFrame[33].posZ = -1.0f;
	KeyFrame[33].giroMonito = 135.0f;

	KeyFrame[34].posX = 26.0f;
	KeyFrame[34].posY = 3.0f;
	KeyFrame[34].posZ = -2.0f;
	KeyFrame[34].giroMonito = 135.0f;

	KeyFrame[35].posX = 25.0f;
	KeyFrame[35].posY = 4.0f;
	KeyFrame[35].posZ = -3.0f;
	KeyFrame[35].giroMonito = 135.0f;

	KeyFrame[36].posX = 24.0f;
	KeyFrame[36].posY = 3.0f;
	KeyFrame[36].posZ = -4.0f;
	KeyFrame[36].giroMonito = 135.0f;

	KeyFrame[37].posX = 23.0f;
	KeyFrame[37].posY = 2.0f;
	KeyFrame[37].posZ = -5.0f;
	KeyFrame[37].giroMonito = 135.0f;

	KeyFrame[38].posX = 22.0f;
	KeyFrame[38].posY = 1.0f;
	KeyFrame[38].posZ = -6.0f;
	KeyFrame[38].giroMonito = 135.0f;

	KeyFrame[39].posX = 21.0f;
	KeyFrame[39].posY = 0.0f;
	KeyFrame[39].posZ = -7.0f;
	KeyFrame[39].giroMonito = 135.0f;

	KeyFrame[40].posX = 21.0f;
	KeyFrame[40].posY = 1.0f;
	KeyFrame[40].posZ = -8.0f;
	KeyFrame[40].giroMonito = 135.0f;

	KeyFrame[41].posX = 19.0f;
	KeyFrame[41].posY = 2.0f;
	KeyFrame[41].posZ = -9.0f;
	KeyFrame[41].giroMonito = 135.0f;

	KeyFrame[42].posX = 18.0f;
	KeyFrame[42].posY = 3.0f;
	KeyFrame[42].posZ = -10.0f;
	KeyFrame[42].giroMonito = 135.0f;

	KeyFrame[43].posX = 17.0f;
	KeyFrame[43].posY = 4.0f;
	KeyFrame[43].posZ = -11.0f;
	KeyFrame[43].giroMonito = 135.0f;

	KeyFrame[44].posX = 16.0f;
	KeyFrame[44].posY = 3.0f;
	KeyFrame[44].posZ = -12.0f;
	KeyFrame[44].giroMonito = 135.0f;

	KeyFrame[45].posX = 15.0f;
	KeyFrame[45].posY = 2.0f;
	KeyFrame[45].posZ = -13.0f;
	KeyFrame[45].giroMonito = 135.0f;

	KeyFrame[46].posX = 14.0f;
	KeyFrame[46].posY = 1.0f;
	KeyFrame[46].posZ = -14.0f;
	KeyFrame[46].giroMonito = 135.0f;

	KeyFrame[47].posX = 13.0f;
	KeyFrame[47].posY = 0.0f;
	KeyFrame[47].posZ = -15.0f;
	KeyFrame[47].giroMonito = 225.0f;

	KeyFrame[48].posX = 13.0f;
	KeyFrame[48].posY = 1.0f;
	KeyFrame[48].posZ = -13.0f;
	KeyFrame[48].giroMonito = 225.0f;

	KeyFrame[49].posX = 11.0f;
	KeyFrame[49].posY = 2.0f;
	KeyFrame[49].posZ = -12.0f;
	KeyFrame[49].giroMonito = 225.0f;

	KeyFrame[50].posX = 10.0f;
	KeyFrame[50].posY = 3.0f;
	KeyFrame[50].posZ = -11.0f;
	KeyFrame[50].giroMonito = 225.0f;

	KeyFrame[51].posX = 9.0f;
	KeyFrame[51].posY = 2.0f;
	KeyFrame[51].posZ = -10.0f;
	KeyFrame[51].giroMonito = 225.0f;

	KeyFrame[52].posX = 8.0f;
	KeyFrame[52].posY = 1.0f;
	KeyFrame[52].posZ = -9.0f;
	KeyFrame[52].giroMonito = 225.0f;

	KeyFrame[53].posX = 7.0f;
	KeyFrame[53].posY = 0.0f;
	KeyFrame[53].posZ = -8.0f;
	KeyFrame[53].giroMonito = 225.0f;

	KeyFrame[54].posX = 6.0f;
	KeyFrame[54].posY = 1.0f;
	KeyFrame[54].posZ = -7.0f;
	KeyFrame[54].giroMonito = 225.0f;

	KeyFrame[55].posX = 5.0f;
	KeyFrame[55].posY = 2.0f;
	KeyFrame[55].posZ = -6.0f;
	KeyFrame[55].giroMonito = 225.0f;

	KeyFrame[56].posX = 4.0f;
	KeyFrame[56].posY = 3.0f;
	KeyFrame[56].posZ = -5.0f;
	KeyFrame[56].giroMonito = 225.0f;

	KeyFrame[57].posX = 3.0f;
	KeyFrame[57].posY = 4.0f;
	KeyFrame[57].posZ = -4.0f;
	KeyFrame[57].giroMonito = 225.0f;

	KeyFrame[58].posX = 2.0f;
	KeyFrame[58].posY = 3.0f;
	KeyFrame[58].posZ = -3.0f;
	KeyFrame[58].giroMonito = 225.0f;

	KeyFrame[59].posX = 1.0f;
	KeyFrame[59].posY = 2.0f;
	KeyFrame[59].posZ = -2.0f;
	KeyFrame[59].giroMonito = 225.0f;

	KeyFrame[60].posX = 0.0f;
	KeyFrame[60].posY = 1.0f;
	KeyFrame[60].posZ = -1.0f;
	KeyFrame[60].giroMonito = 225.0f;

	KeyFrame[61].posX = 0.0f;
	KeyFrame[61].posY = 0.0f;
	KeyFrame[61].posZ = 0.0f;
	KeyFrame[61].giroMonito = 225.0f;

	KeyFrame[62].posX = 0.0f;
	KeyFrame[62].posY = 0.0f;
	KeyFrame[62].posZ = 0.0f;
	KeyFrame[62].giroMonito = 0.0f;
	

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.3f, 0.3f, 0.3f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.3f, 1.5f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.2f, 5.0f));
		model = glm::scale(model, glm::vec3(0.13f));
		staticShader.setMat4("model", model);
		museo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.2f, 5.0f));
		model = glm::scale(model, glm::vec3(0.13f));
		staticShader.setMat4("model", model);
		techo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, -48.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		lago.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(-65.0f, 0.9f, -110.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-65.0f, 0.9f, -145.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, 0.9f, -175.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 0.9f, -175.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, 0.9f, -145.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, 0.9f, -110.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, 1.0f, 160.0f));
		model = glm::scale(model, glm::vec3(3.3f));
		staticShader.setMat4("model", model);
		fuente.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, 1.0f, 160.0f));
		model = glm::scale(model, glm::vec3(3.3f));
		staticShader.setMat4("model", model);
		fuente.Draw(staticShader);


		// SALA DISNEY PIXAR

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0f, 1.0f, 48.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
	    baby.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, 1.0f, 65.0f));
		model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.06f));
		staticShader.setMat4("model", model);
		caballo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-54.0f, 1.0f, 28.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.065f));
		staticShader.setMat4("model", model);
		aladin.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, 1.0f, 26.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.11f));
		staticShader.setMat4("model", model);
		nala.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-52.0f, 0.7f, 89.0f));
		model = glm::translate(model, glm::vec3(movAuto_x, 0.0f, movAuto_z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		mcqueen.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);//der del
		llanta.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-0.2f, 0.0f, -5.2f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model); //der tra
		llanta.Draw(staticShader);
		
		model = glm::translate(tmp, glm::vec3(.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model); //izq tras
		llanta.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 5.2f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model); //izq tras
		llanta.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-57.0f, 1.0f, 45.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		sullivan.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 1.0f, 69.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f));
		staticShader.setMat4("model", model);
		stitch.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 1.0f, 55.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		lilo.Draw(staticShader);

		//SALA STUDIO GHIBLI

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-57.0f, 0.0f, -20.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		espantapajaros.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f, 10.0f, -117.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		yubaba.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-65.0f, 5.0f, -45.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		avion.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 1.0f, -65.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		chihiro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 3.0f, -20.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		ohmu.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 1.0f, -65.0f));
		model = glm::scale(model, glm::vec3(0.21f));
		staticShader.setMat4("model", model);
		princesa.Draw(staticShader);

		
		// SALA WARNER BROS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 1.0f, 87.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		bugs.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 87.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		pato.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(66.0f, 1.0f, 83.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		coyote.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(66.0f, 1.0f, 65.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f));
		staticShader.setMat4("model", model);
		correCaminos.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(66.0f, 1.0f, 49.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f));
		staticShader.setMat4("model", model);
		taz.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(66.0f, 1.0f, 28.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		staticShader.setMat4("model", model);
		marvin.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(66.0f, 1.0f, 12.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		scooby.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		vilma.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(33.0f, 1.0f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		staticShader.setMat4("model", model);
		carroPicapiedras.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(33.0f, 1.0f, 40.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		pedro.Draw(staticShader);

		// Personaje animado

		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 30.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(0.85f));
		animShader.setMat4("model", model);
		animacionPersonaje.Draw(animShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animacion ^= true;
		circuito_auto = 0;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		movAuto_x = 0.0f;
		animacion = false;

	}

	//Song
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
			PlaySound(TEXT("SongToyStory.wav"), NULL, SND_ASYNC);;
		
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
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
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}