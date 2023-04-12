////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 125 graphics engine";

// Objects to render
Skeleton* Window::skeleton;
Skin* Window::skin;
AnimationClip* Window::animClip;
AnimationPlayer* Window::anim;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;


////////////////////////////////////////////////////////////////////////////////

std::string skelfile = "";
std::string skinfile = "";
std::string animfile = "";
bool showSkelMode = true;
bool wireframeMode = false;
bool cullingMode = false;


void imguiDraw(Skeleton* sk, AnimationClip* animClip) {

	// model window
	ImGui::SetNextWindowSize(ImVec2(240, 420), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::Begin("Model");

	// File info
	ImGui::BeginGroup();
	ImGui::InputText("Skel path", &skelfile);
	ImGui::InputText("Skin path", &skinfile);
	ImGui::InputText("Anim path", &animfile);

	if (ImGui::Button("Load")) {
		Window::cleanObjects();
		Window::initializeObjects();
		ImGui::EndGroup();
		ImGui::End();
		return;
	}
	ImGui::EndGroup();

	ImGui::Separator();

	ImGui::Checkbox("Skeleton", &showSkelMode);
	ImGui::Checkbox("Wire Mode", &wireframeMode);
	if (ImGui::Checkbox("Cull faces", &cullingMode)) {
		// Enable backface culling (fixes z-fighting)
		if (cullingMode) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	if (sk) sk->Show();	// improve this later

	ImGui::End();

	// anim window
	if (animClip) {
		ImGui::SetNextWindowSize(ImVec2(240, 100), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(260, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowBgAlpha(0.1f);	//TwDefine(" Animation color='150 20 0' ");
		ImGui::Begin("Animation");

		if (ImGui::Button("Play")) {
			Window::resetPlayback();
		}

		if (animClip) animClip->Show();

		ImGui::End();
	}
}

// Constructors and desconstructors 
bool Window::initializeProgram(GLFWwindow* window) {

	// Setup imgui --
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	// --


	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Load in skeleton
	skeleton = new Skeleton();
	if (!skeleton->Load(skelfile.c_str())) {
		delete skeleton;
		skeleton = nullptr;
	}

	// Load in skin
	skin = new Skin();
	if (!skin->Load(skinfile.c_str())) {
		delete skin;
		skin = nullptr;
	}

	// Load in anim
	bool hasAnim = true;
	anim = new AnimationPlayer();

	animClip = new AnimationClip();
	if (!animClip->Load(animfile.c_str())) {
		delete animClip;
		animClip = nullptr;

		hasAnim = false;
	}
	else anim->SetClip(animClip);

	return true;
}

void Window::resetPlayback() {
	if (anim) anim->Play();
}

bool fileIsType(std::string n, std::string type = ".txt") {
	return n.find(type) != std::string::npos;
}

void Window::setInputFiles(const std::vector<std::string> filepaths) {
	// Set file vars
	for(std::string f : filepaths) {
		if (fileIsType(f, ".skel")) skelfile = f;
		else if (fileIsType(f, ".skin")) skinfile = f;
		else if (fileIsType(f, ".anim")) animfile = f;
	}
}

void Window::cleanObjects() {
	// Deallcoate the objects.
	if (skeleton) delete skeleton;
	if (skin) delete skin;

	skeleton = nullptr;
	skin = nullptr;
	

	delete anim;
	if (animClip) delete animClip;
	animClip = nullptr;
}

void Window::cleanUp()
{
	cleanObjects();

	// stop imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// imgui setup in initializeProgram()

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));

	// ImGui::WindowSize(w, h)???
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback(float deltaTime)
{
	// Perform any updates as necessary. 
	Cam->Update();

	if (anim && animClip) {
		anim->Update(deltaTime);

		if (skeleton) skeleton->SetPose(anim->GetCurrentPose());
	}

	if (skeleton) skeleton->Update();

	if (skin) skin->Update(skeleton);
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();


	// Clear the color and depth buffers.					******
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the objects.
	glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
	if(skeleton && showSkelMode) skeleton->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(skin) skin->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

	// imgui new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	imguiDraw(skeleton, animClip);	// simple helper method

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Swap buffers.										******
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return;

	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_R:
			resetCamera();
			if (anim) anim->Play();
			break;

		default:
			break;
		}
	}
}

void Window::charCallback(GLFWwindow* window, unsigned int codepoint)
{
	// ImGui::WantCaptureChar???
	//if (TwEventCharGLFW(codepoint, GLFW_PRESS)) return;


}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse) {
		LeftDown = RightDown = false;
		return;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (ImGui::GetIO().WantCaptureMouse) return;

	if (yoffset) {
		const float rate = 0.05f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - (float)yoffset * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	if (ImGui::GetIO().WantCaptureMouse) {
		LeftDown = RightDown = false;
		return;
	}

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (RightDown || LeftDown) {
		const float rate = 0.5f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	// Zoom moved to scroll wheel
}

////////////////////////////////////////////////////////////////////////////////