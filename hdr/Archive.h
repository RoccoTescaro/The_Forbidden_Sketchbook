#pragma once
#include "Serializable.h"
#include <fstream>
#include <vector>
#include <list>
#include <map>

//create some expr to enable use of some function at run time 
//essencialy serialization of Pod (plain old data eg: float, int, bool, ecc.) and Serializable object differs
template<class Type> constexpr bool is_Pod = std::is_trivial<Type>::value && std::is_standard_layout<Type>::value; 
template<class Type> constexpr bool is_Serializable = std::is_base_of<Serializable<Type>, Type>::value;
template<class Type, class RetType = void> using if_Pod = std::enable_if_t<is_Pod<Type>, RetType>;
template<class Type, class RetType = void> using if_Serializable = std::enable_if_t<is_Serializable<Type>, RetType>;

class Archive
{
public:
	enum Mode
	{
		Save = std::ios_base::binary | std::ios_base::out | std::ios_base::trunc,
		Load = std::ios_base::binary | std::ios_base::in,
	};

	Archive(const char* fileName, Mode mode = Mode::Save) 
		: file(fileName, mode), mode(mode) 
	{
		reset(); 
	};

	//serialize
	template<typename Type> Archive& operator<<(Type& obj);
	//deserialize
	template<typename Type> Archive& operator>>(Type& obj);
	//both serialize and deserialize depending on mode
	template<typename Type> Archive& serialize(Type& obj);
	
	inline Mode getMode() const
	{
		return mode;
	};

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

	inline void reset()
	{
		objToShared.clear();
		idToObj.clear();
		objToId.clear();
	};

private:

	template<typename Type> if_Pod<Type> save(Type& obj);
	template<typename Type> if_Pod<Type> load(Type& obj);
	template<typename Type> if_Serializable<Type> save(Type& obj);
	template<typename Type> if_Serializable<Type> load(Type& obj);

	template<typename Type, size_t size> if_Pod<Type> save(Type(&array)[size]);
	template<typename Type, size_t size> if_Pod<Type> load(Type(&array)[size]);
	template<typename Type, size_t size> if_Serializable<Type> save(Type(&array)[size]);
	template<typename Type, size_t size> if_Serializable<Type> load(Type(&array)[size]);

	template<typename Type> if_Pod<Type> save(Type*& ptr);
	template<typename Type> if_Pod<Type> load(Type*& ptr);
	template<typename Type> if_Serializable<Type> save(Type*& ptr);
	template<typename Type> if_Serializable<Type> load(Type*& ptr);

	//definition of series of template function for some types of class without a serializable function
	//not derivable from Serializable<Type> (we could actually use the Adapter pattern but 
	//would be too inefficent in terms of lines of code)

	template<typename Type> void save(std::shared_ptr<Type>& ptr);
	template<typename Type> void load(std::shared_ptr<Type>& ptr);

	//#TODO add a save and load for weak_ptr

	template<typename Type> void save(std::unique_ptr<Type>& ptr);
	template<typename Type> void load(std::unique_ptr<Type>& ptr);

	template<typename Type> void save(std::vector<Type>& vec);
	template<typename Type> void load(std::vector<Type>& vec);

	template<typename Type> void save(std::list<Type>& list);
	template<typename Type> void load(std::list<Type>& list);

	template<typename Type, size_t size> void save(std::array<Type, size>& array);
	template<typename Type, size_t size> void load(std::array<Type, size>& array);

	template<typename Key, typename Value> void save(std::map<Key, Value>& map);
	template<typename Key, typename Value> void load(std::map<Key, Value>& map);

	template<typename Type> void save(sf::Vector2<Type>& vec2);
	template<typename Type> void load(sf::Vector2<Type>& vec2);

	template<size_t size> void save(char(&string)[size]);
	template<size_t size> void load(char(&string)[size]);

	inline void save(std::string& str)
	{
		uint32_t size = uint32_t(str.length());
		save(&size, sizeof(uint32_t));
		if (size)
			save((void*)str.data(), size);
	}

	inline void load(std::string& str)
	{
		uint32_t size;
		load(&size, sizeof(uint32_t));
		str.clear();
		if (size)
		{
			std::string str2(size + 1, '\0');
			load((void*)str2.data(), size);
			str.swap(str2);
		}
	}

	inline void save(void* data, size_t size) 
	{
		ASSERT("trying to save in loading mode", mode == Mode::Save)
		file.write((char*)data, size);
	}

	inline void load(void* data, size_t size)
	{
		ASSERT("trying to load in saving mode", mode == Mode::Load)
		file.read((char*)data, size);
	}

	std::fstream file;
	Mode mode;

