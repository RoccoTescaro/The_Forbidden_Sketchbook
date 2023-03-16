#pragma once
#include "Utils.h"
#include "Serializable.h"
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <list>
#include <map>
#include <unordered_map>

//The code is higly ispired from a conference of Cris Ryan in CppNow 2022 (https://youtu.be/aWPI7vDOAK8)
//which goes trought the implementation of a binary serialization system 
//I've added some features and made some changes, like changed serialization from binary to textual for easier debugging

template<class Type> constexpr bool is_Pod = std::is_trivial<Type>::value && std::is_standard_layout<Type>::value;
template<class Type> constexpr bool is_Serializable = std::is_base_of<Serializable, Type>::value;
template<class Type> using if_Pod = std::enable_if_t<is_Pod<Type>, void>;
template<class Type> using if_Serializable = std::enable_if_t<is_Serializable<Type>, void>;

class Archive
{
public:

	enum Mode
	{
		Save = std::ios_base::binary | std::ios_base::out | std::ios_base::trunc, //trunc is used to overwrite the file
		Load = std::ios_base::binary | std::ios_base::in,
	};

	Archive(const std::string& path, Mode mode = Mode::Save) : file(path.c_str(), static_cast<std::ios_base::openmode>(mode)), mode(mode) { reset(); };

	//serialize
	template<typename Type> Archive& operator<<(Type& obj);
	//deserialize
	template<typename Type> Archive& operator>>(Type& obj);
	//both serialize and deserialize depending on mode
	template<typename Type> Archive& serialize(Type& obj);

	Mode getMode() const { return mode; }

	inline void setMode(Mode mode)
	{
		if (this->mode == mode)
			return;

		reset();

		switch (mode)
		{
		case Mode::Save: this->mode = Mode::Save; break;
		case Mode::Load: this->mode = Mode::Load; break;
		}
	}

	void reset()
	{
		objToShared.clear();
		idToObj.clear();
		objToId.clear();
	}

private:
	template<typename Type> if_Pod<Type> save(Type& obj);
	template<typename Type> if_Pod<Type> load(Type& obj);
	template<typename Type> if_Serializable<Type> save(Type& obj);
	template<typename Type> if_Serializable<Type> load(Type& obj);

	template<typename Type> if_Pod<Type> save(Type*& ptr);
	template<typename Type> if_Pod<Type> load(Type*& ptr);
	template<typename Type> if_Serializable<Type> save(Type*& ptr);
	template<typename Type> if_Serializable<Type> load(Type*& ptr);

	//definition of a series of template function for some types of class without a serializable function
	//not derivable from Serializable (we could actually use the Adapter Pattern but 
	//would be too inefficent in terms of lines of code). The copiler choose the best fitting overloaded function
	//by parameter comparison, if noone of this is fitting it uses the reference one and let the object
	//to define of to serialize itself.

	template<typename Type> void save(std::shared_ptr<Type>& ptr);
	template<typename Type> void load(std::shared_ptr<Type>& ptr);

	template<typename Type> void save(std::weak_ptr<Type>& ptr);
	template<typename Type> void load(std::weak_ptr<Type>& ptr);

	template<typename Type> void save(std::unique_ptr<Type>& ptr);
	template<typename Type> void load(std::unique_ptr<Type>& ptr);

	template<typename Type> void save(std::vector<Type>& vec);
	template<typename Type> void load(std::vector<Type>& vec);

	template<typename Type> void save(std::list<Type>& list);
	template<typename Type> void load(std::list<Type>& list);
	
	template<typename Key, typename Value> void save(std::map<Key, Value>& map);
	template<typename Key, typename Value> void load(std::map<Key, Value>& map);	
	
	template<typename Type> void save(sf::Vector2<Type>& vec);
	template<typename Type> void load(sf::Vector2<Type>& vec);

	inline void save(sf::Color& color) 
	{
		tab();
		file << "sf::Color -" << std::endl;
		nTab++;
		uint8_t red = color.r;
		uint8_t green = color.g;
		uint8_t blue = color.b;
		uint8_t alpha = color.a;
		save(red);
		save(green);
		save(blue);
		save(alpha);
		nTab--;
	};

	inline void load(sf::Color& color) 
	{
		std::string colorIntro;
		std::getline(file, colorIntro);
		std::getline(file, colorIntro); //not sure why but it work, might be couse >> operator doesn't remove completly last line;
		
		uint8_t red, green, blue, alpha;
		load(red);
		load(green);
		load(blue);
		load(alpha);

		color.r = red;
		color.g = green;
		color.b = blue;
		color.a = alpha;
	};

	inline void save(std::string& string)
	{
		tab();
		file << "std::string : " << string << std::endl;
	}

