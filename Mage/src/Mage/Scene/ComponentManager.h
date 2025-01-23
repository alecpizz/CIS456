#pragma once

#include "../Core/Core.h"
#include "Entity.h"
#include "ComponentVector.h"

namespace Mage {

    class SystemManager ;

    // Unlike ComponentVector, ComponentManager IS marked MAGE_API.
    // This is because this is a regular class with member function templates.
    // This means this class exists in the DLL, but additional member function template
    //  instantiations can be created for it in the client executable.
    // Doing it this way allows the Application object to "own" the ComponentManager
    //  as it does the other *Manager classes.

}
