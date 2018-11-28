#ifndef SCENE_H
#define SCENE_H

class Scene {
public:
	//Vec3 worldGravity;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float time) = 0;
	virtual void Render() = 0;
};

#endif