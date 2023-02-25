#pragma once
#include <queue>
#include <memory>
#include "Entity.h"

class Weapon;
class PathAlgorithm {};
class AStar : public PathAlgorithm{};
class DiglettMovement : public PathAlgorithm{};

class GameCharacter : public Entity //TODO fix constructors
{
public:
    GameCharacter(uint8_t maxHealth, uint8_t health, uint8_t maxEnergy, uint8_t energy, uint8_t priority) 
        : maxHealth(maxHealth), health(health), maxEnergy(maxEnergy), energy(energy), priority(priority) {};
    GameCharacter() : energy(0), priority(0) {};

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
    inline uint8_t getMaxEnergy() const { return maxEnergy; };
    inline const uint8_t& getHealth() const { return health; };
    inline const uint8_t& getEnergy() const { return energy; };
    inline uint8_t getPriority() const { return priority; };
    inline uint8_t getRange() const { return 0; /*return weapon->getRange()*/};


    inline void setEnergy(const uint8_t newEnergy) { energy=newEnergy; };


    //GAMECHARACTER FUNCTIONS
    bool isInRange(Map &map) const;
    void updateStepQueue(const sf::Vector2<float> target);
    inline void turnReset(){    setEnergy(getMaxEnergy());
                                stepQueue.clear();                  };
    inline bool isStepQueueEmpty(){     return stepQueue.empty();   };

    inline void serialize(Archive& fs) 
    {
        Entity::serialize(fs);
        fs.serialize(health);
        fs.serialize(energy);
        //fs.serialize(stepQueue);
    }
protected:
    //STATS
    const uint8_t maxHealth = 0;  
    const uint8_t maxEnergy = 0;
    const uint8_t priority = 0;
    uint8_t health = 0;
    uint8_t energy = 0;
    //Weapon weapon; //std::unique_ptr<Weapon> weapon = nullptr;

    //MOVEMENT
    //std::unique_ptr<PathAlgorithm> movementStrategy;
    const float animationSpeed = 300.f;
    std::deque<sf::Vector2<float>> stepQueue;
};


class Player : public GameCharacter
{
public:
    Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB);
    Player() : Player(100,100,255,255,255) {}; //should initialize by the list initialization the const memeber that we dont wont/cant serialize

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
