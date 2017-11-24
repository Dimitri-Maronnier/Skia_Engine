#ifndef ASSET_H
#define ASSET_H
#include <string>
#include "src/Assets/assetsmanager.h"


//-------------------------------------------------------------------------
// The Asset class.
//-------------------------------------------------------------------------
class Asset
{
protected:
    std::string	m_name;
    std::string	m_path;
    unsigned int m_handle;
    unsigned long m_refCount;

public:

    //-------------------------------------------------------------------------
    // The Asset class constructor.
    //-------------------------------------------------------------------------
    Asset( const unsigned int handle, const std::string& name, const std::string& path = "./" )
    {
        // Store the name.
        if( !name.empty() )
            m_name = name;

        // Store the path.
        if( !path.empty() )
            m_path = path;

        //Store the handle
        m_handle = handle;


        // Start the reference count.
        m_refCount = 1;
    }

    //-------------------------------------------------------------------------
    // The asset class destructor.
    //-------------------------------------------------------------------------
    virtual ~Asset()
    {
    }

    //-------------------------------------------------------------------------
    // Returns the name of the asset.
    //-------------------------------------------------------------------------
    std::string GetName()
    {
        return m_name;
    }

    //-------------------------------------------------------------------------
    // Returns the path to the asset.
    //-------------------------------------------------------------------------
    std::string GetPath()
    {
        return m_path;
    }


    //-------------------------------------------------------------------------
    // Returns the handle of the asset.
    //-------------------------------------------------------------------------
    unsigned int GetHandle()
    {
        return m_handle;
    }

    //-------------------------------------------------------------------------
    // Increments the asset's reference count.
    //-------------------------------------------------------------------------
    void IncRef()
    {
        m_refCount++;
    }

    //-------------------------------------------------------------------------
    // Decrements the asset's reference count.
    //-------------------------------------------------------------------------
    void DecRef()
    {
        m_refCount--;
    }

    //-------------------------------------------------------------------------
    // Returns the asset's reference count.
    //-------------------------------------------------------------------------
    unsigned long GetRefCount()
    {
        return m_refCount;
    }

};

#endif // ASSET_H
