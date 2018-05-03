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

#ifndef __OPENSPACE_MODULE_VOLUME___CORETRANSFERFUNCTIONHANDLER___H__
#define __OPENSPACE_MODULE_VOLUME___CORETRANSFERFUNCTIONHANDLER___H__

#include <modules/volume/transferfunction/transferfunctionhandler.h>
#include <openspace/rendering/transferfunction.h>
// #include <ghoul/opengl/texture.h>

namespace openspace::volume {

/**
 * Handler class for managing a transfer function for a
 * renderable volume that uses the implementation of a
 * transfer function from the core package.
 * 
 * This class is basically just an adapter for that class so 
 * that this and the counterpart VolumeTransferFunctionHandler 
 * are interchangeable. VolumeTransferFunctionHandler works 
 * with a different implementation of the transfer function
 * that makes use of things necesary for the new GUI.
 * 
 * Refactor away this sub class once the counterpart is 
 * fully implemented.
 */
class CoreTransferFunctionHandler : public TransferFunctionHandler {
public:
    CoreTransferFunctionHandler(const std::string& filepath)
        : TransferFunctionHandler(nullptr), // wont work
        _transferFunction(std::make_shared<openspace::TransferFunction>(filePath))
    {}

    void initialize() {
      _transferFunction->updateTexture();
    }

    void setFilepath(const std::string& path) {
        _transferFunction->setPath(path);
    }

    void setTexture(); // ?

    // void uploadTexture();

    bool hasTexture() {
        return _transferFunction->getTexture() != nullptr;
    }

    ghoul::opengl::Texture& getTexture() {
        return _transferFunction->getTexture();
    }

    // to be implemented
    // put in base class?
    void update() {
        _transferFunction->updateTexture();
    }

    void bind() {
        _transferFunction->bind();
    }

private:
    std::shared_ptr<openspace::TransferFunction> _transferFunction;
}

} // namespace openspace::volume

#endif // __OPENSPACE_MODULE_VOLUME___CORETRANSFERFUNCTIONHANDLER___H__
