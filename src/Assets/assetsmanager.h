/*
ResourceManagement.hpp

A template based simple and fast resource manager using STL and a base resource class to be used with the manager.

Copyright (c) 2008 by Ashic Mahtab and Zinat Wali.
Permission is granted to use, distribute, or modify this source, provided that this copyright notice remains intact.

The code is provided "as is" with no guarantees.
It has been tested in Microsoft Visual Studio 2008 running under Windows Vista Ultimate x64.

Feel free to contact us at:
aheartattack@gmail.com (Ashic)
zinat.wali@gmail.com (Zinat)

Happy coding :)
*/

#ifndef ASSETSMANAGER_H
#define ASSETSMANAGER_H

#include "src/define.h"
#include <QString>

#include <string>
#include <vector>
#include <stack>
#include <boost/filesystem.hpp>



//-----------------------------------------------------------------------------
// Assets Manager Class
//-----------------------------------------------------------------------------
template <class T>
class AssetsManager
{
private:
    std::stack<unsigned int> m_handles;
    std::vector<T*> *m_list;	//List of pointers to assets
    void (*Createasset)( T **asset, const unsigned int handle, const std::string& name, const std::string& path ); // Application specific asset creation.

public:

    //-------------------------------------------------------------------------
    // The asset manager class constructor.
    //-------------------------------------------------------------------------
    AssetsManager( void (*CreateassetFunction)( T **asset,
        const unsigned int handle, const std::string& name, const std::string& path ) = NULL )
    {
        m_list = new std::vector<T*>;

        Createasset = CreateassetFunction;
    }

    //-------------------------------------------------------------------------
    // The asset manager class destructor.
    //-------------------------------------------------------------------------
    ~AssetsManager()
    {
        EmptyList();
        delete m_list;
    }

    //-------------------------------------------------------------------------
    // The asset manager cleanUp.
    //-------------------------------------------------------------------------
    void cleanUp()
    {
        EmptyList();
        delete m_list;
    }


    //-------------------------------------------------------------------------
    // Returns a pointer to the vector of assets.
    //-------------------------------------------------------------------------
    std::vector<T*> *GetList()
    {
        return m_list;
    }

    //-------------------------------------------------------------------------
    // Returns a asset by its filename.
    //-------------------------------------------------------------------------
    T* GetElement( const std::string& name, const std::string& path = "./" )
    {
        // Ensure the name and path are valid, and the list is valid and not empty.
        if( name.empty() || path.empty() || m_list == NULL || m_list->empty())
            return NULL;

        //// Iterate the list looking for the specified asset.
        for(std::vector<T*>::iterator i = m_list->begin();
            i != m_list->end(); i++)
        {
            if((*i) != NULL)
                if((*i)->GetName() == name)
                    if((*i)->GetPath() == path)
                        return (*i);
        }

        // Return NULL if the asset was not found.
        return NULL;
    }

    //-------------------------------------------------------------------------
    // Returns a asset by its path.
    //-------------------------------------------------------------------------
    T* GetElementByPath(const std::string& path = "./" )
    {
        // Ensure the name and path are valid, and the list is valid and not empty.
        if(path.empty() || m_list == NULL || m_list->empty())
            return NULL;

        //// Iterate the list looking for the specified asset.
        for(std::vector<T*>::iterator i = m_list->begin();
            i != m_list->end(); i++)
        {
            if((*i) != NULL)
                if((*i)->GetPath() == path)
                    return (*i);
        }

        // Return NULL if the asset was not found.
        return NULL;
    }

    //-------------------------------------------------------------------------
    // Returns a asset by its handle.
    //-------------------------------------------------------------------------
    T *GetElement( const int handle )
    {
        if(handle < m_list->size() && handle >= 0)
            return (*m_list)[handle];

        return NULL;
    }

    //-------------------------------------------------------------------------
    // Clears the asset list.
    //-------------------------------------------------------------------------
    void EmptyList()
    {
        for(std::vector<T*>::iterator i = m_list->begin();
            i != m_list->end(); i++)
            SAFE_DELETE(*i);

        while(!m_handles.empty())
            m_handles.pop();

        //use vector swapping trick to release previously used memory.
        m_list->clear();
        m_list->swap(*m_list);
    }

    //-------------------------------------------------------------------------
    // Removes the given asset from the manager.
    //-------------------------------------------------------------------------
    void Remove( const unsigned int handle )
    {
        // Ensure the asset to be removed and the list is valid.
        if(handle < 0 || m_list == NULL || handle >= m_list->size() || (*m_list)[handle] == NULL)
            return;

        T * asset = (*m_list)[handle];

        // Decrement the asset's reference count.
        asset->DecRef();

        // If the asset is no longer being used then destroy it.
        if( asset->GetRefCount() == 0 )
        {
            //add the handle to the stack of free handles
            m_handles.push(handle);
            //delete the asset
            delete asset;
            //mark the slot as NULL
            (*m_list)[handle] = NULL;
        }
    }

    //-------------------------------------------------------------------------
    // Adds a new asset to the manager.
    //-------------------------------------------------------------------------
    unsigned int Add( const std::string& name, const std::string& path = "./" )
    {
        // Ensure the list, the asset name, and its path are valid.
        if( m_list == NULL || name.empty() || path.empty() )
            return -1;

        //Ensure File Exist
        boost::filesystem::path bPath = path;
        if(!boost::filesystem::exists(bPath))
            return -1;

        // If the element already exists, then return a pointer to it.
        T *element = GetElement( name, path );
        if( element != NULL )
        {
            element->IncRef();
            return element->GetHandle();
        }


        //Check if there is an available handle. If not, use new handle.
        bool handleAvailable = !m_handles.empty();
        unsigned int handle;
        if(handleAvailable)
        {
            handle = m_handles.top();
            m_handles.pop();
        }
        else handle = m_list->size();

        // Create the asset, preferably through the application specific
        // function if it is available.
        T *asset = NULL;
        if( Createasset != NULL )
            Createasset( &asset, handle, name, path );
        else
            asset = new T( handle, name, path );


        //Add the asset to the manager. If there is an available handle, then
        //we store the asset using the handle. Otherwise we add it to the vector.
        if(handleAvailable)
            (*m_list)[handle] = asset;
        else
            m_list->push_back(asset);

        //return the handle
        return handle;
    }

    //-------------------------------------------------------------------------
    // Overloaded [] operator for easier access
    //-------------------------------------------------------------------------
    T* operator [](unsigned int handle)
    {
        if(handle < m_list->size() && handle >= 0)
            return (*m_list)[handle];

        return NULL;
    }

};



#endif // ASSETSMANAGER_H
