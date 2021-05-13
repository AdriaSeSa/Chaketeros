#ifndef _SCENESELECTAREA_H_
#define _SCENESELECTAREA_H_

#include "Scene.h"
#include "External/SDL_image/include/SDL_image.h"
#include "Animation.h"

class SceneSelectArea : public Scene
{
private:
	//Textures
	SDL_Texture* texSelectArea = nullptr;
	SDL_Texture* texLevels = nullptr;
	SDL_Texture* texMainMenu = nullptr;

	//	Textures Rectangles
	SDL_Rect SelectStageBackgroundRect;
	SDL_Rect UIStageLevel1Rect;
	SDL_Rect UIStageLevel2Rect;
	SDL_Rect StageCheeseandStarsRect;
	// Animation
	Animation texLevel1CheeseAnim, texLevel2CheeseAnim, texLevel3CheeseAnim, texLevel4CheeseAnim, texLevel5CheeseAnim;
	// Arrow Position and current Arrow
	iPoint arrowLevelPosition[5];
	iPoint* currentArrowLevelPos = nullptr;

	uint changeSelectSFX, selectSFX;
public:
	
	SceneSelectArea();

	// Destructor
	~SceneSelectArea();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp(bool finalCleanUp = true) override;
};


#endif // !_SCENESELECTAREA_H_


