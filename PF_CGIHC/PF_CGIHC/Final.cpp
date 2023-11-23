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
Camera camera(glm::vec3(0.0f, 40.0f, 320.0f));
float MovementSpeed = 0.1f;
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
float	giroCaballoTorso = 0.0f,
		giroCaballoCabeza = 0.0f,
		giroCola = 0.0f,
		giroPD1arriba = 0.0f, giroPD1abajo = 0.0f,
		giroPD2arriba = 0.0f, giroPD2abajo = 0.0f, giroPDbase = 0.0f,
		giroPT1arriba = 0.0f, giroPT1abajo = 0.0f,
		giroPT2arriba = 0.0f, giroPT2abajo = 0.0f;
float	bf_Y = 0.0f,
		bf_Z = 0.0f,
		gCabeza = 0.0f,
		gP1_PataA = 0.0f,
		gP2_PataA = 0.0f,
		gP3_PataA = 0.0f,
		gP1_PataBC = 0.0f,
		gP2_PataBC = 0.0f,
		gP3_PataBC = 0.0f,
		gPinzas = 0.0f,
		gOjo = 0.0f;

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		giroMonitoInc = 0.0f;
float	giroCaballoTorsoInc = 0.0f,
		giroCaballoCabezaInc = 0.0f,
		giroColaInc = 0.0f,
		giroPD1arribaInc = 0.0f, giroPD1abajoInc = 0.0f,
		giroPD2arribaInc = 0.0f, giroPD2abajoInc = 0.0f, giroPDbaseInc = 0.0f,
		giroPT1arribaInc = 0.0f, giroPT1abajoInc = 0.0f,
		giroPT2arribaInc = 0.0f, giroPT2abajoInc = 0.0f;
float	bf_YInc = 0.0f,
		bf_ZInc = 0.0f,
		gCabezaInc = 0.0f,
		gP1_PataAInc = 0.0f,
		gP2_PataAInc = 0.0f,
		gP3_PataAInc = 0.0f,
		gP1_PataBCInc = 0.0f,
		gP2_PataBCInc = 0.0f,
		gP3_PataBCInc = 0.0f,
		gPinzasInc = 0.0f,
		gOjoInc = 0.0f;


#define MAX_FRAMES 70
int i_max_steps = 10;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float giroMonito;
	float	giroCaballoTorso,
			giroCaballoCabeza,
			giroCola,
			giroPD1arriba, giroPD1abajo,
			giroPD2arriba, giroPD2abajo, giroPDbase,
			giroPT2arriba, giroPT2abajo,
			giroPT1arriba, giroPT1abajo;
	float	bf_Y,
			bf_Z,
			gCabeza,
			gP1_PataA,
			gP2_PataA,
			gP3_PataA,
			gP1_PataBC,
			gP2_PataBC,
			gP3_PataBC,
			gPinzas,
			gOjo;
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
	
	KeyFrame[FrameIndex].giroCaballoTorso = giroCaballoTorso;
	KeyFrame[FrameIndex].giroCaballoCabeza = giroCaballoCabeza;
	KeyFrame[FrameIndex].giroCola = giroCola;
	KeyFrame[FrameIndex].giroPD1arriba = giroPD1arriba; KeyFrame[FrameIndex].giroPD1abajo = giroPD1abajo;
	KeyFrame[FrameIndex].giroPD2arriba = giroPD2arriba; KeyFrame[FrameIndex].giroPD2abajo = giroPD2abajo; KeyFrame[FrameIndex].giroPDbase = giroPDbase;
	KeyFrame[FrameIndex].giroPT1arriba = giroPT1arriba; KeyFrame[FrameIndex].giroPT1abajo = giroPT1abajo;
	KeyFrame[FrameIndex].giroPT2arriba = giroPT2arriba; KeyFrame[FrameIndex].giroPT2abajo = giroPT2abajo;
	std::cout << giroCaballoTorso << " "
		<< giroCaballoCabeza << " "
		<< giroCola << " "
		<< giroPD1arriba << " " << giroPD1abajo << " "
		<< giroPD2arriba << " " << giroPD2abajo << " " << giroPDbase
		<< " " << giroPT1arriba << " " << giroPT1abajo
		<< " " << giroPT2arriba << " " << giroPT2abajo << std::endl;

	KeyFrame[FrameIndex].bf_Y = bf_Y;
	KeyFrame[FrameIndex].bf_Z = bf_Z;
	KeyFrame[FrameIndex].gCabeza = gCabeza;
	KeyFrame[FrameIndex].gP1_PataA = gP1_PataA;
	KeyFrame[FrameIndex].gP2_PataA = gP2_PataA;
	KeyFrame[FrameIndex].gP3_PataA = gP3_PataA;
	KeyFrame[FrameIndex].gP1_PataBC = gP1_PataBC;
	KeyFrame[FrameIndex].gP2_PataBC = gP2_PataBC;
	KeyFrame[FrameIndex].gP3_PataBC = gP3_PataBC;
	KeyFrame[FrameIndex].gPinzas = gPinzas;
	KeyFrame[FrameIndex].gOjo = gOjo;
	std::cout << bf_Y << " "
		<< bf_Z << " "
		<< gCabeza << " "
		<< gP1_PataA << " "
		<< gP2_PataA << " "
		<< gP3_PataA << " "
		<< gP1_PataBC << " "
		<< gP2_PataBC << " "
		<< gP3_PataBC << " "
		<< gPinzas << " "
		<< gOjo << std::endl;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	giroMonito = KeyFrame[0].giroMonito;

	giroCaballoTorso = KeyFrame[0].giroCaballoTorso;
	giroCaballoCabeza = KeyFrame[0].giroCaballoCabeza;
	giroCola = KeyFrame[0].giroCola;
	giroPD1arriba = KeyFrame[0].giroPD1arriba; giroPD1abajo = KeyFrame[0].giroPD1abajo;
	giroPD2arriba = KeyFrame[0].giroPD2arriba; giroPD2abajo = KeyFrame[0].giroPD2abajo; giroPDbase = KeyFrame[0].giroPDbase;
	giroPT1arriba = KeyFrame[0].giroPT1arriba; giroPT1abajo = KeyFrame[0].giroPT1abajo;
	giroPT2arriba = KeyFrame[0].giroPT2arriba; giroPT2abajo = KeyFrame[0].giroPT2abajo;

	bf_Y = KeyFrame[0].bf_Y;
	bf_Z = KeyFrame[0].bf_Z;
	gCabeza = KeyFrame[0].gCabeza;
	gP1_PataA = KeyFrame[0].gP1_PataA;
	gP2_PataA = KeyFrame[0].gP2_PataA;
	gP3_PataA = KeyFrame[0].gP3_PataA;
	gP1_PataBC = KeyFrame[0].gP1_PataBC;
	gP2_PataBC = KeyFrame[0].gP2_PataBC;
	gP3_PataBC = KeyFrame[0].gP3_PataBC;
	gPinzas = KeyFrame[0].gPinzas;
	gOjo = KeyFrame[0].gOjo;
}