	inline void load(std::string& string)
	{
		std::string toParse;
		std::getline(file, toParse);
		while (toParse.find("std::string : ") == std::string::npos)
			std::getline(file, toParse);

		toParse.erase(0, toParse.find("std::string : ") + 14);
		toParse.erase(std::remove(toParse.begin(), toParse.end(), '\r'), toParse.end());
		string = toParse;
	}

	inline void tab() //beutify loader file
	{
		for (auto i = 0; i < nTab; i++)
			file << "\t";
	}

	std::fstream file;
	Mode mode;

	uint8_t nTab = 0;
	uint32_t nextId = 1;
	std::unordered_map<void*, uint32_t> objToId;
	std::unordered_map<uint32_t, void*> idToObj;
	std::unordered_map<void*, std::shared_ptr<Serializable>> objToShared;
};

template<typename Type>
Archive& Archive::operator<<(Type& obj)
{
	save(obj);
	return *this;
}

template<typename Type>
Archive& Archive::operator>>(Type& obj)
{
	load(obj);
	return *this;
}

template<typename Type>
Archive& Archive::serialize(Type& obj)
{
	switch (mode)
	{
	case Mode::Save: save(obj); break;
	case Mode::Load: load(obj); break;
	}

	return *this;
}

template<typename Type>
if_Pod<Type> Archive::save(Type& obj)
{
	tab();
	//We cast to the most bits type of data to prevent char and unsigned char 
	//with value 0 1 2 to ruin the structure and readability of the saved file.
	file << typeid(Type).name() << " : " << (long double)obj << std::endl; 
}

template<typename Type>
if_Pod<Type> Archive::load(Type& obj)
{
	//remove type
	std::string intro;
	std::getline(file, intro, ':');
	std::getline(file, intro, ' ');

	long double temp;
	file >> temp;
	obj = (Type)temp;
}

template<typename Type>
if_Serializable<Type> Archive::save(Type& obj)
{
	//there is no real need to save the type 
	//since unlike for pointers where a Base class pointer could
	//actually point to a derived class, here with 
	//reference, we are sure of the type to deserialize
	//but improve error handling further more
	tab();
	file << typeid(Type).name() << " -" << std::endl;
	nTab++;
	std::string typeId = obj.getTypeId();
	save(typeId);
	obj.serialize(*this);
	nTab--;
}

template<typename Type>
if_Serializable<Type> Archive::load(Type& obj)
{
	std::string className;
	std::getline(file, className);

	std::string typeId;
	load(typeId);

	obj.serialize(*this);
}

template<typename Type>
if_Pod<Type> Archive::save(Type*& ptr)
{
	tab();
	file << "pointer to " << typeid(Type).name() << " -" << std::endl;
	nTab++;
	//we can not use variable size id (size_t) since we have no way to deduce it on load 
	//in reverse of the array case that also means that we can not have virtually infinite id 
	uint32_t& objId = objToId[ptr];

	if (objId)
	{
		//obj has already been saved 
		//we just memorize that someone is pointing to him
		save(objId);
		nTab--;
		return;
	}

	objId = nextId++;
	save(objId);
	save(*ptr);
	nTab--;
}

template<typename Type>
if_Pod<Type> Archive::load(Type*& ptr)
{

	std::string pointer;
	std::getline(file, pointer);

	uint32_t objId;
	load(objId);

	Type*& temp = (Type*&)idToObj[objId]; //take it from memory if already deserialized
	if (!temp && (objId != 0)) //else construct it
	{
		temp = new Type;
		load(*temp);
	}

	ptr = temp;
}



template<typename Type>
if_Serializable<Type> Archive::save(Type*& ptr)
{
	//ASSERT(ptr == nullptr);
	tab();
	file << "pointer to " << typeid(Type).name() << " -" << std::endl;
	nTab++;
	//we can not use variable size id (size_t) since we have no way to deduce it on load in reverse of the array case
	//that also means that we can not have virtually infinite id 
	uint32_t& objId = objToId[ptr];

	if (objId)
	{
		//obj has already been saved 
		//we just memorize that someone is pointing to him
		save(objId);
		nTab--;
		return;
	}

	objId = nextId++;
	save(objId);

	std::string typeId = ptr->getTypeId();
	save(typeId);

	save(*ptr);
	nTab--;
}

template<typename Type>
if_Serializable<Type> Archive::load(Type*& ptr)
{
	std::string pointer;
	std::getline(file, pointer);

	uint32_t objId;
	load(objId);

	Type*& temp = (Type*&)(idToObj[objId]); //take it from memory if already deserialized
	if (!temp && objId != 0) //else construct it
	{
		std::string typeId;
		load(typeId);

		temp = dynamic_cast<Type*>(Register::getType(typeId));
		load(*temp);
	}
	ptr = temp;
}