	uint32_t nextId = 0;
	std::unordered_map<void*, uint32_t> objToId;
	std::unordered_map<uint32_t, void*> idToObj;
	std::unordered_map<void*, std::shared_ptr<SerializableBase>> objToShared;
};

template<typename Type>
inline Archive& Archive::operator<<(Type& obj)
{
	save(obj);
	return *this;
}

template<typename Type>
inline Archive& Archive::operator>>(Type& obj)
{
	load(obj);
	return *this;
}

template<typename Type>
inline Archive& Archive::serialize(Type& obj)
{
	switch (mode)
	{
	case Mode::Save: save(obj); break;
	case Mode::Load: load(obj); break;
	}

	return *this;
}

template<typename Type>
inline if_Pod<Type> Archive::save(Type& obj)
{
	save(&obj, sizeof(obj));
}

template<typename Type>
inline if_Pod<Type> Archive::load(Type& obj)
{
	load(&obj, sizeof(obj));
}

template<typename Type>
inline if_Serializable<Type> Archive::save(Type& obj)
{
	//there is no real need to save the type 
	//since unlike for pointers where a Base class pointer could
	//actually point to a derived class, here with 
	//reference, we are sure of the type to deserialize
	//but improve error handling further more
	uint32_t typeId = Serializable<Type>::getTypeId();
	save(&typeId, sizeof(uint32_t));
	
	obj.serialize(*this);
}

template<typename Type>
inline if_Serializable<Type> Archive::load(Type& obj)
{
	uint32_t typeId;
	load(&typeId, sizeof(uint32_t));
	ASSERT("trying to load wrong serializable object", typeId == Serializable<Type>::getTypeId());

	obj.serialize(*this);
}

template<typename Type, size_t size>
inline if_Pod<Type> Archive::save(Type(&array)[size])
{
	//there is no real need to save the size or neither the type 
	//since in the corresponding load function will receive it as imput anyways
	//but improve error handling further more
	save(&size, sizeof(size));

	save(&array, sizeof(array));
}

template<typename Type, size_t size>
inline if_Pod<Type> Archive::load(Type(&array)[size])
{
	size_t savedSize;
	load(&savedSize, sizeof(size));
	ASSERT("trying to load an array with wrong size", savedSize == size);

	load(&array, sizeof(array));
}

template<typename Type, size_t size>
inline if_Serializable<Type> Archive::save(Type(&array)[size])
{
	//there is no real need to save the size or neither the type 
	//since in the corresponding load function will receive it as imput anyways
	//but improve error handling further more
	save(&size, sizeof(size));

	uint32_t typeId = Serializable<Type>::getTypeId();
	save(&typeId, sizeof(uint32_t));

	for (auto& item : array)
		item.serialize(*this);
}

template<typename Type, size_t size>
inline if_Serializable<Type> Archive::load(Type(&array)[size])
{
	size_t savedSize;
	load(&savedSize, sizeof(size));
	ASSERT("trying to load an array with wrong size", savedSize == size);

	uint32_t typeId;
	load(&typeId, sizeof(uint32_t));
	ASSERT("trying to load wrong serializable object", typeId == Serializable<Type>::getTypeId());

	for (auto& item : array)
		item.serialize(*this);
}

template<typename Type>
inline if_Pod<Type> Archive::save(Type*& ptr)
{
	//we can not use variable size id (size_t) since we have no way to deduce it on load 
	//in reverse of the array case that also means that we can not have virtually infinite id 
	uint32_t& objId = objToId[ptr];

	if (objId)
	{
		//obj has already been saved 
		//we just memorize that someone is pointing to him
		save(&objId, sizeof(uint32_t));
		return;
	}

	objId = nextId++;
	save(&objId, sizeof(uint32_t));
	save(*ptr);
}

template<typename Type>
inline if_Pod<Type> Archive::load(Type*& ptr)
{
	uint32_t objId;
	load(&objId, sizeof(uint32_t));

	ptr = (Type*&)idToObj[objId]; //take it from memory if already deserialized
	if (!ptr) //else construct it
	{
		ptr = new Type;
		load(*ptr);
	}
}

template<typename Type>
inline if_Serializable<Type> Archive::save(Type*& ptr)
{
	//we can not use variable size id (size_t) since we have no way to deduce it on load in reverse of the array case
	//that also means that we can not have virtually infinite id 
	uint32_t& objId = objToId[ptr];

	if (objId)
	{
		//obj has already been saved 
		//we just memorize that someone is pointing to him
		save(&objId, sizeof(uint32_t));
		return;
	}

	objId = nextId++;
	save(&objId, sizeof(uint32_t));
	
	uint32_t typeId = Serializable<Type>::getTypeId();
	save(&typeId, sizeof(uint32_t));

	save(*ptr);
}

