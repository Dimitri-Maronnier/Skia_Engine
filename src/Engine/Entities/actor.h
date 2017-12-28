#ifndef ACTOR_H
#define ACTOR_H
#include <QString>
#include <vector>
class Scene;

class Actor
{
public:

    virtual ~Actor() = default;
    virtual void setUp(Scene *scene) = 0;
    virtual void update(double deltaTime) = 0;

    static std::vector<Actor*> Actors;
private:
    QString _path;

};

#endif // ACTOR_H
