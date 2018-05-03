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

#ifndef __OPENSPACE_MODULE_VOLUME___TRANSFERFUNCTIONHANDLER___H__
#define __OPENSPACE_MODULE_VOLUME___TRANSFERFUNCTIONHANDLER___H__

#include <functional>
#include <ghoul/opengl/texture.h>
#include <ghoul/glm.h>
// #include <modules/volume/transferfunction/transferfunctionproperty.h>

#include <vector>
#include <memory>

namespace openspace::volume {

class Envelope;

class TransferFunctionHandler : public properties::PropertyOwner {
public:
    TransferFunctionHandler(const properties::StringProperty& prop)
        : properties::PropertyOwner({ "TransferFunctionHandler" })
    {}

    virtual void initialize() = 0;
    // void initialize();

    virtual void setFilepath(const std::string& path) = 0;

    virtual void setTexture() = 0; // do we need this
    // void uploadTexture();
    virtual bool hasTexture() = 0;

    virtual ghoul::opengl::Texture& getTexture() = 0;

    // how to give different types
    // virtual auto getTransferFunction() = 0;
    // virtual void getTransferFunction(pointer&) = 0;
    // bind() or bindTexture or bindTransferfunction?
    // std::shared_ptr<openspace::TransferFunction> getTransferFunction();

private:
    std::shared_ptr<ghoul::opengl::Texture> _texture = nullptr;

protected:
    std::string _filePath;
};

} //namespace openspace::volume

#endif // __OPENSPACE_MODULE_VOLUME___TRANSFERFUNCTIONHANDLER___H__
