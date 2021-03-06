// #include "Manager.h"
#include <algorithm>
#include <cassert>
#include <iostream>

template<typename T>
inline Resource<T>::Resource(const Handle handle, const std::string& filename) :
		mName(std::find_if(filename.rbegin(), filename.rend(), [](char ch)
		{	return ch == '/';}).base(), filename.end()), mFilename(filename)
{
	mHandle = handle;
}

template<typename T>
inline Resource<T>::~Resource()
{
}

template<typename T>
inline std::string Resource<T>::GetName()
{
	return mName;
}

template<typename T>
inline std::string Resource<T>::GetFilename()
{
	return mFilename;
}

template<typename T>
inline Handle Resource<T>::GetHandle()
{
	return mHandle;
}

template<typename T>
inline T* Resource<T>::GetRaw()
{
	return mRaw;
}

//-----------------------------------------------------------------------------

template<typename Resource, typename Loader>
inline ResourceManager<Resource, Loader>::ResourceManager()
{
	Logger::Debug << "Initializing ResourceManager for " << typeid(Resource).name() << Logger::endl;
}

template<typename Resource, typename Loader>
inline ResourceManager<Resource, Loader>::~ResourceManager()
{
	EmptyList();
}

template<typename Resource, typename Loader>
inline Resource* ResourceManager<Resource, Loader>::operator [](const std::string& name)
{
	if (name.empty())
	{
		Logger::Error << "Cannot get unamed resource" << Logger::endl;
		return NULL;
	}

	for (auto* p : mList)
		if (p != NULL)
			if (p->GetName() == name)
				return p;

	Logger::Error << "No resource with handle " << name << " on ResourceManager" << Logger::endl;
	return NULL;
}

template<typename Resource, typename Loader>
inline Resource* ResourceManager<Resource, Loader>::operator [](Handle handle)
{
	if (handle < mList.size() && handle >= 0)
		return mList[handle];

	Logger::Error << "No resource with handle " << handle << " on ResourceManager" << Logger::endl;
	return NULL;
}

template<typename Resource, typename Loader>
inline Resource* ResourceManager<Resource, Loader>::GetElement(const std::string& name)
{
	if (name.empty())
	{
		Logger::Error << "Cannot get unamed resource" << Logger::endl;
		return NULL;
	}

	for (auto* p : mList)
		if (p != NULL)
			if (p->GetName() == name)
				return p;

	Logger::Error << "No resource named " << name << " on ResourceManager" << Logger::endl;
	return NULL;
}

template<typename Resource, typename Loader>
inline Resource* ResourceManager<Resource, Loader>::GetElement(Handle handle)
{
	if (handle < mList.size() && handle >= 0)
		return mList[handle];

	Logger::Error << "No resource with handle " << handle << " on ResourceManager" << Logger::endl;
	return NULL;
}

template<typename Resource, typename Loader>
inline void ResourceManager<Resource, Loader>::EmptyList()
{
	for (auto* p : mList)
		SAFE_DELETE(p);

	while (!mHandles.empty())
		mHandles.pop();

	mList.clear();
}

template<typename Resource, typename Loader>
template<typename ... Filename>
inline Handle ResourceManager<Resource, Loader>::Load(const std::string& name, const std::string& filename, Filename ... filenames)
{
	assert(!name.empty() && "resource must have a non empty name");
	assert(!name.empty() && "resource must be loaded from non empty filename");

	Resource* resource = NULL;
	for (auto* p : mList)
			if (p != NULL)
				if (p->GetName() == name)
					resource = p;
	if (resource != NULL)
	{
		Logger::Info << name << " already in manager" << Logger::endl;
		return resource->GetHandle();
	}
	resource = NULL;

	bool available = !mHandles.empty();
	unsigned int handle;
	if (available)
		handle = mHandles.top();
	else
		handle = mList.size();

	Logger::Info << "Loading " << name << Logger::endl;
	bool loaded = Loader::Load(&resource, handle, filename, std::forward<Filename>(filenames)...);
	if (!loaded)
	{
		Logger::Error << "Failed to load " << name << Logger::endl;
		// TODO return default resource of this type (e.g., default texture, default mesh...)
	}

	if (resource != NULL)
	{
		resource->mName = name;

		if (available)
		{
			mHandles.pop(); // removes handle from stack
			mList[handle] = resource;
		}
		else
		{
			mList.push_back(resource);
		}

		return handle;
	}
	else
	{
		Logger::Info << "Finished loading resource collection " << name << Logger::endl;
	}

	return -1;
}

template<typename Resource, typename Loader>
inline Handle ResourceManager<Resource, Loader>::Add(const std::string& name, typename Resource::raw_type* raw)
{
	assert(!name.empty() && "resource must have a non empty name");
	assert(raw != NULL && "raw pointer cannot be NULL");

	Resource* resource = NULL;
		for (auto* p : mList)
				if (p != NULL)
					if (p->GetName() == name)
						resource = p;
	if (resource != NULL)
	{
		Logger::Info << name << " already in manager" << Logger::endl;
		return resource->GetHandle();
	}
	resource = NULL;

	bool available = !mHandles.empty();
	unsigned int handle;
	if (available)
	{
		handle = mHandles.top();
		mHandles.pop();
	}
	else
		handle = mList.size();

	Logger::Info << "Adding " << name << Logger::endl;
	resource = new Resource(handle, name);
	resource->mRaw = raw;

	assert(resource != NULL && "failed to add resource");

	if (available)
		mList[handle] = resource;
	else
		mList.push_back(resource);

	return handle;
}

template<typename Resource, typename Loader>
inline void ResourceManager<Resource, Loader>::Remove(Handle handle)
{
	if (handle < 0 || handle >= mList.size() || mList[handle] == NULL)
		return;

	Resource* resource = mList[handle];
	mHandles.push(handle);
	delete resource;
	mList[handle] = NULL;
}
