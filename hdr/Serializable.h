#pragma once
#include <typeinfo>
#include <iostream>

class Archive;

class Serializable
{
public:
	virtual ~Serializable() = default;
	virtual void serialize(Archive& fs) = 0;
	virtual std::string getTypeId() const { return static_cast<std::string>(typeid(*this).name()); };
};


