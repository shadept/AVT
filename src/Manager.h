#ifndef MANAGER_H_
#define MANAGER_H_

#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "Logger.h"

typedef unsigned int Handle;

template<typename T>
class Resource
{
public:
	Resource(const Handle handle, const std::string& filename);
	virtual ~Resource();

	std::string GetName();
	std::string GetFilename();
	Handle GetHandle();
	T* GetRaw();

//	void IncRef();
//	void DecRef();
//	unsigned int GetRefCount() const;

	typedef T raw_type;

	// internal use
public:
	T* mRaw = NULL;
	std::string mName;

private:
	std::string mFilename;
	std::string mPath;
	Handle mHandle = 0;
//	unsigned int mRefCount;
};

#define DECLARE_RESOURCE(C) \
	struct C##Resource : public Resource<C> { \
		C##Resource(Handle handle, const std::string& filename) : Resource(handle, filename) {};\
		friend struct C##Loader;\
	}

#define DECLARE_RESOURCE_(C, B) \
	struct C##Resource : public Resource<B> { \
		C##Resource(Handle handle, const std::string& filename) : Resource(handle, filename) {};\
		friend struct C##Loader;\
	}

#define DECLARE_LOADER(C)\
	struct C##Loader {\
		static bool Load(C##Resource** resource, Handle handle, const std::string& filename);\
	}

#define DECLARE_RESOURCE_AND_LOADER(C) DECLARE_RESOURCE(C); DECLARE_LOADER(C)
#define DECLARE_RESOURCE_AND_LOADER_(C, B) DECLARE_RESOURCE_(C, B); DECLARE_LOADER(C)

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

template<typename Resource, typename Loader>
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	Resource* operator[](const std::string& name);
	Resource* operator[](Handle handle);

	Resource* GetElement(const std::string& name);
	Resource* GetElement(Handle handle);

	void EmptyList();

	template<typename... Filename>
	Handle Load(const std::string& name, const std::string& filename, Filename... filenames);
	Handle Add(const std::string& name, typename Resource::raw_type* raw);
	void Remove(Handle handle);

private:
	std::stack<Handle> mHandles;
	std::vector<Resource*> mList;
};

#define DECLARE_MANAGER(C) extern ResourceManager<C##Resource, C##Loader> C##Manager
#define IMPLEMENT_MANAGER(C) ResourceManager<C##Resource, C##Loader> C##Manager

#include "Manager.inl"

#endif // MANAGER_H_
