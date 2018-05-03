/*****************************************************************************************
 *                                                                                       *
 * openspace                                                                             *
 *                                                                                       *
 * copyright (c) 2014-2018                                                               *
 *                                                                                       *
 * permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "software"), to deal in the software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the software, and to    *
 * permit persons to whom the software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * the above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the software.                                              *
 *                                                                                       *
 * the software is provided "as is", without warranty of any kind, express or implied,   *
 * including but not limited to the warranties of merchantability, fitness for a         *
 * particular purpose and noninfringement. in no event shall the authors or copyright    *
 * holders be liable for any claim, damages or other liability, whether in an action of  *
 * contract, tort or otherwise, arising from, out of or in connection with the software  *
 * or the use or other dealings in the software.                                         *
 ****************************************************************************************/

#include <modules/volume/transferfunction/volumetransferfunctionhandler.h>
#include <modules/volume/transferfunction/transferfunctionproperty.h>
#include <ghoul/misc/dictionary.h>
#include <openspace/properties/scalarproperty.h>
#include <iostream>
#include <fstream>

#include <ghoul/opengl/ghoul_gl.h>
#include <ghoul/filesystem/filesystem.h>

namespace openspace::volume {

static const openspace::properties::Property::PropertyInfo TransferFunctionInfo = {
    "TransferFunction",
    "TransferFunction",
    "All the envelopes used in the transfer function"
};

static const openspace::properties::Property::PropertyInfo HistogramInfo = {
    "Histogram",
    "Histogram",
    "All the data"
};

static const openspace::properties::Property::PropertyInfo DataUnitInfo = {
    "DataUnit",
    "DataUnit",
    "Unit of the data"
};

static const openspace::properties::Property::PropertyInfo MinValueInfo = {
    "MinValue",
    "MinValue",
    "Minimum value in the data"
};

static const openspace::properties::Property::PropertyInfo MaxValueInfo = {
    "MaxValue",
    "MaxValue",
    "Maximum value in the data"
};

static const openspace::properties::Property::PropertyInfo SaveTransferFunctionInfo = {
    "SaveTransferFunction",
    "Save Transfer Function",
    "Save your transfer function"
};

// auto?
ghoul::opengl::Texture::FilterMode filtermode = ghoul::opengl::Texture::FilterMode::Linear;
ghoul::opengl::Texture::WrappingMode wrappingmode = ghoul::opengl::Texture::WrappingMode::ClampToEdge;

VolumeTransferFunctionHandler::VolumeTransferFunctionHandler(const properties::StringProperty& prop)
    : TransferFunctionHandler(prop),
    _transferFunctionProperty(TransferFunctionInfo),
    _dataUnit(DataUnitInfo),
    _minValue(MinValueInfo),
    _maxValue(MaxValueInfo),
    _histogramProperty(HistogramInfo),
    _saveTransferFunction(SaveTransferFunctionInfo)
{}

void VolumeTransferFunctionHandler::initialize() {
    addproperty(_transferfunctionpath);
    addproperty(_transferfunctionproperty);
    addproperty(_histogramproperty);
    addproperty(_dataunit);
    addproperty(_minvalue);
    addproperty(_maxvalue);
    addproperty(_savetransferfunction);

    this->addtag("tf");
    _texture = std::make_shared<ghoul::opengl::texture>(
        glm::size3_t(1024, 1, 1), 
        ghoul::opengl::texture::format::rgba,
        gl_rgba, 
        gl_float, 
        filtermode, 
        wrappingmode
    );

    if (_filepath != "") {
        loadstoredenvelopes();
    }

    _transferfunctionproperty.onchange([this]() {
        settexture();
    });

    _savetransferfunction.onchange([this]() {
        saveenvelopes();
    });

}

void VolumeTransferFunctionHandler::setFilepath(const std::string& path) { 
    _filePath = path; 
}

void VolumeTransferFunctionHandler::setTexture() {
    if (_transferFunctionProperty.value().createTexture(_texture)) {
        uploadTexture();
    }
}

bool VolumeTransferFunctionHandler::hasTexture() {
    return _texture != nullptr;
}

void VolumeTransferFunctionHandler::uploadTexture() {
    _texture->uploadTexture();
}

void VolumeTransferFunctionHandler::sethistogramproperty(std::shared_ptr<openspace::histogram> histogram) {
    // histogram->print();
    _histogramProperty.setValue(histogram->getBinaryData());
}

void VolumeTransferFunctionHandler::setUnit(const std::string& unit) {
    _dataUnit.set(unit);
}

void VolumeTransferFunctionHandler::setMinAndMaxValue(const float& min, const float& max) {
    std::stringstream s_min;
    s_min << min;
    std::string s = s_min.str();
    _minValue.set(s);
    std::stringstream s_max;
    s_max << max;
    std::string t = s_max.str();
    _maxValue.set(t);
}

void VolumeTransferFunctionHandler::loadStoredEnvelopes() {
    TransferFunction tf;
    tf.loadEnvelopesFromFile(_filePath);
    if (tf.hasEnvelopes()) {
        _transferFunctionProperty.setValue(tf);
        setTexture();
    }
}

void VolumeTransferFunctionHandler::saveEnvelopes() {
    _transferFunctionProperty.value().saveEnvelopesToFile(_filePath);
}

void VolumeTransferFunctionHandler::bind() {
    _texture->bind();
}

ghoul::opengl::Texture& VolumeTransferFunctionHandler::getTexture() {
    return *_texture.get();
}

} // namespace openspace::volume