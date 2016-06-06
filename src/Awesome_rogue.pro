#-------------------------------------------------
#
# Project created by QtCreator 2015-11-02T13:45:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Awesome_rogue
TEMPLATE = app


SOURCES += main.cpp \
    Controles/commands.cpp \
    Controles/keyconfig.cpp \
    Controles/commandsvalue.cpp \
    Systemes/controlable.cpp \
    Systemes/drawablelist.cpp \
    Systemes/updatable.cpp \
    Machine/statemachine.cpp \
    Machine/States/state.cpp \
    GUI/Widgets/widget.cpp \
    Utilities/quadrender.cpp \
    Map/block.cpp \
    Map/room.cpp \
    Map/location.cpp \
    Map/map.cpp \
    Map/Render/roomrender.cpp \
    Map/Render/worldrender.cpp \
    Map/Generator/generator.cpp \
    Map/Generator/patern.cpp \
    Map/blocktype.cpp \
    Collisions/hitbox.cpp \
    Collisions/collisions.cpp \
    Map/door.cpp \
    Entities/entitylist.cpp \
    Entities/Types/entity.cpp \
    Collisions/pathfinder.cpp \
    Entities/Types/player.cpp \
    Entities/Types/squaremob.cpp \
    Collisions/path.cpp \
    Entities/Types/trackermob.cpp \
    Events/event.cpp \
    Entities/Types/crosslasermob.cpp \
    Entities/Types/punchballmob.cpp \
    Entities/Types/circlemob.cpp \
    Entities/entityfactory.cpp \
    GUI/gameinterface.cpp \
    GUI/minimap.cpp \
    GUI/Widgets/Buttons/abstractbutton.cpp \
    GUI/Widgets/ComboBoxs/abstractcombobox.cpp \
    GUI/Widgets/Sliders/abstractslider.cpp \
    GUI/Widgets/Sliders/abstractquantifiedslider.cpp \
    GUI/Widgets/TextBoxs/abstracttextbox.cpp \
    GUI/Widgets/Togglable/abstracttogglable.cpp \
    GUI/Widgets/RadioButtons/abstactradiobuttonitem.cpp \
    GUI/Widgets/RadioButtons/radiobuttons.tpp \
    GUI/lifebar.cpp \
    Machine/States/testwidgetsstate.cpp \
    GUI/Widgets/graphichelper.cpp \
    GUI/Widgets/Buttons/basicbutton.cpp \
    GUI/Widgets/Sliders/basicslider.cpp \
    GUI/Widgets/Togglable/togglablebasicbutton.cpp \
    GUI/Widgets/TextBoxs/basictextbox.cpp \
    GUI/Widgets/Togglable/basiccheckbox.cpp \
    GUI/Widgets/ComboBoxs/basiccombobox.cpp \
    GUI/Widgets/Sliders/basicquantifiedslider.cpp \
    GUI/textDrawer.cpp \
    GUI/adaptablebounds.cpp \
    GUI/Widgets/RadioButtons/basicradiobuttonitem.cpp \
    Machine/States/mapteststate.cpp \
    Machine/States/menustate.cpp \
    Projectiles/Types/projectile.cpp \
    Projectiles/projectilelist.cpp \
    Projectiles/Types/delayedpunch.cpp

HEADERS  += \
    Controles/commands.h \
    Controles/commandsvalue.h \
    Controles/keyconfig.h \
    Systemes/controlable.h \
    Systemes/drawablelist.h \
    Systemes/staticlist.h \
    Systemes/updatable.h \
    Machine/statemachine.h \
    Machine/States/state.h \
    Utilities/extendstd.h \
    Utilities/noncopiable.h \
    GUI/Widgets/widget.h \
    Utilities/ressource.h \
    Utilities/vect2convert.h \
    Utilities/quadrender.h \
    Map/block.h \
    Utilities/matrix.h \
    Map/room.h \
    Map/door.h \
    Map/location.h \
    Map/map.h \
    Map/Render/roomrender.h \
    Map/Render/worldrender.h \
    Map/Generator/generator.h \
    Map/Generator/patern.h \
    Map/blocktype.h \
    Collisions/hitbox.h \
    Collisions/collisions.h \
    Map/orientation.h \
    Entities/entitylist.h \
    Entities/Types/entity.h \
    Collisions/pathfinder.h \
    Entities/Types/player.h \
    Events/event.h \
    Events/eventreceiver.h \
    Entities/Types/squaremob.h \
    Collisions/path.h \
    Entities/Types/trackermob.h \
    Entities/Types/crosslasermob.h \
    Entities/Types/punchballmob.h \
    Entities/Types/circlemob.h \
    Entities/entityfactory.h \
    Entities/entitytype.h \
    Events/Datas/evententitycreated.h \
    Events/Datas/eventcenterofviewchanged.h \
    Events/Datas/eventsizeviewchanged.h \
    GUI/gameinterface.h \
    GUI/minimap.h \
    GUI/Widgets/Buttons/abstractbutton.h \
    GUI/Widgets/ComboBoxs/abstractcombobox.h \
    GUI/Widgets/Sliders/abstractslider.h \
    GUI/Widgets/Sliders/abstractquantifiedslider.h \
    GUI/Widgets/TextBoxs/abstracttextbox.h \
    GUI/Widgets/Togglable/abstracttogglable.h \
    GUI/Widgets/RadioButtons/radiobuttons.h \
    GUI/Widgets/RadioButtons/abstactradiobuttonitem.h \
    GUI/lifebar.h \
    Machine/States/testwidgetsstate.h \
    GUI/Widgets/graphichelper.h \
    GUI/Widgets/Buttons/basicbutton.h \
    GUI/Widgets/Sliders/basicslider.h \
    GUI/Widgets/Togglable/togglablebasicbutton.h \
    GUI/Widgets/TextBoxs/basictextbox.h \
    GUI/Widgets/Togglable/basiccheckbox.h \
    GUI/Widgets/ComboBoxs/basiccombobox.h \
    GUI/Widgets/Sliders/basicquantifiedslider.h \
    GUI/textDrawer.h \
    GUI/adaptablebounds.h \
    GUI/margin.h \
    GUI/Widgets/RadioButtons/basicradiobuttonitem.h \
    Machine/States/mapteststate.h \
    Machine/States/menustate.h \
    Projectiles/Types/projectile.h \
    Projectiles/projectilelist.h \
    Events/Datas/eventprojectilecreated.h \
    Projectiles/projectilefactory.h \
    Events/eventgetter.h \
    Events/Datas/eventpreplayerchangeroom.h \
    Events/Datas/eventplayerchangeroom.h \
    Events/Datas/evententitychangeroom.h \
    Projectiles/Types/delayedpunch.h

DEFINES += SFML_STATIC

CONFIG += c++14
#QMAKE_CXXFLAGS += -std=c++11
#QMAKE_CXXFLAGS += -std=gnu++1y
#QMAKE_LFLAGS += -static-libgcc -static-libstdc++

LIBS += -LC:/Users/Nicolas/Programation/c++/SFML/DW2_2.3_(Qt)/lib

CONFIG(release, debug|release): LIBS += -lsfml-graphics-s \
                                        -lsfml-window-s \
                                        -lsfml-system-s \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm

CONFIG(debug, debug|release): LIBS +=   -lsfml-graphics-s-d \
                                        -lsfml-window-s-d \
                                        -lsfml-system-s-d \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm

INCLUDEPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.3_(Qt)/include
DEPENDPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.3_(Qt)/include