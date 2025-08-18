#include "Game/App.hpp"
#include "Game/Game.hpp"
#include "Game/Sound.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/DX11Renderer.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Window/Window.hpp"



App*			g_theApp		= nullptr;		// Created and owned by Main_Windows.cpp
//InputSystem*	g_theInput		= nullptr;		// Created and owned by the App
Window*			g_theWindow		= nullptr;		// Created and owned by the App
Renderer*		g_theRenderer	= nullptr;		// Created and owned by the App
AudioSystem*    g_theAudio		= nullptr;		// Created and owned by the App
bool			g_isDebugDraw	= false;

App::App()
{
}

App::~App()
{
}

void App::Startup()
{
	// Create all Engine subsystems
	EventSystemConfig eventSystemConfig;
	g_theEventSystem = new EventSystem(eventSystemConfig);

	InputConfig inputConfig;
	g_theInput = new InputSystem(inputConfig);

	WindowConfig windowConfig;
	windowConfig.m_inputSystem = g_theInput;
	windowConfig.m_aspectRatio = 2.f;
	windowConfig.m_windowTitle = "Starship Gold";
	g_theWindow = new Window(windowConfig);

	RendererConfig rendererConfig;
	rendererConfig.m_window = g_theWindow;
	g_theRenderer = new DX11Renderer(rendererConfig);

	DevConsoleConfig devConsoleConfig;
	devConsoleConfig.m_defaultRenderer = g_theRenderer;
	g_theDevConsole = new DevConsole(devConsoleConfig);

	AudioConfig audioConfig;
	g_theAudio = new AudioSystem(audioConfig);

	// Start up all Engine subsystems
	g_theEventSystem->Startup();
	g_theDevConsole->Startup();
	g_theInput->Startup();
	g_theWindow->Startup();
	g_theRenderer->Startup();
	g_theAudio->Startup();

	// Initialize game-related stuff: create and start the game
	Sound::Init(); // load music
	m_timeLastFrameStart = GetCurrentTimeSeconds();
	m_theGame = new Game();
	g_theEventSystem->SubscribeEventCallbackFunction("Quit", OnQuitEvent);

	g_theDevConsole->AddText(DevConsole::INFO_MINOR, R"(
How to control
### Player Ship Controls
- Movement:
  - Left JoyStick: control direction and thrust	
  - S: Turn Left
  - F: Turn Right
  - E: Thrust Forward
- Shooting:
  - Space [XBOX A Button]: Fire Bullet
- Respawning:
  - N: Respawn Player Ship

### Debugging
- F1: Toggle Debug Drawing for all entities

### Asteroid Management
- I: Spawn a Random Asteroid

### Game State Management
- F8: Hard Reset the Game
- P: Toggle Pause/Unpause the Game
- T: Toggle time scale between 0.1 and 1.0
- O: Unpause the Game for one frame of Update, then Pause Again
- N/Space [Xbox Start] enter the game
- ESC [Xbox Back] leave game, and again to quit
)");

}

void App::Shutdown()
{
	// Destroy game-related stuff
	delete m_theGame;
	m_theGame = nullptr;

	// Shut down all Engine subsystems
	g_theAudio->Shutdown();
	g_theRenderer->Shutdown();
	g_theWindow->Shutdown();
	g_theInput->Shutdown();
	g_theDevConsole->Shutdown();
	g_theEventSystem->Shutdown();

	// Destroy all engine subsystems
	delete g_theAudio;
	g_theAudio = nullptr;
	delete g_theInput;
	g_theInput = nullptr;
	delete g_theRenderer;
	g_theRenderer = nullptr;
	delete g_theWindow;
	g_theWindow = nullptr;
	delete g_theInput;
	g_theInput = nullptr;
	delete g_theEventSystem;
	g_theEventSystem = nullptr;
}

void App::RunMainLoop()
{
	while (!IsQuitting())
	{
		RunFrame();
	}
}

void App::RunFrame()
{
	Clock::TickSystemClock();

	BeginFrame();			// Engine pre-frame stuff
	Update();	// Game updates / moves/ spawns / hurts/ kills stuffs
	Render();				// Game draws current state of things
	EndFrame();				// Engine post-frame stuff
}

void App::HandleQuitRequested()
{
	m_isQuitting = true;
}

void App::BeginFrame()
{
	g_theEventSystem->BeginFrame();
    g_theDevConsole->BeginFrame();
	g_theInput->BeginFrame();
	g_theWindow->BeginFrame();
	g_theRenderer->BeginFrame();
	g_theAudio->BeginFrame();
}

void App::Update()
{
	if (m_theGame->ShouldGameReset() || g_theInput->WasKeyJustPressed(KEYCODE_F8))
	{
		delete m_theGame;
		m_theGame = new Game();
	}

	m_theGame->Update();
}

void App::Render() const
{
	g_theRenderer->ClearScreen(Rgba8(0, 0, 0));
	m_theGame->Render();


	// Render DevConsole
	Camera devConsoleCamera;
	AABB2 bounds(0.f, 0.f, SCREEN_SIZE_X, SCREEN_SIZE_Y);
	devConsoleCamera.SetOrthoView(bounds.m_mins, bounds.m_maxs);
	g_theRenderer->BeginCamera(devConsoleCamera);
	g_theDevConsole->Render(bounds);
	g_theRenderer->EndCamera(devConsoleCamera);
}

void App::EndFrame()
{
	g_theAudio->EndFrame();
	g_theRenderer->EndFrame();
	g_theWindow->EndFrame();
	g_theInput->EndFrame();
	g_theDevConsole->EndFrame();
	g_theEventSystem->EndFrame();
}

bool OnQuitEvent(EventArgs& args)
{
	UNUSED(args);
	g_theApp->HandleQuitRequested();
	return false;
}