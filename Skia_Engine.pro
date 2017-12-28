QT       += core gui
QT       += opengl


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = Skia_Engine
TEMPLATE = app


INCLUDEPATH += $$PWD/external/include
LIBS += -L$$PWD/external/lib -lboost_regex-vc140-mt-1_64 -lboost_system-vc140-mt-1_64 -lboost_iostreams-vc140-mt-1_64 -lboost_filesystem-vc140-mt-1_64 -lboost_regex-vc140-mt-gd-1_64 -lboost_system-vc140-mt-gd-1_64 -lboost_iostreams-vc140-mt-gd-1_64 -lboost_filesystem-vc140-mt-gd-1_64
LIBS += -lglew32 -lfreeglut -L$$PWD/external/bin -L$$PWD/external/lib
LIBS += -L$$PWD/external/bin -L$$PWD/external/lib -lsoil2 -lassimp

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/glwidget.cpp \
    src/Engine/Shaders/shaderprogram.cpp \
    src/Engine/Shaders/shader.cpp \
    src/Engine/Utils/matrix.cpp \
    src/utils.cpp \
    src/Engine/Entities/camerathird.cpp \
    src/Engine/Entities/camera.cpp \
    src/Engine/Entities/camerafirst.cpp \
    src/Loaders/loader.cpp \
    src/Engine/Models/mesh.cpp \
    src/Engine/Entities/light.cpp \
    src/Loaders/imgloader.cpp \
    src/Loaders/vertex.cpp \
    src/Engine/Models/model.cpp \
    src/Engine/Materials/material.cpp \
    src/Editor/newprojectdialog.cpp \
    src/Editor/Utils/foldergestion.cpp \
    src/Editor/Utils/projectinfo.cpp \
    src/Loaders/objloader.cpp \
    src/Editor/ctreewidgetitem.cpp \
    src/Editor/contentbrowser.cpp \
    src/Engine/scene.cpp \
    src/Engine/Entities/entity.cpp \
    src/Engine/Entities/object3dstatic.cpp \
    src/Editor/detailsarea.cpp \
    src/Editor/Materials/materialeditorwindow.cpp \
    src/Editor/Materials/materialgraphwidget.cpp \
    src/qnodeseditor/qneblock.cpp \
    src/qnodeseditor/qneconnection.cpp \
    src/qnodeseditor/qneport.cpp \
    src/qnodeseditor/qnodeseditor.cpp \
    src/Editor/Materials/Nodes/nodesampler2d.cpp \
    src/Editor/Materials/Nodes/nodebase.cpp \
    src/Editor/Materials/Nodes/nodematerial.cpp \
    src/Editor/Materials/listfunctionsview.cpp \
    src/Editor/Materials/glsltreewidgetitem.cpp \
    src/Editor/Materials/customlabeltype.cpp \
    src/Engine/Shaders/equirectangulartocubemapshader.cpp \
    src/Engine/Shaders/simpleskyboxshader.cpp \
    src/Engine/Shaders/PBR/irradianceconvolutionshader.cpp \
    src/Engine/Shaders/PBR/prefiltershader.cpp \
    src/Engine/Shaders/PBR/brdfshader.cpp \
    src/Editor/imageeditor.cpp \
    src/Editor/Utils/compressor.cpp \
    src/Engine/Materials/texture.cpp \
    src/Assets/assetsmanager.cpp \
    src/Assets/asset.cpp \
    src/Assets/assetscollections.cpp \
    src/Editor/Objects3dStatic/object3dstaticeditor.cpp \
    src/Editor/Materials/glmaterialpreview.cpp \
    src/Editor/globjectpreview.cpp \
    src/Engine/Shaders/PBR/deferredshader.cpp \
    src/Engine/Shadows/shadowmap.cpp \
    src/Engine/Utils/rendertools.cpp \
    src/Editor/scenetree.cpp \
    src/Engine/Shaders/pivotshader.cpp \
    src/Engine/Utils/smath.cpp \
    src/Engine/Entities/actor.cpp \
    src/Engine/Shadows/shadowpassshader.cpp \
    src/Editor/newactordialog.cpp \
    src/Engine/Controller/inputhandler.cpp \
    src/Game/game.cpp \
    src/Game/mainperso.cpp \
    src/Engine/Entities/springcamera.cpp \
    src/Engine/Controller/inputabstract.cpp \
    src/Game/projectile.cpp \
    src/Engine/Entities/collision.cpp

