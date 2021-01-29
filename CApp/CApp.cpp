#include "CApp/CApp.h"
#include "app.h"

using namespace std;

extern CApp app;

enum STATUS : Uint32 {
	ST_ALL_CLEARED = 0x00000000,
	ST_ALL_SETTED = 0xFFFFFFFF,
	ST_INITIED = 0x00000001,
	ST_APP_LOCKED = 0x00000002,
};


CApp::CApp() :
	m_uStatus(-1),
	m_uWindowID(-1),
	
	m_pWindow(nullptr),
	m_pRenderer(nullptr),
	
	m_nTimerID(-1),

	m_pScene(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		cerr << "SDL_Init failed with error:" << SDL_GetError() << endl;
		return;
	}

	m_pWindow = SDL_CreateWindow(
		MAIN_WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		MAIN_WINDOW_WIDTH,
		MAIN_WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (!m_pWindow) {
		SDL_Quit();
		cerr << "SDL_CreateWindow() failed with error: " << SDL_GetError() << endl;
		return;
	}

	m_uWindowID = SDL_GetWindowID(m_pWindow);

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

	if (m_pRenderer == nullptr) {
		cerr << "Failed to create accelerated renderer." << endl;
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);
		if (m_pRenderer == nullptr) {
			cerr << "Failed to create software renderer.\n" << endl;
			return;
		}
	}

	CScene::Init(m_pRenderer, new SDL_Rect{PADDING_GAMEAREA_X, PADDING_GAMEAREA_Y, GAMEAREA_SIZE_X, GAMEAREA_SIZE_Y});

	try
	{
		m_pScene = new CScene();
	}
	catch (const string& ex)
	{
		cerr << ex << endl;
		return;
	}
	

	m_nTimerID = SDL_AddTimer(MAIN_ANIMATION_TICK, AnimateCallBack, &app);

	SDL_RenderPresent(m_pRenderer);

	m_uStatus |= ST_INITIED;

	Run();
}

CApp::~CApp()
{
	if (m_nTimerID != -1) {
		SDL_RemoveTimer(m_nTimerID);
		m_nTimerID = -1;
	}

	if(m_pScene) delete m_pScene;

	if (m_pRenderer) {
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}

	if (m_pWindow) {
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}

	SDL_Quit();

}

void CApp::Run()
{
	if (!(m_uStatus & ST_INITIED)) return;

	int quit{ 0 };
	SDL_Event event;

	do {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = 1;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	} while (quit != 1);
}

bool CApp::IsInitiated() const { return m_uStatus & ST_INITIED; }

Uint32 CApp::AnimateCallBack(Uint32 interval, void* pParam) {
	
	CApp* pApp{ reinterpret_cast<CApp*>(pParam) };
	SDL_RenderClear(pApp->m_pRenderer);

	pApp->m_pScene->Draw();

	SDL_SetRenderDrawColor(pApp->m_pRenderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(pApp->m_pRenderer, MAIN_WINDOW_WIDTH / 2, 0, MAIN_WINDOW_WIDTH / 2, MAIN_WINDOW_HEIGHT);

	SDL_SetRenderDrawColor(pApp->m_pRenderer, 0, 0, 0, 255);

	SDL_RenderPresent(pApp->m_pRenderer);
	return interval;
}