void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	giroCaballoTorsoInc = (KeyFrame[playIndex + 1].giroCaballoTorso - KeyFrame[playIndex].giroCaballoTorso) / i_max_steps;
	giroCaballoCabezaInc = (KeyFrame[playIndex + 1].giroCaballoCabeza - KeyFrame[playIndex].giroCaballoCabeza) / i_max_steps;
	giroColaInc = (KeyFrame[playIndex + 1].giroCola - KeyFrame[playIndex].giroCola) / i_max_steps;
	giroPD1arribaInc = (KeyFrame[playIndex + 1].giroPD1arriba - KeyFrame[playIndex].giroPD1arriba) / i_max_steps;
	giroPD1abajoInc = (KeyFrame[playIndex + 1].giroPD1abajo - KeyFrame[playIndex].giroPD1abajo) / i_max_steps;
	giroPD2arribaInc = (KeyFrame[playIndex + 1].giroPD2arriba - KeyFrame[playIndex].giroPD2arriba) / i_max_steps;
	giroPD2abajoInc = (KeyFrame[playIndex + 1].giroPD2abajo - KeyFrame[playIndex].giroPD2abajo) / i_max_steps;
	giroPDbaseInc = (KeyFrame[playIndex + 1].giroPDbase - KeyFrame[playIndex].giroPDbase) / i_max_steps;
	giroPT1arribaInc = (KeyFrame[playIndex + 1].giroPT1arriba - KeyFrame[playIndex].giroPT1arriba) / i_max_steps;
	giroPT1abajoInc = (KeyFrame[playIndex + 1].giroPT1abajo - KeyFrame[playIndex].giroPT1abajo) / i_max_steps;
	giroPT2arribaInc = (KeyFrame[playIndex + 1].giroPT2arriba - KeyFrame[playIndex].giroPT2arriba) / i_max_steps;
	giroPT2abajoInc = (KeyFrame[playIndex + 1].giroPT2abajo - KeyFrame[playIndex].giroPT2abajo) / i_max_steps;

	bf_YInc = (KeyFrame[playIndex + 1].bf_Y - KeyFrame[playIndex].bf_Y) / i_max_steps;
	bf_ZInc = (KeyFrame[playIndex + 1].bf_Z - KeyFrame[playIndex].bf_Z) / i_max_steps;
	gCabezaInc = (KeyFrame[playIndex + 1].gCabeza - KeyFrame[playIndex].gCabeza) / i_max_steps;
	gP1_PataAInc = (KeyFrame[playIndex + 1].gP1_PataA - KeyFrame[playIndex].gP1_PataA) / i_max_steps;
	gP2_PataAInc = (KeyFrame[playIndex + 1].gP2_PataA - KeyFrame[playIndex].gP2_PataA) / i_max_steps;
	gP3_PataAInc = (KeyFrame[playIndex + 1].gP3_PataA - KeyFrame[playIndex].gP3_PataA) / i_max_steps;
	gP1_PataBCInc = (KeyFrame[playIndex + 1].gP1_PataBC - KeyFrame[playIndex].gP1_PataBC) / i_max_steps;
	gP2_PataBCInc = (KeyFrame[playIndex + 1].gP2_PataBC - KeyFrame[playIndex].gP2_PataBC) / i_max_steps;
	gP3_PataBCInc = (KeyFrame[playIndex + 1].gP3_PataBC - KeyFrame[playIndex].gP3_PataBC) / i_max_steps;
	gPinzasInc = (KeyFrame[playIndex + 1].gPinzas - KeyFrame[playIndex].gPinzas) / i_max_steps;
	gOjoInc = (KeyFrame[playIndex + 1].gOjo - KeyFrame[playIndex].gOjo) / i_max_steps;
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

			giroCaballoTorso += giroCaballoTorsoInc;
			giroCaballoCabeza += giroCaballoCabezaInc;
			giroCola += giroColaInc;
			giroPD1arriba += giroPD1arribaInc; giroPD1abajo += giroPD1abajoInc;
			giroPD2arriba += giroPD2arribaInc; giroPD2abajo += giroPD2abajoInc; giroPDbase += giroPDbaseInc;
			giroPT1arriba += giroPT1arribaInc; giroPT1abajo += giroPT1abajoInc;
			giroPT2arriba += giroPT2arribaInc; giroPT2abajo += giroPT2abajoInc;

			bf_Y += bf_YInc;
			bf_Z += bf_ZInc;
			gCabeza += gCabezaInc;
			gP1_PataA += gP1_PataAInc;
			gP2_PataA += gP2_PataAInc;
			gP3_PataA += gP3_PataAInc;
			gP1_PataBC += gP1_PataBCInc;
			gP2_PataBC += gP2_PataBCInc;
			gP3_PataBC += gP3_PataBCInc;
			gPinzas += gPinzas;
			gOjo += gOjo;

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
	Model sullivan("resources/objects/sullivan/sullivan.obj");
	Model lago("resources/objects/Lago/lago.obj");
	Model fuente("resources/objects/Fuente/Fuen.obj");
	Model museo("resources/objects/museo/museo/museo.obj");
	Model techo("resources/objects/Techo/techo.obj");
	Model banca("resources/objects/Silla/Silla.obj");
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

	//Carga de modelos para construir 'Tiro al Blanco' 
	Model cabeza_caballo("resources/objects/caballo_por_piezas/cabeza_caballoTS.obj");
	Model torso_caballo("resources/objects/caballo_por_piezas/cuerpo_caballoTS.obj");
	Model cola_caballo("resources/objects/caballo_por_piezas/cola_caballoTS.obj");
	Model pataDel1_arriba_caballo("resources/objects/caballo_por_piezas/pataDel1_arriba_caballoTS.obj");
	Model pataDel1_abajo_caballo("resources/objects/caballo_por_piezas/pataDel1_abajo_caballoTS.obj");
	Model pataDel1_planta_caballo("resources/objects/caballo_por_piezas/pataDel1_planta_caballoTS.obj");
	Model pataDel2_arriba_caballo("resources/objects/caballo_por_piezas/pataDel2_arriba_caballoTS.obj");
	Model pataDel2_abajo_caballo("resources/objects/caballo_por_piezas/pataDel2_abajo_caballoTS.obj");
	Model pataDel2_planta_caballo("resources/objects/caballo_por_piezas/pataDel2_planta_caballoTS.obj");
	Model pataTras1_arriba_caballo("resources/objects/caballo_por_piezas/pataTras1_arriba_caballoTS.obj");
	Model pataTras1_abajo_caballo("resources/objects/caballo_por_piezas/pataTras1_abajo_caballoTS.obj");
	Model pataTras1_planta_caballo("resources/objects/caballo_por_piezas/pataTras1_planta_caballoTS.obj");
	Model pataTras2_arriba_caballo("resources/objects/caballo_por_piezas/pataTras2_arriba_caballoTS.obj");
	Model pataTras2_abajo_caballo("resources/objects/caballo_por_piezas/pataTras2_abajo_caballoTS.obj");
	Model pataTras2_planta_caballo("resources/objects/caballo_por_piezas/pataTras2_planta_caballoTS.obj");

	//Carga de modelos para construir 'BabyFace' 
	Model alfombra_babyface("resources/objects/babyface_por_piezas/babyface_Alfombra.obj");
	Model cuerpo_babyface("resources/objects/babyface_por_piezas/babyface_Base.obj");
	Model cabeza_babyface("resources/objects/babyface_por_piezas/babyface_Cabeza.obj");
	Model ojo_babyface("resources/objects/babyface_por_piezas/babyface_Ojo.obj");
	Model par1_PataA_Del("resources/objects/babyface_por_piezas/babyface_Par1_PataA_Del.obj");
	Model par1_PataA_Tras("resources/objects/babyface_por_piezas/babyface_Par1_PataA_Tras.obj");
	Model par2_PataA_Del("resources/objects/babyface_por_piezas/babyface_Par2_PataA_Del.obj");
	Model par2_PataA_Tras("resources/objects/babyface_por_piezas/babyface_Par2_PataA_Tras.obj");
	Model par3_PataA_Izq("resources/objects/babyface_por_piezas/babyface_Par3_PataA_Izq.obj");
	Model par3_PataA_Der("resources/objects/babyface_por_piezas/babyface_Par3_PataA_Der.obj");
	Model par1_PataBC_Del("resources/objects/babyface_por_piezas/babyface_Par1_PataBC_Del.obj");
	Model par1_PataBC_Tras("resources/objects/babyface_por_piezas/babyface_Par1_PataBC_Tras.obj");
	Model par2_PataBC_Del("resources/objects/babyface_por_piezas/babyface_Par2_PataBC_Del.obj");
	Model par2_PataBC_Tras("resources/objects/babyface_por_piezas/babyface_Par2_PataBC_Tras.obj");
	Model par3_PataBC_Izq("resources/objects/babyface_por_piezas/babyface_Par3_PataBC_Izq.obj");
	Model par3_PataBC_Der("resources/objects/babyface_por_piezas/babyface_Par3_PataBC_Der.obj");
	Model brazo_Izq("resources/objects/babyface_por_piezas/babyface_Brazo_Izq.obj");
	Model brazo_Der("resources/objects/babyface_por_piezas/babyface_Brazo_Der.obj");
	Model pinza_Izq_Sup("resources/objects/babyface_por_piezas/babyface_Pinza_Izq_Sup.obj");
	Model pinza_Izq_Inf("resources/objects/babyface_por_piezas/babyface_Pinza_Izq_Inf.obj");
	Model pinza_Der_Sup("resources/objects/babyface_por_piezas/babyface_Pinza_Der_Sup.obj");
	Model pinza_Der_Inf("resources/objects/babyface_por_piezas/babyface_Pinza_Der_Inf.obj");



	//Inicialización de KeyFrames
	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].posZ = 0.0f;
	KeyFrame[0].giroMonito = -45.0f;

	KeyFrame[0].giroCaballoTorso = 0.0f;
	KeyFrame[0].giroCaballoCabeza = 0.0f;
	KeyFrame[0].giroCola = 0.0f;
	KeyFrame[0].giroPD1arriba = 0.0f;
	KeyFrame[0].giroPD1abajo = 0.0f;
	KeyFrame[0].giroPD2arriba = 0.0f;
	KeyFrame[0].giroPD2abajo = 0.0f;
	KeyFrame[0].giroPDbase = 0.0f;
	KeyFrame[0].giroPT2arriba = 0.0f;
	KeyFrame[0].giroPT2abajo = 0.0f;
	KeyFrame[0].giroPT1arriba = 0.0f;
	KeyFrame[0].giroPT1abajo = 0.0f;

	KeyFrame[0].bf_Y = 0.0f;
	KeyFrame[0].bf_Z = 0.0f;
	KeyFrame[0].gCabeza = 0.0f;
	KeyFrame[0].gP1_PataA = 0.0f;
	KeyFrame[0].gP2_PataA = 0.0f;
	KeyFrame[0].gP3_PataA = 0.0f;
	KeyFrame[0].gP1_PataBC = 0.0f;
	KeyFrame[0].gP2_PataBC = 0.0f;
	KeyFrame[0].gP3_PataBC = 0.0f;
	KeyFrame[0].gPinzas = 0.0f;
	KeyFrame[0].gOjo = 0.0f;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 1.0f;
	KeyFrame[1].posZ = 1.0f;
	KeyFrame[1].giroMonito = -45.0f;

	KeyFrame[1].giroCaballoTorso = 0.0f;
	KeyFrame[1].giroCaballoCabeza = 11.0f;
	KeyFrame[1].giroCola = 0.0f;
	KeyFrame[1].giroPD1arriba = -42.0f;
	KeyFrame[1].giroPD1abajo = -31.0f;
	KeyFrame[1].giroPD2arriba = 0.0f;
	KeyFrame[1].giroPD2abajo = 0.0f;
	KeyFrame[1].giroPDbase = 0.0f;
	KeyFrame[1].giroPT2arriba = 0.0f;
	KeyFrame[1].giroPT2abajo = 0.0f;
	KeyFrame[1].giroPT1arriba = -17.0f;
	KeyFrame[1].giroPT1abajo = 20.0f;

	KeyFrame[1].bf_Y = 0.0f;
	KeyFrame[1].bf_Z = 0.0f;
	KeyFrame[1].gCabeza = -33.0f;
	KeyFrame[1].gP1_PataA = 0.0f;
	KeyFrame[1].gP2_PataA = 0.0f;
	KeyFrame[1].gP3_PataA = 0.0f;
	KeyFrame[1].gP1_PataBC = 0.0f;
	KeyFrame[1].gP2_PataBC = 0.0f;
	KeyFrame[1].gP3_PataBC = 0.0f;
	KeyFrame[1].gPinzas = 0.0f;
	KeyFrame[1].gOjo = 0.0f;

	KeyFrame[2].posX = 1.0f;
	KeyFrame[2].posY = 2.0f;
	KeyFrame[2].posZ = 2.0f;
	KeyFrame[2].giroMonito = -45.0f;

	KeyFrame[2].giroCaballoTorso = 0.0f;
	KeyFrame[2].giroCaballoCabeza = 11.0f;
	KeyFrame[2].giroCola = -29.0f;
	KeyFrame[2].giroPD1arriba = 23.0f;
	KeyFrame[2].giroPD1abajo = 4.0f;
	KeyFrame[2].giroPD2arriba = 0.0f;
	KeyFrame[2].giroPD2abajo = 0.0f;
	KeyFrame[2].giroPDbase = 0.0f;
	KeyFrame[2].giroPT2arriba = 0.0f;
	KeyFrame[2].giroPT2abajo = 0.0f;
	KeyFrame[2].giroPT1arriba = -17.0f;
	KeyFrame[2].giroPT1abajo = 20.0f;

	KeyFrame[2].bf_Y = 0.0f;
	KeyFrame[2].bf_Z = 0.0f;
	KeyFrame[2].gCabeza = -60.0f;
	KeyFrame[2].gP1_PataA = 0.0f;
	KeyFrame[2].gP2_PataA = 0.0f;
	KeyFrame[2].gP3_PataA = 0.0f;
	KeyFrame[2].gP1_PataBC = 0.0f;
	KeyFrame[2].gP2_PataBC = 0.0f;
	KeyFrame[2].gP3_PataBC = 0.0f;
	KeyFrame[2].gPinzas = 0.0f;
	KeyFrame[2].gOjo = 0.0f;

	KeyFrame[3].posX = 2.0f;
	KeyFrame[3].posY = 3.0f;
	KeyFrame[3].posZ = 3.0f;
	KeyFrame[3].giroMonito = -45.0f;

	KeyFrame[3].giroCaballoTorso = 0.0f;
	KeyFrame[3].giroCaballoCabeza = 11.0f;
	KeyFrame[3].giroCola = -29.0f;
	KeyFrame[3].giroPD1arriba = -19.0f;
	KeyFrame[3].giroPD1abajo = -31.0f;
	KeyFrame[3].giroPD2arriba = 0.0f;
	KeyFrame[3].giroPD2abajo = 0.0f;
	KeyFrame[3].giroPDbase = 0.0f;
	KeyFrame[3].giroPT2arriba = 0.0f;
	KeyFrame[3].giroPT2abajo = 0.0f;
	KeyFrame[3].giroPT1arriba = -17.0f;
	KeyFrame[3].giroPT1abajo = 20.0f;

	KeyFrame[3].bf_Y = 0.0f;
	KeyFrame[3].bf_Z = 0.0f;
	KeyFrame[3].gCabeza = -43.0f;
	KeyFrame[3].gP1_PataA = 0.0f;
	KeyFrame[3].gP2_PataA = 0.0f;
	KeyFrame[3].gP3_PataA = 0.0f;
	KeyFrame[3].gP1_PataBC = 0.0f;
	KeyFrame[3].gP2_PataBC = 0.0f;
	KeyFrame[3].gP3_PataBC = 0.0f;
	KeyFrame[3].gPinzas = 0.0f;
	KeyFrame[3].gOjo = 0.0f;

	KeyFrame[4].posX = 3.0f;
	KeyFrame[4].posY = 4.0f;
	KeyFrame[4].posZ = 4.0f;
	KeyFrame[4].giroMonito = -45.0f;

	KeyFrame[4].giroCaballoTorso = 0.0f;
	KeyFrame[4].giroCaballoCabeza = 11.0f;
	KeyFrame[4].giroCola = -29.0f;
	KeyFrame[4].giroPD1arriba = 34.0f;
	KeyFrame[4].giroPD1abajo = -20.0f;
	KeyFrame[4].giroPD2arriba = 0.0f;
	KeyFrame[4].giroPD2abajo = 0.0f;
	KeyFrame[4].giroPDbase = 0.0f;
	KeyFrame[4].giroPT2arriba = 0.0f;
	KeyFrame[4].giroPT2abajo = 0.0f;
	KeyFrame[4].giroPT1arriba = -17.0f;
	KeyFrame[4].giroPT1abajo = 20.0f;

	KeyFrame[4].bf_Y = 0.0f;
	KeyFrame[4].bf_Z = 0.0f;
	KeyFrame[4].gCabeza = -5.0f;
	KeyFrame[4].gP1_PataA = 0.0f;
	KeyFrame[4].gP2_PataA = 0.0f;
	KeyFrame[4].gP3_PataA = 0.0f;
	KeyFrame[4].gP1_PataBC = 0.0f;
	KeyFrame[4].gP2_PataBC = 0.0f;
	KeyFrame[4].gP3_PataBC = 0.0f;
	KeyFrame[4].gPinzas = 0.0f;
	KeyFrame[4].gOjo = 0.0f;

	KeyFrame[5].posX = 4.0f;
	KeyFrame[5].posY = 3.0f;
	KeyFrame[5].posZ = 5.0f;
	KeyFrame[5].giroMonito = -45.0f;

	KeyFrame[5].giroCaballoTorso = 0.0f;
	KeyFrame[5].giroCaballoCabeza = 11.0f;
	KeyFrame[5].giroCola = -29.0f;
	KeyFrame[5].giroPD1arriba = -40.0f;
	KeyFrame[5].giroPD1abajo = -25.0f;
	KeyFrame[5].giroPD2arriba = 0.0f;
	KeyFrame[5].giroPD2abajo = 0.0f;
	KeyFrame[5].giroPDbase = 0.0f;
	KeyFrame[5].giroPT2arriba = 0.0f;
	KeyFrame[5].giroPT2abajo = 0.0f;
	KeyFrame[5].giroPT1arriba = -17.0f;
	KeyFrame[5].giroPT1abajo = 20.0f;

	KeyFrame[5].bf_Y = 0.0f;
	KeyFrame[5].bf_Z = 0.0f;
	KeyFrame[5].gCabeza = 28.0f;
	KeyFrame[5].gP1_PataA = 0.0f;
	KeyFrame[5].gP2_PataA = 0.0f;
	KeyFrame[5].gP3_PataA = 0.0f;
	KeyFrame[5].gP1_PataBC = 0.0f;
	KeyFrame[5].gP2_PataBC = 0.0f;
	KeyFrame[5].gP3_PataBC = 0.0f;
	KeyFrame[5].gPinzas = 0.0f;
	KeyFrame[5].gOjo = 0.0f;

	KeyFrame[6].posX = 5.0f;
	KeyFrame[6].posY = 2.0f;
	KeyFrame[6].posZ = 6.0f;
	KeyFrame[6].giroMonito = -45.0f;

	KeyFrame[6].giroCaballoTorso = 0.0f;
	KeyFrame[6].giroCaballoCabeza = 11.0f;
	KeyFrame[6].giroCola = 0.0f;
	KeyFrame[6].giroPD1arriba = 11.0f;
	KeyFrame[6].giroPD1abajo = -10.0f;
	KeyFrame[6].giroPD2arriba = 0.0f;
	KeyFrame[6].giroPD2abajo = 0.0f;
	KeyFrame[6].giroPDbase = 0.0f;
	KeyFrame[6].giroPT2arriba = 0.0f;
	KeyFrame[6].giroPT2abajo = 0.0f;
	KeyFrame[6].giroPT1arriba = -17.0f;
	KeyFrame[6].giroPT1abajo = 20.0f;

	KeyFrame[6].bf_Y = 0.0f;
	KeyFrame[6].bf_Z = 0.0f;
	KeyFrame[6].gCabeza = 58.0f;
	KeyFrame[6].gP1_PataA = 0.0f;
	KeyFrame[6].gP2_PataA = 0.0f;
	KeyFrame[6].gP3_PataA = 0.0f;
	KeyFrame[6].gP1_PataBC = 0.0f;
	KeyFrame[6].gP2_PataBC = 0.0f;
	KeyFrame[6].gP3_PataBC = 0.0f;
	KeyFrame[6].gPinzas = 0.0f;
	KeyFrame[6].gOjo = 0.0f;

	KeyFrame[7].posX = 7.0f;
	KeyFrame[7].posY = 0.0f;
	KeyFrame[7].posZ = 8.0f;
	KeyFrame[7].giroMonito = -45.0f;

	KeyFrame[7].giroCaballoTorso = 0.0f;
	KeyFrame[7].giroCaballoCabeza = -7.0f;
	KeyFrame[7].giroCola = 4.0f;
	KeyFrame[7].giroPD1arriba = 11.0f;
	KeyFrame[7].giroPD1abajo = -10.0001f;
	KeyFrame[7].giroPD2arriba = 0.0f;
	KeyFrame[7].giroPD2abajo = 0.0f;
	KeyFrame[7].giroPDbase = 0.0f;
	KeyFrame[7].giroPT2arriba = 0.0f;
	KeyFrame[7].giroPT2abajo = 0.0f;
	KeyFrame[7].giroPT1arriba = -17.0f;
	KeyFrame[7].giroPT1abajo = 20.0f;

	KeyFrame[7].bf_Y = 0.0f;
	KeyFrame[7].bf_Z = 0.0f;
	KeyFrame[7].gCabeza = 28.0f;
	KeyFrame[7].gP1_PataA = 0.0f;
	KeyFrame[7].gP2_PataA = 0.0f;
	KeyFrame[7].gP3_PataA = 0.0f;
	KeyFrame[7].gP1_PataBC = 0.0f;
	KeyFrame[7].gP2_PataBC = 0.0f;
	KeyFrame[7].gP3_PataBC = 0.0f;
	KeyFrame[7].gPinzas = 0.0f;
	KeyFrame[7].gOjo = 0.0f;

	KeyFrame[8].posX = 7.0f;
	KeyFrame[8].posY = 1.0f;
	KeyFrame[8].posZ = 8.0f;
	KeyFrame[8].giroMonito = -45.0f;

	KeyFrame[8].giroCaballoTorso = -39.0f;
	KeyFrame[8].giroCaballoCabeza = -7.0f;
	KeyFrame[8].giroCola = -12.0f;
	KeyFrame[8].giroPD1arriba = -76.0f;
	KeyFrame[8].giroPD1abajo = -10.0001f;
	KeyFrame[8].giroPD2arriba = -72.0f;
	KeyFrame[8].giroPD2abajo = 0.0f;
	KeyFrame[8].giroPDbase = 0.0f;
	KeyFrame[8].giroPT2arriba = 42.0f;
	KeyFrame[8].giroPT2abajo = 0.0f;
	KeyFrame[8].giroPT1arriba = 32.0f;
	KeyFrame[8].giroPT1abajo = 20.0f;

	KeyFrame[8].bf_Y = 0.0f;
	KeyFrame[8].bf_Z = 0.0f;
	KeyFrame[8].gCabeza = -5.0f;
	KeyFrame[8].gP1_PataA = 0.0f;
	KeyFrame[8].gP2_PataA = 0.0f;
	KeyFrame[8].gP3_PataA = 0.0f;
	KeyFrame[8].gP1_PataBC = 0.0f;
	KeyFrame[8].gP2_PataBC = 0.0f;
	KeyFrame[8].gP3_PataBC = 0.0f;
	KeyFrame[8].gPinzas = 0.0f;
	KeyFrame[8].gOjo = 0.0f;

	KeyFrame[9].posX = 8.0f;
	KeyFrame[9].posY = 2.0f;
	KeyFrame[9].posZ = 9.0f;
	KeyFrame[9].giroMonito = -45.0f;

	KeyFrame[9].giroCaballoTorso = -39.0f;
	KeyFrame[9].giroCaballoCabeza = -7.0f;
	KeyFrame[9].giroCola = -12.0f;
	KeyFrame[9].giroPD1arriba = -86.0f;
	KeyFrame[9].giroPD1abajo = 38.9999f;
	KeyFrame[9].giroPD2arriba = -32.0f;
	KeyFrame[9].giroPD2abajo = 58.0f;
	KeyFrame[9].giroPDbase = 25.0f;
	KeyFrame[9].giroPT2arriba = 42.0f;
	KeyFrame[9].giroPT2abajo = 0.0f;
	KeyFrame[9].giroPT1arriba = 32.0f;
	KeyFrame[9].giroPT1abajo = 20.0f;

	KeyFrame[9].bf_Y = 0.0f;
	KeyFrame[9].bf_Z = 1.0f;
	KeyFrame[9].gCabeza = -5.0f;
	KeyFrame[9].gP1_PataA = -15.0f;
	KeyFrame[9].gP2_PataA = 0.0f;
	KeyFrame[9].gP3_PataA = 0.0f;
	KeyFrame[9].gP1_PataBC = 0.0f;
	KeyFrame[9].gP2_PataBC = 0.0f;
	KeyFrame[9].gP3_PataBC = -6.0f;
	KeyFrame[9].gPinzas = -4.0f;
	KeyFrame[9].gOjo = 0.0f;

	KeyFrame[10].posX = 9.0f;
	KeyFrame[10].posY = 3.0f;
	KeyFrame[10].posZ = 10.0f;
	KeyFrame[10].giroMonito = -45.0f;

	KeyFrame[10].giroCaballoTorso = -39.0f;
	KeyFrame[10].giroCaballoCabeza = -7.0f;
	KeyFrame[10].giroCola = -12.0f;
	KeyFrame[10].giroPD1arriba = -41.0f;
	KeyFrame[10].giroPD1abajo = 38.9999f;
	KeyFrame[10].giroPD2arriba = -93.0f;
	KeyFrame[10].giroPD2abajo = 25.0f;
	KeyFrame[10].giroPDbase = 25.0f;
	KeyFrame[10].giroPT2arriba = 42.0f;
	KeyFrame[10].giroPT2abajo = 0.0f;
	KeyFrame[10].giroPT1arriba = 32.0f;
	KeyFrame[10].giroPT1abajo = 20.0f;

	KeyFrame[10].bf_Y = 0.0f;
	KeyFrame[10].bf_Z = 1.0f;
	KeyFrame[10].gCabeza = -5.0f;
	KeyFrame[10].gP1_PataA = 0.0f;
	KeyFrame[10].gP2_PataA = 12.0f;
	KeyFrame[10].gP3_PataA = 6.0f;
	KeyFrame[10].gP1_PataBC = 0.0f;
	KeyFrame[10].gP2_PataBC = 0.0f;
	KeyFrame[10].gP3_PataBC = -6.0f;
	KeyFrame[10].gPinzas = -4.0f;
	KeyFrame[10].gOjo = 0.0f;

	KeyFrame[11].posX = 10.0f;
	KeyFrame[11].posY = 4.0f;
	KeyFrame[11].posZ = 11.0f;
	KeyFrame[11].giroMonito = -45.0f;

	KeyFrame[11].giroCaballoTorso = -39.0f;
	KeyFrame[11].giroCaballoCabeza = -1.99999f;
	KeyFrame[11].giroCola = 30.0f;
	KeyFrame[11].giroPD1arriba = -78.9999f;
	KeyFrame[11].giroPD1abajo = 7.99987f;
	KeyFrame[11].giroPD2arriba = -36.0001f;
	KeyFrame[11].giroPD2abajo = 25.0001f;
	KeyFrame[11].giroPDbase = 11.0f;
	KeyFrame[11].giroPT2arriba = 36.0f;
	KeyFrame[11].giroPT2abajo = 0.0f;
	KeyFrame[11].giroPT1arriba = 40.0f;
	KeyFrame[11].giroPT1abajo = 20.0f;

	KeyFrame[11].bf_Y = 0.0f;
	KeyFrame[11].bf_Z = 2.0f;
	KeyFrame[11].gCabeza = -4.99999f;
	KeyFrame[11].gP1_PataA = -12.0f;
	KeyFrame[11].gP2_PataA = 0.00000476837f;
	KeyFrame[11].gP3_PataA = 0.00000238419f;
	KeyFrame[11].gP1_PataBC = 0.0f;
	KeyFrame[11].gP2_PataBC = 0.0f;
	KeyFrame[11].gP3_PataBC = -6.0f;
	KeyFrame[11].gPinzas = -9.0f;
	KeyFrame[11].gOjo = 0.0f;

	KeyFrame[12].posX = 11.0f;
	KeyFrame[12].posY = 3.0f;
	KeyFrame[12].posZ = 12.0f;
	KeyFrame[12].giroMonito = -45.0f;

	KeyFrame[12].giroCaballoTorso = -2.0f;
	KeyFrame[12].giroCaballoCabeza = -9.99997f;
	KeyFrame[12].giroCola = -1.99999f;
	KeyFrame[12].giroPD1arriba = -3.99982f;
	KeyFrame[12].giroPD1abajo = -2.00024f;
	KeyFrame[12].giroPD2arriba = -11.0005f;
	KeyFrame[12].giroPD2abajo = 2.00029f;
	KeyFrame[12].giroPDbase = 9.99995f;
	KeyFrame[12].giroPT2arriba = 9.00011f;
	KeyFrame[12].giroPT2abajo = -8.0f;
	KeyFrame[12].giroPT1arriba = 5.00011f;
	KeyFrame[12].giroPT1abajo = -3.0f;

	KeyFrame[12].bf_Y = 0.0f;
	KeyFrame[12].bf_Z = 3.0f;
	KeyFrame[12].gCabeza = -4.99999f;
	KeyFrame[12].gP1_PataA = -20.0f;
	KeyFrame[12].gP2_PataA = 12.0f;
	KeyFrame[12].gP3_PataA = 5.0f;
	KeyFrame[12].gP1_PataBC = 0.0f;
	KeyFrame[12].gP2_PataBC = 0.0f;
	KeyFrame[12].gP3_PataBC = -6.0f;
	KeyFrame[12].gPinzas = -3.0f;
	KeyFrame[12].gOjo = 0.0f;

	KeyFrame[13].posX = 12.0f;
	KeyFrame[13].posY = 2.0f;
	KeyFrame[13].posZ = 13.0f;
	KeyFrame[13].giroMonito = -45.0f;

	KeyFrame[13].giroCaballoTorso = 0.0f;
	KeyFrame[13].giroCaballoCabeza = 0.0f;
	KeyFrame[13].giroCola = 0.0f;
	KeyFrame[13].giroPD1arriba = 0.0f;
	KeyFrame[13].giroPD1abajo = 0.0f;
	KeyFrame[13].giroPD2arriba = 0.0f;
	KeyFrame[13].giroPD2abajo = 0.0f;
	KeyFrame[13].giroPDbase = 0.0f;
	KeyFrame[13].giroPT2arriba = 0.0f;
	KeyFrame[13].giroPT2abajo = 0.0f;
	KeyFrame[13].giroPT1arriba = 0.0f;
	KeyFrame[13].giroPT1abajo = 0.0f;

	KeyFrame[13].bf_Y = 0.0f;
	KeyFrame[13].bf_Z = 3.0f;
	KeyFrame[13].gCabeza = -4.99999f;
	KeyFrame[13].gP1_PataA = -1.00002f;
	KeyFrame[13].gP2_PataA = 1.00001f;
	KeyFrame[13].gP3_PataA = 2.0f;
	KeyFrame[13].gP1_PataBC = 0.0f;
	KeyFrame[13].gP2_PataBC = 0.0f;
	KeyFrame[13].gP3_PataBC = -6.0f;
	KeyFrame[13].gPinzas = -4.0f;
	KeyFrame[13].gOjo = 0.0f;

	KeyFrame[14].posX = 13.0f;
	KeyFrame[14].posY = 1.0f;
	KeyFrame[14].posZ = 14.0f;
	KeyFrame[14].giroMonito = -45.0f;

	KeyFrame[14].bf_Y = 0.0f;
	KeyFrame[14].bf_Z = 3.0f;
	KeyFrame[14].gCabeza = -4.99999f;
	KeyFrame[14].gP1_PataA = -1.00002f;
	KeyFrame[14].gP2_PataA = 1.00001f;
	KeyFrame[14].gP3_PataA = 2.0f;
	KeyFrame[14].gP1_PataBC = 0.0f;
	KeyFrame[14].gP2_PataBC = 0.0f;
	KeyFrame[14].gP3_PataBC = -6.0f;
	KeyFrame[14].gPinzas = -15.0f;
	KeyFrame[14].gOjo = 0.0f;

	KeyFrame[15].posX = 14.0f;
	KeyFrame[15].posY = 0.0f;
	KeyFrame[15].posZ = 15.0f;
	KeyFrame[15].giroMonito = -45.0f;

	KeyFrame[15].bf_Y = 0.0f;
	KeyFrame[15].bf_Z = 3.0f;
	KeyFrame[15].gCabeza = -4.99999f;
	KeyFrame[15].gP1_PataA = -1.00002f;
	KeyFrame[15].gP2_PataA = 1.00001f;
	KeyFrame[15].gP3_PataA = 2.0f;
	KeyFrame[15].gP1_PataBC = 0.0f;
	KeyFrame[15].gP2_PataBC = 0.0f;
	KeyFrame[15].gP3_PataBC = -6.0f;
	KeyFrame[15].gPinzas = -15.0f;
	KeyFrame[15].gOjo = 24.0f;

	KeyFrame[16].posX = 14.0f;
	KeyFrame[16].posY = 1.0f;
	KeyFrame[16].posZ = 15.0f;
	KeyFrame[16].giroMonito = 45.0f;

	KeyFrame[16].bf_Y = 0.0f;
	KeyFrame[16].bf_Z = 3.0f;
	KeyFrame[16].gCabeza = -4.99999f;
	KeyFrame[16].gP1_PataA = -1.00002f;
	KeyFrame[16].gP2_PataA = 1.00001f;
	KeyFrame[16].gP3_PataA = 2.0f;
	KeyFrame[16].gP1_PataBC = 0.0f;
	KeyFrame[16].gP2_PataBC = 0.0f;
	KeyFrame[16].gP3_PataBC = -6.0f;
	KeyFrame[16].gPinzas = -21.0f;
	KeyFrame[16].gOjo = 24.0f;

	KeyFrame[17].posX = 15.0f;
	KeyFrame[17].posY = 2.0f;
	KeyFrame[17].posZ = 14.0f;
	KeyFrame[17].giroMonito = 45.0f;

	KeyFrame[17].bf_Y = 0.0f;
	KeyFrame[17].bf_Z = 3.0f;
	KeyFrame[17].gCabeza = -4.99999f;
	KeyFrame[17].gP1_PataA = -1.00002f;
	KeyFrame[17].gP2_PataA = 1.00001f;
	KeyFrame[17].gP3_PataA = 2.0f;
	KeyFrame[17].gP1_PataBC = 0.0f;
	KeyFrame[17].gP2_PataBC = 0.0f;
	KeyFrame[17].gP3_PataBC = -6.0f;
	KeyFrame[17].gPinzas = -21.0f;
	KeyFrame[17].gOjo = 36.0f;

	KeyFrame[18].posX = 16.0f;
	KeyFrame[18].posY = 3.0f;
	KeyFrame[18].posZ = 13.0f;
	KeyFrame[18].giroMonito = 45.0f;

	KeyFrame[18].bf_Y = 0.0f;
	KeyFrame[18].bf_Z = 3.0f;
	KeyFrame[18].gCabeza = -4.99999f;
	KeyFrame[18].gP1_PataA = -1.00002f;
	KeyFrame[18].gP2_PataA = 1.00001f;
	KeyFrame[18].gP3_PataA = 2.0f;
	KeyFrame[18].gP1_PataBC = 0.0f;
	KeyFrame[18].gP2_PataBC = 0.0f;
	KeyFrame[18].gP3_PataBC = -6.0f;
	KeyFrame[18].gPinzas = -11.0f;
	KeyFrame[18].gOjo = 36.0f;

	KeyFrame[19].posX = 17.0f;
	KeyFrame[19].posY = 4.0f;
	KeyFrame[19].posZ = 12.0f;
	KeyFrame[19].giroMonito = 45.0f;

	KeyFrame[19].bf_Y = 0.0f;
	KeyFrame[19].bf_Z = 3.0f;
	KeyFrame[19].gCabeza = -4.99999f;
	KeyFrame[19].gP1_PataA = -1.00002f;
	KeyFrame[19].gP2_PataA = 1.00001f;
	KeyFrame[19].gP3_PataA = 2.0f;
	KeyFrame[19].gP1_PataBC = 0.0f;
	KeyFrame[19].gP2_PataBC = 0.0f;
	KeyFrame[19].gP3_PataBC = -6.0f;
	KeyFrame[19].gPinzas = -4.0f;
	KeyFrame[19].gOjo = 3.0f;

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
		staticShader.setVec3("dirLight.specular", glm::vec3(0.4f, 0.4f, 0.4f));

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
		glm::mat4 tmpCTS = glm::mat4(1.0f);
		glm::mat4 tmpCTS2 = glm::mat4(1.0f);
		glm::mat4 tmpBTS = glm::mat4(1.0f);
		glm::mat4 tmpBTS2 = glm::mat4(1.0f);

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

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje: Tiro Al Blanco
		// -------------------------------------------------------------------------------------------------------------------------

		// Caballo  torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, 5.95f, 63.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0));
		tmpCTS = model = glm::rotate(model, glm::radians(giroCaballoTorso), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		torso_caballo.Draw(staticShader);

		// Caballo  cabeza
		model = glm::translate(tmpCTS, glm::vec3(0.0f, 4.0f, 4.0f));
		model = glm::rotate(model, glm::radians(giroCaballoCabeza), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		cabeza_caballo.Draw(staticShader);

		// Caballo cola
		model = glm::translate(tmpCTS, glm::vec3(0.0f, 2.0f, -1.0f));
		model = glm::rotate(model, glm::radians(giroCola), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		cola_caballo.Draw(staticShader);

		// Caballo pata delantera 1 arriba
		model = glm::translate(tmpCTS, glm::vec3(-1.1f, 1.2f, 5.2f)); //6.2
		tmpCTS2 = model = glm::rotate(model, glm::radians(giroPD1arriba), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel1_arriba_caballo.Draw(staticShader);
		// Caballo pata delantera 1 abajo
		model = glm::translate(tmpCTS2, glm::vec3(0.0f, -2.4f, 0.3f));
		model = glm::rotate(model, glm::radians(giroPD1abajo), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel1_abajo_caballo.Draw(staticShader);
		// Caballo pata delantera 1 planta
		model = glm::translate(model, glm::vec3(0.0f, -2.4f, -0.3f));
		model = glm::rotate(model, glm::radians(giroPDbase), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel1_planta_caballo.Draw(staticShader);

		// Caballo pata delantera 2 arriba
		model = glm::translate(tmpCTS, glm::vec3(1.1f, 1.2f, 5.2f));
		tmpCTS2 = model = glm::rotate(model, glm::radians(giroPD2arriba), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel2_arriba_caballo.Draw(staticShader);
		// Caballo pata delantera 2 abajo
		model = glm::translate(tmpCTS2, glm::vec3(0.0f, -2.4f, 0.3f));
		model = glm::rotate(model, glm::radians(giroPD2abajo), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel2_abajo_caballo.Draw(staticShader);
		// Caballo pata delantera 2 planta
		model = glm::translate(model, glm::vec3(0.0f, -2.4f, -0.3f));
		model = glm::rotate(model, glm::radians(giroPDbase), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel2_planta_caballo.Draw(staticShader);

		// Caballo pata trasera 1 arriba
		model = glm::translate(tmpCTS, glm::vec3(-1.1f, 1.2f, 0.2f));
		tmpCTS2 = model = glm::rotate(model, glm::radians(giroPT1arriba), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataTras1_arriba_caballo.Draw(staticShader);
		// Caballo pata trasera 1 abajo
		model = glm::translate(tmpCTS2, glm::vec3(0.0f, -2.4f, 0.3f));
		model = glm::rotate(model, glm::radians(giroPT1abajo), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataTras1_abajo_caballo.Draw(staticShader);
		// Caballo pata trasera 1 planta
		model = glm::translate(model, glm::vec3(0.0f, -2.4f, -0.3f));
		staticShader.setMat4("model", model);
		pataTras1_planta_caballo.Draw(staticShader);

		// Caballo pata trasera 2 arriba
		model = glm::translate(tmpCTS, glm::vec3(1.1f, 1.2f, 0.2f));
		tmpCTS2 = model = glm::rotate(model, glm::radians(giroPT2arriba), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel2_arriba_caballo.Draw(staticShader);
		// Caballo pata trasera 2 abajo
		model = glm::translate(tmpCTS2, glm::vec3(0.0f, -2.4f, 0.3f));
		model = glm::rotate(model, glm::radians(giroPT2abajo), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		pataDel2_abajo_caballo.Draw(staticShader);
		// Caballo pata trasera 2 planta
		model = glm::translate(model, glm::vec3(0.0f, -2.4f, -0.3f));
		staticShader.setMat4("model", model);
		pataDel2_planta_caballo.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje: BabyFace
		// -------------------------------------------------------------------------------------------------------------------------

		// BabyFace Alfombra
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0f, 0.8f, 48.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		alfombra_babyface.Draw(staticShader);

		// BabyFace base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0f, 2.19f, 48.0f));
		model = glm::translate(model, glm::vec3(bf_Z, bf_Y, 0.0f));
		tmpBTS = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cuerpo_babyface.Draw(staticShader);

		// BabyFace cabeza
		model = glm::translate(tmpBTS, glm::vec3(0.0f, 0.64f, 0.0f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(gCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cabeza_babyface.Draw(staticShader);

		// BabyFace ojo
		model = glm::translate(tmpBTS2, glm::vec3(0.5376f, 1.3868f, 0.985f));
		model = glm::rotate(model, glm::radians(gOjo), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		ojo_babyface.Draw(staticShader);

		// BabyFace Par 1 de Patas A, pata delantera
		model = glm::translate(tmpBTS, glm::vec3(-1.28f, 0.0f, 1.28f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(gP1_PataA), glm::vec3(1.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		par1_PataA_Del.Draw(staticShader);
		// BabyFace Par 1 de Patas BC, pata delantera
		model = glm::translate(tmpBTS2, glm::vec3(-0.85f, 0.833f, 0.973f)); //LISTO
		model = glm::rotate(model, glm::radians(gP1_PataBC), glm::vec3(1.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		par1_PataBC_Del.Draw(staticShader);

		// BabyFace Par 1 de Patas A, pata trasera
		model = glm::translate(tmpBTS, glm::vec3(1.28f, 0.0f, -1.28f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(gP1_PataA), glm::vec3(-1.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		par1_PataA_Tras.Draw(staticShader);
		// BabyFace Par 1 de Patas BC, pata trasera
		model = glm::translate(tmpBTS2, glm::vec3(1.145f, 1.548f, -1.08f)); //LISTO
		model = glm::rotate(model, glm::radians(gP1_PataBC), glm::vec3(-1.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		par1_PataBC_Tras.Draw(staticShader);

		// BabyFace Par 2 de Patas A, pata delantera
		model = glm::translate(tmpBTS, glm::vec3(1.28f, 0.0f, 1.28f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(gP2_PataA), glm::vec3(-1.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		par2_PataA_Del.Draw(staticShader);
		// BabyFace Par 2 de Patas BC, pata delantera
		model = glm::translate(tmpBTS2, glm::vec3(0.893f, 0.831f, 0.959f)); //LISTO
		model = glm::rotate(model, glm::radians(gP2_PataBC), glm::vec3(-1.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		par2_PataBC_Del.Draw(staticShader);

		// BabyFace Par 2 de Patas A, pata trasera
		model = glm::translate(tmpBTS, glm::vec3(-1.28f, 0.0f, -1.28f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(gP2_PataA), glm::vec3(1.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		par2_PataA_Tras.Draw(staticShader);
		// BabyFace Par 2 de Patas BC, pata trasera
		model = glm::translate(tmpBTS2, glm::vec3(-1.154f, 1.569f, -1.08f)); //LISTO
		model = glm::rotate(model, glm::radians(gP2_PataBC), glm::vec3(1.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		par2_PataBC_Tras.Draw(staticShader);

		// BabyFace Par 3 de Patas A, pata izquierda
		model = glm::translate(tmpBTS, glm::vec3(-1.5f, -0.03f, 0.0f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(gP3_PataA), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		par3_PataA_Izq.Draw(staticShader);
		// BabyFace Par 3 de Patas BC, pata izquierda
		model = glm::translate(tmpBTS2, glm::vec3(-1.542f, 1.554f, 0.0f)); //LISTO
		model = glm::rotate(model, glm::radians(gP3_PataBC), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		par3_PataBC_Izq.Draw(staticShader);

		// BabyFace Par 3 de Patas A, pata derecha
		model = glm::translate(tmpBTS, glm::vec3(1.5f, -0.03f, 0.0f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(gP3_PataA), glm::vec3(0.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		par3_PataA_Der.Draw(staticShader);
		// BabyFace Par 3 de Patas BC, pata derecha
		model = glm::translate(tmpBTS2, glm::vec3(1.583f, 1.569f, -0.0f)); //LISTO
		model = glm::rotate(model, glm::radians(gP3_PataBC), glm::vec3(0.0f, 0.0f, -1.0f));
		staticShader.setMat4("model", model);
		par3_PataBC_Der.Draw(staticShader);

		// BabyFace Brazo izquierdo
		model = glm::translate(tmpBTS, glm::vec3(-1.117f, -0.436f, 2.845f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazo_Izq.Draw(staticShader);
		// BabyFace Pinza Izq Superior
		model = glm::translate(tmpBTS2, glm::vec3(0.024f, 1.57f, 0.151f));
		model = glm::rotate(model, glm::radians(gPinzas), glm::vec3(0.25f, 0.0f, 0.1f));
		staticShader.setMat4("model", model);
		pinza_Izq_Sup.Draw(staticShader);
		// BabyFace Pinza Izq Inferior
		model = glm::translate(tmpBTS2, glm::vec3(0.043f, 1.557f, 0.123f));
		model = glm::rotate(model, glm::radians(gPinzas), glm::vec3(-0.25, 0.0f, -0.1f));
		staticShader.setMat4("model", model);
		pinza_Izq_Inf.Draw(staticShader);

		// BabyFace Brazo derecho
		model = glm::translate(tmpBTS, glm::vec3(1.123f, -0.427f, 2.847f));
		tmpBTS2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazo_Der.Draw(staticShader);
		// BabyFace Pinza Der Superior
		model = glm::translate(tmpBTS2, glm::vec3(-0.389f, 1.394f, -0.7f));
		model = glm::rotate(model, glm::radians(gPinzas), glm::vec3(0.25f, 0.0f, -0.1f));
		staticShader.setMat4("model", model);
		pinza_Der_Sup.Draw(staticShader);
		// BabyFace Pinza Der Inferior
		model = glm::translate(tmpBTS2, glm::vec3(-0.369f, 1.365f, -0.687f));
		model = glm::rotate(model, glm::radians(gPinzas), glm::vec3(-0.25f, 0.0f, 0.1f));
		staticShader.setMat4("model", model);
		pinza_Der_Inf.Draw(staticShader);


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
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(0.7f));
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