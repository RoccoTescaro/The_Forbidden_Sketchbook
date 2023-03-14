#pragma once
#include <queue>
#include <memory>
#include "Entity.h"
#include "PathFinding.h"
#include "Utils.h"
#include "Weapon.h"
#include <cmath>


class GameCharacter : public Entity //TODO make abstract
{
public:
    GameCharacter(uint8_t maxHealth, int8_t health, uint8_t maxEnergy, int8_t energy, uint8_t priority) 
        : maxHealth(maxHealth), maxEnergy(maxEnergy), priority(priority), weapon(new Weapon(1,1,1))
    {
        setHealth(health); //fix health if greater than maxHealth
        setEnergy(energy); //fix energy if greater tha maxEnergy
    };

    virtual ~GameCharacter() = default;

    inline void render(sf::RenderWindow& window) override 
    {   
        Entity::render(window);
        weapon->render(window);    
    };
    
    //GETTER AND SETTER
    inline bool isSolid() const override { return true; };
    inline const uint8_t& getMaxHealth() const { return maxHealth; }; 
    inline const uint8_t& getMaxEnergy() const { return maxEnergy; };
    inline const int8_t& getHealth() const { return health; };
    inline const int8_t& getEnergy() const { return energy; };
    inline uint8_t getPriority() const { return priority; };
    inline std::unique_ptr<PathAlgorithm>& getMovementStrategy() { return movementStrategy; };
    inline Weapon& getWeapon() { return *weapon; };

    inline void setWeapon(const Weapon &newWeapon){ weapon.reset(new Weapon(newWeapon));};
    inline void setPos(const sf::Vector2<float>& pos) override  
    {   
        Entity::setPos(pos);
        weapon->setPos(pos); 
    };
    
    inline void setHealth(const int8_t newHealth) { health = std::max<int8_t>(std::min<int8_t>(newHealth, maxHealth), 0); }
    inline void setEnergy(const int8_t newEnergy) { energy = std::max<int8_t>(std::min<int8_t>(newEnergy, maxEnergy), 0); }
                                            
    //FUNCTIONS
    inline void turnReset(){ energy = maxEnergy; }; 
    
    inline void serialize(Archive& fs) 
    {
        Entity::serialize(fs);
        fs.serialize(health);
        fs.serialize(energy);
    }

    virtual void move(Map &map, sf::Vector2<float> target, const float &dt); 
    void interact(Map &map, sf::Vector2<float> target, const float &dt) override; 

protected:
    const float speed = 200.f;
    const uint8_t maxHealth = 20;  
    const uint8_t maxEnergy = 20;
    const uint8_t priority = 0;
    int8_t health = 0;
    int8_t energy = 0;
    std::unique_ptr<PathAlgorithm> movementStrategy;
    std::unique_ptr<Weapon> weapon;
};



