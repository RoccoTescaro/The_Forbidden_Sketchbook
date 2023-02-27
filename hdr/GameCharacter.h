#pragma once
#include <queue>
#include <memory>
#include "Entity.h"
#include "PathFinding.h"
#include "Utils.h"

class Weapon;

class GameCharacter : public Entity //TODO fix constructors
{
public:
    GameCharacter(uint8_t maxHealth, uint8_t health, uint8_t maxEnergy, uint8_t energy, uint8_t priority) 
        : maxHealth(maxHealth), maxEnergy(maxEnergy), health(health), energy(energy), priority(priority) {};
    GameCharacter() :  priority(0) {};

    //ENTITY
    void update(Map &map, const float &dt) override;
    void execute(GameCharacter &gameCharacter, Map &map) override;
    inline void render() override 
    {   
        Entity::render();
        //weapon->render();    
    };
    
    //GAMECHARACTER GET&SET
    inline bool isSolid() const override { return true; };
    inline uint8_t getMaxHealth() const { return maxHealth; }; //need to return as a reference and not by copy to allow hud auto update
    inline uint8_t getMaxEnergy() const { return maxEnergy; }; //need to return as a reference and not by copy to allow hud auto update
    inline const uint8_t& getHealth() const { return health; };
    inline const uint8_t& getEnergy() const { return energy; };
    inline uint8_t getPriority() const { return priority; };
    inline float getRange() const { return 1; /*return weapon->getRange()*/};


    //GAMECHARACTER FUNCTIONS
    void updateStepQueue( Map &map, const sf::Vector2<float> target);
    inline void turnReset(){    energy+=maxEnergy;
                                stepQueue.clear();           }; 
    inline bool isStepQueueEmpty(){     return stepQueue.empty();   };

    inline void serialize(Archive& fs) 
    {
        Entity::serialize(fs);
        fs.serialize(health);
        //fs.serialize(stepQueue);
    }
protected:
    //STATS
    const uint8_t maxHealth = 20;  
    const uint8_t maxEnergy = 20;
    const uint8_t priority = 0;
    uint8_t health = 0;
    uint8_t energy = 0;
    //Weapon weapon; //std::unique_ptr<Weapon> weapon = nullptr;

    //MOVEMENT
    const float animationSpeed = 300.f;
    std::deque<sf::Vector2<float>> stepQueue;
    std::unique_ptr<PathAlgorithm> movementStrategy;

};


class Player : public GameCharacter
{
public:
    Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB);
    Player() : Player(100,0,255,255,255) {}; //should initialize by the list initialization the const memeber that we dont wont/cant serialize

    inline const sf::Color& getFilterColor() const { return filterColor; };
  
    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(filterColor);
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

    void serialize(Archive& fs) override
    {
        GameCharacter::serialize(fs);
    }
private:
    static Serializable* create() { return new Melee{100,100}; };
    static Register registration;
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
private:
    static Serializable* create() { return new Bat{100,100}; };
    static Register registration;
};


class Ranged : public GameCharacter
{
public:
    Ranged(uint8_t health, uint8_t energy);

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(animationTime);
    }
private:
    static Serializable* create() { return new Ranged{100,100}; };
    static Register registration;

    const float animationDuration = 0.f;
    float animationTime = 0.f;
};
