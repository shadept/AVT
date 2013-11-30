#ifndef MANAGER_H_
#define MANAGER_H_

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

typedef unsigned int Handle;

struct MatchPathSeparator
{
	bool operator()(char ch) const
	{
		return ch == '/';
	}
};

template<typename T>
class Resource
{
public:
	Resource(const Handle handle, const std::string& filename) :
			mRaw(0), mName(std::find_if(filename.rbegin(), filename.rend(), MatchPathSeparator()).base(), filename.end()), mFilename(filename), mHandle(handle)
	{
	}
	virtual ~Resource()
	{
	}

	std::string GetName()
	{
		return mName;
	}
	std::string GetFilename()
	{
		return mFilename;
	}
	Handle GetHandle()
	{
		return mHandle;
	}

	T* GetRaw()
	{
		return mRaw;
	}

//	void IncRef();
//	void DecRef();
//	unsigned int GetRefCount() const;

	typedef T raw_type;

	// internal use
public:
	T* mRaw;

private:
	std::string mName;
	std::string mFilename;
	std::string mPath;
	Handle mHandle;
//	unsigned int mRefCount;
};

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

template<typename T, typename Loader>
class ResourceManager
{
public:
	ResourceManager()
	{
	}

	~ResourceManager()
	{
		EmptyList();
	}

	T* GetElement(const std::string& name)
	{
		if (name.empty())
		{
			return NULL;
		}

		for (auto* p : mList)
		{
			if (p != NULL)
				if (p->GetFilename() == name)
					return p;
		}

		return NULL;
	}

	T* GetElement(Handle handle)
	{
		if (handle < mList.size() && handle >= 0)
			return mList[handle];
		return NULL;
	}

	void EmptyList()
	{
		for (auto* p : mList)
			SAFE_DELETE(p);

		while (!mHandles.empty())
			mHandles.pop();

		mList.clear();
	}

	void Remove(Handle handle)
	{
		if (handle < 0 || handle >= mList.size() || mList[handle] == NULL)
			return;

		T* resource = mList[handle];
		mHandles.push(handle);
		delete resource;
		mList[handle] = NULL;
	}

	Handle Add(const std::string& filename, typename T::raw_type* raw = NULL)
	{
		if (filename.empty())
		{
			return -1;
		}

		T* element = GetElement(filename);
		if (element != NULL)
		{
			return element->GetHandle();
		}

		bool available = !mHandles.empty();
		unsigned int handle;
		if (available)
		{
			handle = mHandles.top();
			mHandles.pop();
		}
		else
			handle = mList.size();

		T* resource = NULL;
		std::cout << "[LOADING] " << filename << std::endl;
		if (raw == NULL)
		{
			bool loaded = Loader::Load(&resource, handle, filename);
			if (loaded)
				std::cout << "[LOADED] " << filename << std::endl;
			else
				std::cout << "[ERROR] Failed to load " << filename << std::endl;
		}
		else
		{
			resource = new T(handle, filename);
			resource->mRaw = raw;
		}

		assert(resource != NULL && "Failed to load resource");

		if (available)
			mList[handle] = resource;
		else
			mList.push_back(resource);

		return handle;
	}

	T* operator[](Handle handle)
	{
		if (handle < mList.size() && handle >= 0)
			return mList[handle];
		return NULL;
	}

private:
	std::stack<Handle> mHandles;
	std::vector<T*> mList;
};

#endif // MANAGER_H_