HEADERS += \
        src/mainwindow.h \
    src/glwidget.h \
    src/Engine/Shaders/shaderprogram.h \
    src/Engine/Shaders/shader.h \
    src/Engine/Utils/matrix.h \
    src/utils.h \
    src/Engine/Entities/camerathird.h \
    src/Engine/Entities/camera.h \
    src/Engine/Entities/camerafirst.h \
    src/Loaders/loader.h \
    src/Engine/Models/mesh.h \
    src/Engine/Entities/light.h \
    src/Loaders/imgloader.h \
    src/Loaders/vertex.h \
    src/Engine/Models/model.h \
    src/Engine/Materials/material.h \
    src/Editor/newprojectdialog.h \
    src/Editor/Utils/foldergestion.h \
    src/Editor/Utils/projectinfo.h \
    src/Loaders/objloader.h \
    src/Editor/ctreewidgetitem.h \
    src/Editor/contentbrowser.h \
    src/Engine/scene.h \
    src/Engine/Entities/entity.h \
    src/Engine/Entities/object3dstatic.h \
    src/Editor/detailsarea.h \
    src/Editor/Materials/materialeditorwindow.h \
    src/Editor/Materials/materialgraphwidget.h \
    src/qnodeseditor/qneblock.h \
    src/qnodeseditor/qneconnection.h \
    src/qnodeseditor/qneport.h \
    src/qnodeseditor/qnodeseditor.h \
    src/Editor/Materials/Nodes/nodesampler2d.h \
    src/Editor/Materials/Nodes/nodebase.h \
    src/Editor/Materials/Nodes/nodematerial.h \
    src/Editor/Materials/listfunctionsview.h \
    src/Editor/Materials/glsltreewidgetitem.h \
    src/Editor/Materials/customlabeltype.h \
    src/Engine/Shaders/equirectangulartocubemapshader.h \
    src/Engine/Shaders/simpleskyboxshader.h \
    src/Engine/Shaders/PBR/irradianceconvolutionshader.h \
    src/Engine/Shaders/PBR/prefiltershader.h \
    src/Engine/Shaders/PBR/brdfshader.h \
    src/Editor/imageeditor.h \
    src/Editor/Utils/compressor.h \
    src/Engine/Materials/texture.h \
    src/define.h \
    src/Assets/assetsmanager.h \
    src/Assets/asset.h \
    src/Assets/assetscollections.h \
    src/Editor/Objects3dStatic/object3dstaticeditor.h \
    src/Editor/Materials/glmaterialpreview.h \
    src/Editor/globjectpreview.h \
    src/Engine/Shaders/PBR/deferredshader.h \
    src/Engine/Shadows/shadowmap.h \
    src/Engine/Utils/rendertools.h \
    src/Editor/scenetree.h \
    src/Engine/Shaders/pivotshader.h \
    src/Engine/Utils/smath.h \
    src/Engine/Entities/actor.h \
    src/Engine/Shadows/shadowpassshader.h \
    src/Editor/newactordialog.h \
    src/Engine/Controller/inputhandler.h \
    src/Game/game.h \
    src/Game/mainperso.h \
    src/Engine/Entities/springcamera.h \
    src/Engine/Controller/inputabstract.h \
    src/Game/projectile.h \
    src/Engine/Entities/collision.h
FORMS += \
        src/mainwindow.ui \
    src/Editor/newprojectdialog.ui \
    src/Editor/Materials/materialeditorwindow.ui \
    src/Editor/imageeditor.ui \
    src/Editor/Objects3dStatic/object3dstaticeditor.ui \
    src/Editor/newactordialog.ui


RESOURCES += \
    ressources.qrc
