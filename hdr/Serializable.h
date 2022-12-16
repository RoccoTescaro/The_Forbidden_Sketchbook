#pragma once
#include "Utils.h"
#include <unordered_map>

class Archive;

class SerializableBase 
{
public:
    virtual ~SerializableBase() {};

    virtual void serialize(Archive& fs) = 0;
};

class Factory
{
public:
    Factory(uint32_t typeId, SerializableBase* (*create)())
    {
        typeIdToCreate[typeId] = create;
    };

    inline static SerializableBase* get(uint32_t typeId)
    {
        return typeIdToCreate[typeId]();
    };

private:
    //map a typeid to a type create function
    static std::unordered_map<uint32_t, SerializableBase*(*)()> typeIdToCreate; 
};

std::unordered_map<uint32_t, SerializableBase* (*)()> typeIdToCreate;

template<class Type>
class Serializable : public SerializableBase
{
public:
    inline static SerializableBase* create() const
    {
        //every class that derives from Serializable MUST have an empty constructor.
        ASSERT("serializable class has not an empty contructor", std::is_default_constructible<Type>::value);

        return new Type;
    };

    inline static uint32_t getTypeId() const
    {
        return typeid(Type).hash_code();
    };

private:

    //this function is never used, its only purpose is to not allow the compiler to not istantiate 
    //the regist attribute due to optimization.
    inline virtual const Factory* getRegist() const { return &regist; };

    //the object istantiated allows the registration of the class at compiler time 
    //in the factory map so that from a given id that is always the same 
    //we can istantiate a similar object. 
    static const Factory regist;
};

template<class Type>
const Factory Serializable<Type>::regist{ typeid(Type).hash_code(), create };