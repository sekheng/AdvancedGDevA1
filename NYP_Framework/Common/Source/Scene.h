#ifndef SCENE_H
#define SCENE_H

#include <string>

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

    virtual bool onNotify(const std::string &zeEvent) {
        return false;
    }
};

#endif // SCENE_H