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

#ifndef __OPENSPACE_MODULE_VOLUME___VOLUMETRANSFERFUNCTIONHANDLER___H__
#define __OPENSPACE_MODULE_VOLUME___VOLUMETRANSFERFUNCTIONHANDLER___H__

#include <modules/volume/transferfunction/transferfunctionhandler.h>
#include <openspace/properties/propertyowner.h>
#include <openspace/properties/triggerproperty.h>
#include <openspace/properties/stringproperty.h>
#include <openspace/properties/binaryproperty.h>
#include <openspace/properties/scalarproperty.h>
#include <openspace/util/histogram.h>
#include <modules/volume/transferfunction/transferfunctionproperty.h>
#include <modules/volume/transferfunction/transferfunction.h>

namespace openspace::volume {

/**
 * Handler class for managing a transfer function for a
 * renderable volume that uses lua formatted envelopes
 * and a transfer function property, made for the
 * editable transfer function GUI.
 * 
 * This class makes use of the transferfunction implementation
 * here in the volume module, as opposed to the implementation
 * in the core package.
 */
class VolumeTransferFunctionHandler : public TransferFunctionHandler {
public:
    VolumeTransferFunctionHandler();
    void initialize();

    void setFilepath(const std::string& path);

    void setTexture(); 
    void uploadTexture();
    bool hasTexture();
    ghoul::opengl::Texture& getTexture();

    void loadStoredEnvelopes();
    void saveEnvelopes();

    void setHistogramProperty(std::shared_ptr<openspace::Histogram> histogram);
    void setUnit(const std::string& unit);
    void setMinAndMaxValue(const float& min, const float& max);

    void bind();

private:
    properties::StringProperty _transferFunctionPath;
    properties::StringProperty _dataUnit;
    properties::StringProperty _minValue;
    properties::StringProperty _maxValue;
    properties::TriggerProperty _saveTransferFunction;
    properties::TransferFunctionProperty _transferFunctionProperty;
    properties::BinaryProperty _histogramProperty;
}

#endif // __OPENSPACE_MODULE_VOLUME___VOLUMETRANSFERFUNCTIONHANDLER___H__
