#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "GameCharacter.h"
#include "Map.h"


class Action
{
public:
	Action(sf::Vector2<float> target) : target(target) {};

	virtual ~Action() = default;

	virtual void update(Map &map,std::weak_ptr<GameCharacter> actor, const float &dt) = 0;

	virtual bool isAnimationEnded(std::weak_ptr<GameCharacter> actor) = 0;

protected:

	const sf::Vector2<float> target;

};

class Move : public Action
{
public:

	Move(sf::Vector2<float> target) : Action(target) {};

	void update(Map &map, std::weak_ptr<GameCharacter> actor, const float &dt) override;
	inline bool isAnimationEnded(std::weak_ptr<GameCharacter> actor) override { return (Utils::Math::distance(actor.lock()->getPos(),target)<eps);};

private:

    const float speed = 100.f;
	const float eps = 3.f;

};

class Attack : public Action
{
public:

	Attack(sf::Vector2<float> target) : Action(target) {};

	void update(Map &map, std::weak_ptr<GameCharacter> actor, const float &dt) override;
	inline bool isAnimationEnded(std::weak_ptr<GameCharacter> actor) override{return true;};

};

class Interact : public Action
{
public:

	Interact(sf::Vector2<float> target) : Action(target) {};

	void update(Map &map, std::weak_ptr<GameCharacter> actor, const float &dt) override;
	inline bool isAnimationEnded(std::weak_ptr<GameCharacter> actor) override{};

};