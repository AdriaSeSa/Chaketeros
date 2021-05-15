#include "ModuleScene.h"

PlayerSettings* playerSettings = nullptr;

ModuleScene::ModuleScene()
{
	scenes[0] = new SceneIntro();
	scenes[1] = new SceneMainTitle();
	scenes[2] = new SceneSelectArea();
	scenes[3] = new SceneSelectStage();
	scenes[4] = new SceneLevel1();
	scenes[5] = new SceneLevel2();
	scenes[6] = new SceneLevelBoss();
	scenes[7] = new SceneGameOver();

	playerSettings = PlayerSettings::Instance();
}

ModuleScene::~ModuleScene()
{

}

bool ModuleScene::Start()
{
	bool ret = true;

	screenRect = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };	//Screen-size rectangle
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	currentScene = scenes[INTRO_SCENE];


	if(currentScene == nullptr)
	{
		return ret;
	}

	currentScene->Start();

	return ret;
}

UpdateResult ModuleScene::PreUpdate()
{
	if (currentScene == nullptr)
	{
		return UpdateResult::UPDATE_CONTINUE;
	}

	currentScene->PreUpdate();

	return UpdateResult::UPDATE_CONTINUE;
}

UpdateResult ModuleScene::Update()
{
	if (currentScene == nullptr)
	{
		return UpdateResult::UPDATE_CONTINUE;
	}
		
	if (currentStep == FADE_NONE) { currentScene->Update(); return UpdateResult::UPDATE_CONTINUE; }	//Si no hay FADE solo se ejecuta el UPDATE de la CURRENT SCENE

	if (currentStep == FADE_IN)		//Si hay FADE IN, se ejecuta el Update HASTA QUE EL RECTANGULO NEGRO tenga opacidad m�xima
	{
		currentFrame++;
		currentScene->Update();
		if (currentFrame >= maxFrames)	//Si el rect�ngulo negro tiene opacidad m�xima, cambiamos de escena e incializamos la siguiente
		{
			currentScene->CleanUp(false);
			currentScene = scenes[newScene];
			currentScene->score = playerSettings->playerScore;
			currentScene->Start();
			currentStep = FADE_OUT;
			return UpdateResult::UPDATE_CONTINUE;
		}
		return UpdateResult::UPDATE_CONTINUE;
	}
	else //Si no es FADE_NONE o FADE_IN deber?ser FADE_OUT
	{
		--currentFrame;		//Restamos la opacidad del rect�ngulo negro
		currentScene->Update();
		if (currentFrame <= 0) { currentStep = FADE_NONE; }	//Si la opacidad es 0, volvemos a FADE_NONE
	}

	return UpdateResult::UPDATE_CONTINUE;
}

UpdateResult ModuleScene::PostUpdate()
{
	if (currentScene == nullptr)
	{
		return UpdateResult::UPDATE_CONTINUE;
	}
	
	currentScene->PostUpdate();

	if (currentStep != FADE_NONE)
	{
		float fadeRatio = (float)currentFrame / (float)maxFrames;

		App->render->AddRectRenderQueue(screenRect, { 0,0,0,(Uint8)(fadeRatio * 255.0f) });
	}
	
	return UpdateResult::UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(Collider* c1, Collider* c2)
{
	currentScene->OnCollision(c1, c2);
}

void ModuleScene::WillCollision(Collider* c1, Collider* c2)
{
	currentScene->WillCollision(c1, c2);
}

bool ModuleScene::ChangeCurrentScene(uint index, int frames, int sceneScore)	//CleanUp current scene, change current scene (index), Start current Scene
{
	if (currentStep != FADE_NONE) return false;
	
	currentStep = FADE_IN;
	maxFrames = frames;
	currentFrame = 0;
	newScene = index;
	playerSettings->playerScore = sceneScore;
	return true;
}

bool ModuleScene::CleanUp()
{
	for (int i = 0; i < SCENES_NUM; i++)
	{
		if (scenes[i] != nullptr)
		{
			scenes[i]->CleanUp();	//CleanUp all scenes (in case the Application is shut down)
			delete scenes[i];
			scenes[i] = nullptr;
		}
	}

	playerSettings->Release();

	return true;
}