template<typename Type>
inline if_Serializable<Type> Archive::load(Type*& ptr)
{
	uint32_t objId;
	load(&objId, sizeof(uint32_t));

	ptr = (Type*&)idToObj[objId]; //take it from memory if already deserialized
	if (!ptr) //else construct it
	{
		uint32_t typeId;
		load(&typeId, sizeof(uint32_t));

		ptr = (Type *)Factory::get(typeId);
		ptr->serialize(*this);
	}
}

template<typename Type>
inline void Archive::save(std::shared_ptr<Type>& ptr)
{
	Type* p = ptr.get();
	save(p);
}

template<typename Type>
inline void Archive::load(std::shared_ptr<Type>& ptr)
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
inline void Archive::save(std::unique_ptr<Type>& ptr)
{
	Type* p = ptr.get();
	save(p);
}

template<typename Type>
inline void Archive::load(std::unique_ptr<Type>& ptr)
{
	Type* pType = nullptr;
	load(pType);
	ptr = std::unique_ptr<Type>(pType);
}

template<typename Type>
inline void Archive::save(std::vector<Type>& vec)
{
	//as for pointers id on load we dont know how many bit we have to read for vector size so we limit it to uint32_t
	uint32_t size = uint32_t(vec.size());
	save(&size, sizeof(uint32_t));
	for (Type& item : vec)
		save(item);
}

template<typename Type>
inline void Archive::load(std::vector<Type>& vec)
{
	uint32_t size;
	load(&size, sizeof(uint32_t));
	vec.clear();
	vec.reserve(size);
	for (uint32_t i = 0; i < size; i++)
	{
		ASSERT("serializable class has not an empty contructor", std::is_default_constructible<Type>::value);
		Type type = {};
		load(type);
		vec.push_back(type);
	}
}

template<typename Type>
inline void Archive::save(std::list<Type>& list)
{
	//as for pointers id on load we dont know how many bit we have to read for vector size so we limit it to uint32_t
	uint32_t size = uint32_t(list.size());
	save(&size, sizeof(uint32_t));
	for (Type& item : list)
		save(item);
}

template<typename Type>
inline void Archive::load(std::list<Type>& list)
{
	uint32_t size;
	load(&size, sizeof(uint32_t));
	list.clear();
	for (uint32_t i = 0; i < size; i++)
	{
		ASSERT("serializable class has not an empty contructor", std::is_default_constructible<Type>::value);
		Type type = {};
		load(type);
		list.push_back(type);
	}
}

template<typename Type, size_t size>
inline void Archive::save(std::array<Type, size>& array)
{
	save(&size, sizeof(size)); //not really needed
	for (Type& item : array)
		save(item);
}

template<typename Type, size_t size>
inline void Archive::load(std::array<Type, size>& array)
{
	size_t savedSize;
	load(&savedSize, sizeof(size));
	ASSERT("trying to load an array with wrong size", savedSize == size);
	for (Type& item : array)
		load(item);
}

template<typename Key, typename Value>
inline void Archive::save(std::map<Key, Value>& map)
{
	uint32_t size = uint32_t(map.size());
	save(&size, sizeof(uint32_t));
	for (auto& pair : map)
	{
		save(pair.first);
		save(pair.second);
	}
}

template<typename Key, typename Value>
inline void Archive::load(std::map<Key, Value>& map)
{
	uint32_t size;
	load(&size, sizeof(uint32_t));
	map.clear();
	for (uint32_t i = 0; i < size; i++)
	{
		ASSERT("key has not an empty contructor", std::is_default_constructible<Type>::value);
		ASSERT("value has not an empty contructor", std::is_default_constructible<Type>::value);
		Key key{};
		Value value{};
		load(key);
		load(value);
		map[key] = value;
	}
}

template<typename Type>
inline void Archive::save(sf::Vector2<Type>& vec2)
{
	save(vec2.x);
	save(vec2.y);
}

template<typename Type>
inline void Archive::load(sf::Vector2<Type>& vec2)
{
	load(vec2.x);
	load(vec2.y);
}

template<size_t size>
inline void Archive::save(char(&string)[size])
{
	char* i = string;
	uint32_t len = 0;

	//check how many of the size char are effectivly defined
	while (*i++ && size > len++);
	if (size < len) return ERROR("size < len");

	save(&len, sizeof(uint32_t));
	if (len)
		save(string, len);
}

template<size_t size>
inline void Archive::load(char(&string)[size])
{
	uint32_t len;
	load(&len, sizeof(uint32_t));

	if (len <= size) return ERROR("size <= len"); 

	if (len)
		load(string, len);
	string[len] = '\0';
}
