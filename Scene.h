#ifndef SCENE_H
#define SCENE_H
enum InputEvent {
	KEY_PRESS_DEFAULT = 0,
	KEY_PRESS_1,
	KEY_PRESS_2,
	KEY_PRESS_3,
	KEY_PRESS_4,
	KEY_PRESS_5,
	KEY_PRESS_6,
	KEY_PRESS_7,
	KEY_PRESS_8,
	KEY_PRESS_9,
	KEY_PRESS_0,
	KEY_PRESS_TOTAL
};

class Scene {
public:

	InputEvent *currentInput; //Instaniate in OnCreate!

	//Must instantiated with an SDL_Window pointer variable, that will be passed through as a overload
	//Inside OnCreate instantiate the currentInput as type InputEvent
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float time) = 0;
	virtual void Render() = 0;

	//Takes the currentInput variable address as a parameter
	//Use an if or switch case in order to check the value of currentInput against an InputEvent
	virtual void InputManagement(InputEvent* inputEvent);
};

#endif