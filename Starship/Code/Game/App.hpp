#pragma once
#include "Engine/Core/EventSystem.hpp"

//-----------------------------------------------------------------------------------------------
class Game;

//-----------------------------------------------------------------------------------------------
class App
{
public:
    App();
    ~App();
    void Startup();
    void Shutdown();
    void RunMainLoop();
    void RunFrame();

    void HandleQuitRequested();
    bool IsQuitting() const { return m_isQuitting; }

private:
    void BeginFrame();
    void Update();
    void Render() const;
    void EndFrame();
    
private:
    double m_timeLastFrameStart = 0.0f;
    bool m_isQuitting = false;

public:
	Game* m_theGame = nullptr; // for time scale event

};

bool OnQuitEvent(EventArgs& args);
