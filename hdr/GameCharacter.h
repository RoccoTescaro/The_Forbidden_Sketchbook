#pragma once

#include <queue>
#include <memory>
#include "Entity.h"

class Weapon;
class PathAlgorithm {};
class AStar : public PathAlgorithm{};
class DiglettMovement : public PathAlgorithm{};

class GameCharacter : public Entity
{
public:
    GameCharacter(uint8_t maxHealth, uint8_t health, uint8_t maxEnergy, uint8_t energy, uint8_t priority) 
        : maxHealth(maxHealth), health(health), maxEnergy(maxEnergy), energy(energy), priority(priority) {};
    GameCharacter() : energy(0), priority(0) {};

    void update(Map &map, const float &dt) override;
    void execute(GameCharacter &gameCharacter, Map &map) override;
    inline void render() override 
    {   
        Entity::render();
        //weapon->render();    
    };
    
    inline bool isSolid() const override { return true; };
    inline uint8_t getMaxHealth() const { return maxHealth; };
    inline uint8_t getMaxEnergy() const { return maxEnergy; };
    inline uint8_t getHealth() const { return health; };
    inline uint8_t getEnergy() const { return energy; };
    inline uint8_t getPriority() const { return priority; };
    inline uint8_t getRange() const { return 0; /*return weapon->getRange()*/};


    inline void setEnergy(const uint8_t newEnergy) { energy=newEnergy; };

    bool isInRange(Map &map) const;
    void updateStepQueue(Map &map, const sf::Vector2<float> target);
    //stepQueueEmpty?

    inline void roundReset()
    {   
        energy = maxEnergy;
        //stepQueue.clear();  
    }

    inline void serialize(Archive& fs) 
    {
        Entity::serialize(fs);
        fs.serialize(health);
        fs.serialize(energy);
        //fs.serialize(stepQueue);
    }
protected:
    //STATS
    const uint8_t maxHealth = 0; //#TODO make it mutable to serialization
    const uint8_t maxEnergy = 0;
    const uint8_t priority = 0;
    uint8_t health = 0;
    uint8_t energy = 0;
    //Weapon weapon; //std::unique_ptr<Weapon> weapon = nullptr;

    //MOVEMENT
    //std::unique_ptr<PathAlgorithm> movementStrategy;
    const float animationSpeed = 300.f;
    //std::deque<sf::Vector2<float>> stepQueue;
};


class Player : public GameCharacter
{
public:
    Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB);
    Player() {};

    inline const sf::Color& getFilterColor() const { return filterColor; };
    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        //fs.serialize(filterColor);
    }

private:
    static Serializable* create() { return new Player; };
    static Register registration;
    sf::Color filterColor; 
};


class Melee : public GameCharacter
{
public:
    Melee(uint8_t health, uint8_t energy);
    Melee() {};

    void serialize(Archive& fs) override
    {
        GameCharacter::serialize(fs);
        //fs.serialize(filterColor);
    }
};


class Bat : public GameCharacter
{
public:
    Bat(uint8_t health, uint8_t energy);

    inline bool isSolid() const override { return false; };

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
    }
};


class Ranged : public GameCharacter
{
public:
    Ranged(uint8_t health, uint8_t energy);
    Ranged() {};

    void update(Map &map, const float &dt) override;

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(animationTime);
    }
private:
    const float animationDuration = 0.f;
    float animationTime = 0.f;
};