template<typename Type>
void Archive::save(std::shared_ptr<Type>& ptr)
{
	Type* p = ptr.get();
	save(p);
}

template<typename Type>
void Archive::load(std::shared_ptr<Type>& ptr)
{
	Type* p = nullptr;
	load(p);
	if (p)
	{
		std::shared_ptr<Type>& shared = (std::shared_ptr<Type>&) objToShared[p];
		if (!shared)
			shared = std::shared_ptr<Type>(p);
		ptr = shared;
	}
}

template<typename Type>
void Archive::save(std::weak_ptr<Type>& ptr)
{
	std::shared_ptr<Type> shared = ptr.lock();
	save(shared);
}

template<typename Type>
void Archive::load(std::weak_ptr<Type>& ptr)
{
	std::shared_ptr<Type> shared;
	load(shared);
	ptr = std::weak_ptr<Type>(shared);
}

template<typename Type>
void Archive::save(std::unique_ptr<Type>& ptr)
{
	Type* p = (Type*)ptr.get();
	save(p);
}

template<typename Type>
void Archive::load(std::unique_ptr<Type>& ptr)
{
	Type* pType = nullptr;
	load(pType);
	ptr = std::unique_ptr<Type>(pType);
}

template<typename Type>
void Archive::save(std::vector<Type>& vec)
{
	//as for pointers id on load we dont know how many bit we have to read for vector size so we limit it to uint32_t
	tab();
	file << "vector of " << typeid(Type).name() << " -" << std::endl;
	nTab++;

	uint32_t size = uint32_t(vec.size());
	save(size);

	for (Type& item : vec)
		save(item);
	nTab--;
}

template<typename Type>
void Archive::load(std::vector<Type>& vec)
{
	std::string vector;
	std::getline(file, vector);

	uint32_t size;
	load(size);
	vec.clear();
	vec.reserve(size);
	for (uint32_t i = 0; i < size; i++)
	{
		//ASSERT(!std::is_default_constructible<Type>::value);
		Type type = {};
		load(type);
		//we need to differentiate std::vector<std::unique_ptr<Type>> from std::vector<Type>
		//we could do it by specialize another function with that different parameter and let the compiler choose 
		//the best fitting function but the implementation is pretty much the same 
		if constexpr (std::is_convertible<Type, std::unique_ptr<void>>::value) 
			vec.push_back(std::move(type));
		else
			vec.push_back(type);
	}
}

template<typename Type>
void Archive::save(std::list<Type>& list)
{
	tab();
	file << "list of " << typeid(Type).name() << " -" << std::endl;
	nTab++;
	//as for pointers id on load we dont know how many bit we have to read for vector size so we limit it to uint32_t
	uint32_t size = uint32_t(list.size());
	save(size);
	for (Type& item : list)
		save(item);
	nTab--;
}

template<typename Type>
void Archive::load(std::list<Type>& list)
{
	std::string listIntro;
	std::getline(file, listIntro);

	uint32_t size;
	load(size);
	list.clear();
	for (uint32_t i = 0; i < size; i++)
	{
		//ASSERT(!std::is_default_constructible<Type>::value);
		Type type = {};
		load(type);
		if constexpr (std::is_convertible<Type, std::unique_ptr<void>>::value)
			list.push_back(std::move(type));
		else
			list.push_back(type);
	}
}

template<typename Key, typename Value>
void Archive::save(std::map<Key, Value>& map)
{
	tab();
	file << "map of " << typeid(Key).name() << " to " << typeid(Value).name() << " -" << std::endl;
	nTab++;
	uint32_t size = uint32_t(map.size());
	save(size);
	for (auto& pair : map)
	{
		save(pair.first);
		save(pair.second);
	}
	nTab--;
}

template<typename Key, typename Value>
void Archive::load(std::map<Key, Value>& map) 
{
	std::string mapIntro;
	std::getline(file, mapIntro);

	uint32_t size;
	load(size);
	map.clear();
	for (uint32_t i = 0; i < size; i++)
	{
		//ASSERT(!std::is_default_constructible<Key>::value);
		//ASSERT(!std::is_default_constructible<Value>::value);
		Key key{};
		Value value{};
		load(key);
		load(value);
		map[key] = value;
	}
}

template<typename Type>
void Archive::save(sf::Vector2<Type>& vec)
{
	tab();
	file << "sf::Vector2<" << typeid(Type).name() << "> -" << std::endl;
	nTab++;
	save(vec.x);
	save(vec.y);
	nTab--;
}

template<typename Type>
void Archive::load(sf::Vector2<Type>& vec)
{
	std::string vecIntro;
	std::getline(file, vecIntro);
	load(vec.x);
	load(vec.y);
}
