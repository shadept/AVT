#ifndef MANAGER_H_
#define MANAGER_H_

#include <fstream>
#include <iostream>
#include <map>
#include <string>

template <typename T>
class Manager
{
public:
	static const T* Load(const std::string& filename)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			std::cout << "File " << filename << " does not exist" << std::endl;
			exit(1);
		}

		T* obj = new T();
		std::cout << "Loading " << filename << std::endl;
		obj->Load(file);
		mEntities[filename] = obj;
		return obj;
	}

	static const T* Get(const std::string& name)
	{
		if (mEntities.find(name) == mEntities.end()) {
			return Load(name);
		} else {
			return mEntities[name];
		}
	}

private:
	static std::map<std::string, const T*> mEntities;
};


template <typename T>
std::map<std::string, const T*> Manager<T>::mEntities;

#endif // MANAGER_H_