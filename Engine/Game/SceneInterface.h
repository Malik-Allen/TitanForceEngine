#ifndef SCENEINTERFACE_H
#define SCENEINTERFACE_H
class SceneInterface {
public:
	SceneInterface() {}
	virtual ~SceneInterface() {}

	virtual bool OnCreate() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() = 0;
};
#endif // !SCENEINTERFACE_H
