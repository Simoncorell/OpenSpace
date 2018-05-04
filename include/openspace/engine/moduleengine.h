/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014-2018                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __OPENSPACE_CORE___MODULEENGINE___H__
#define __OPENSPACE_CORE___MODULEENGINE___H__

#include <openspace/properties/propertyowner.h>

#include <map>
#include <memory>
#include <vector>

namespace ghoul { class Dictionary; }
namespace ghoul::systemcapabilities { struct Version; }

namespace openspace {

class OpenSpaceModule;

namespace scripting { struct LuaLibrary; }

/**
 * The ModuleEngine is the central repository for registering and accessing
 * OpenSpaceModule for the current application run. By initializing (#initialize) the
 * ModuleEngine, the default set of OpenSpaceModule%s as generated by CMake in the
 * <code>moduleregistration.h</code> file is automatically registered and created.
 * Additional OpenSpaceModule%s can be registered with the #registerModule function, which
 * will internally call the OpenSpaceModule::initialize method.
 */
class ModuleEngine : public properties::PropertyOwner {
public:
    ModuleEngine();

    /**
     * Registers all of the OpenSpaceModule%s which are created by the CMake configuration
     * and stored in the <code>moduleregistration.h</code> file. For all of those modules
     * the OpenSpaceModule::initialize method with will called.
     * \throw ghoul::RuntimeError If two modules in the default modules have the same
     * name
    */
    void initialize(const std::map<std::string, ghoul::Dictionary>& moduleConfigurations);

    /**
     * Calls the initializeGL functions of all registered OpenSpaceModule%s.
     */
    void initializeGL();

    /**
     * Deinitializes all of the contained OpenSpaceModule%s by calling the
     * OpenSpaceModule::deinitialize methods.
     */
    void deinitialize();

    /**
     * Calls the deinitializeGL functions of all registered OpenSpaceModule%s.
     */
    void deinitializeGL();

    /**
     * Registers the passed \p module with this ModuleEngine. The OpenSpaceModule::create
     * method will be called on the \p module in the process.
     * \param module The OpenSpaceModule that is to be registered
     * \throw ghoul::RuntimeError If the name of the \p module was already registered
     * previously
     * \pre \p module must not be nullptr
     */
    void registerModule(std::unique_ptr<OpenSpaceModule> module,
        const ghoul::Dictionary& configuration);

    /**
     * Returns a list of all registered OpenSpaceModule%s that have been registered with
     * this ModuleEngine. All returned OpenSpaceModule%s are guaranteed to be initialized.
     * \return A list of all registered OpenSpaceModule%s
     */
    std::vector<OpenSpaceModule*> modules() const;

    /**
     * Get the module subclass with given template argument. Requires the module subclass
     * to have the public static member variable <code>name</code> which must be equal to
     * the name of the module used in its constructor.
     * \return a pointer to the module of the given subclass
     */
    template <class ModuleSubClass>
    ModuleSubClass* module() const {
        auto it = std::find_if(_modules.begin(), _modules.end(),
            [](const std::unique_ptr<OpenSpaceModule>& m) {
                return m->identifier() == ModuleSubClass::Name;
            });
        if (it != _modules.end()) {
            return dynamic_cast<ModuleSubClass*>(it->get());
        } else {
            return nullptr;
        }
    }

    /**
     * Returns the combined minimum OpenGL version. The return value is the maximum
     * version of all registered modules' OpenGL versions.
     * \return The combined minimum OpenGL version
     */
    ghoul::systemcapabilities::Version requiredOpenGLVersion() const;

    /**
    * Returns the Lua library that contains all Lua functions available to affect the
    * modules.
    */
    static scripting::LuaLibrary luaLibrary();

private:
    /// The list of all registered OpenSpaceModule%s
    std::vector<std::unique_ptr<OpenSpaceModule>> _modules;
};

} // namespace openspace

#endif // __OPENSPACE_CORE___MODULEENGINE___H__
