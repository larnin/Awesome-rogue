#-------------------------------------------------
#
# Project created by QtCreator 2015-11-02T13:45:04
#
#-------------------------------------------------

QT       -= core gui

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
    Machine/States/menustate.cpp \
    Projectiles/Types/projectile.cpp \
    Projectiles/projectilelist.cpp \
    Projectiles/Types/delayedpunch.cpp \
    Projectiles/Types/smallball.cpp \
    Projectiles/Types/rapidfire.cpp \
    Projectiles/Types/lasermob.cpp \
    Projectiles/Types/cacaura.cpp \
    Machine/smoothcamera.cpp \
    Machine/CameraEffects/effecttest.cpp \
    Particules/Types/particule.cpp \
    Particules/particulelist.cpp \
    Particules/Types/particulelifebar.cpp \
    Machine/listholder.cpp \
    Projectiles/ProjectileLauncher/projectilelauncher.cpp \
    Projectiles/Types/playerbullet.cpp \
    Projectiles/ProjectileLauncher/parallelebulletlauncher.cpp \
    Particules/Types/mobspawn.cpp \
    Entities/populator.cpp \
    Entities/entitytype.cpp \
    Particules/Types/mobdeath.cpp \
    GUI/interactor.cpp \
    GUI/LifeBar/bosslifebar.cpp \
    GUI/LifeBar/lifebar.cpp \
    Entities/Types/boss1part.cpp \
    Projectiles/Types/bigcircle.cpp \
    Machine/CameraEffects/shakeeffect.cpp \
    Machine/States/gamestate.cpp \
    Machine/gameholder.cpp \
    Particules/Types/spawnboss1.cpp \
    GUI/LifeBar/boss1partslifebar.cpp \
    Entities/Types/boss1end.cpp \
    Particules/Types/boss1transition.cpp \
    Utilities/animation.cpp \
    GUI/LifeBar/boss1finallifebar.cpp \
    GUI/showfps.cpp \
    Machine/States/pausestate.cpp \
    GUI/simplecontroler.cpp \
    Machine/States/optionsstate.cpp \
    Controles/keytexture.cpp \
    GUI/Widgets/keyconfigwidget.cpp \
    Controles/commandtype.cpp \
    GUI/Widgets/label.cpp \
    Utilities/configs.cpp \
    Machine/States/mapstate.cpp \
    GUI/globalmaprender.cpp \
    Utilities/delayedtask.cpp \
    Sounds/soundplayer.cpp \
    Items/itemslist.cpp \
    Items/itemtype.cpp \
    Entities/playerinfos.cpp \
    File/serializable.cpp \
    File/serializabletype.cpp \
    File/serializer.cpp \
    Sounds/Effects/fadeineffect.cpp \
    Sounds/Effects/fadeouteffect.cpp \
    Lights/lightrender.cpp \
    Lights/Types/circlecoloredlight.cpp \
    Map/Render/renderdata.cpp \
    Utilities/tr.cpp \
    Machine/States/loadstate.cpp

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
    Machine/States/menustate.h \
    Projectiles/Types/projectile.h \
    Projectiles/projectilelist.h \
    Events/Datas/eventprojectilecreated.h \
    Projectiles/projectilefactory.h \
    Events/eventgetter.h \
    Events/Datas/eventpreplayerchangeroom.h \
    Events/Datas/eventplayerchangeroom.h \
    Events/Datas/evententitychangeroom.h \
    Projectiles/Types/delayedpunch.h \
    Projectiles/Types/smallball.h \
    Projectiles/Types/rapidfire.h \
    Projectiles/Types/lasermob.h \
    Projectiles/Types/cacaura.h \
    Machine/smoothcamera.h \
    Events/Datas/eventinstantcenterofviewchanged.h \
    Events/Datas/eventplaycameraeffect.h \
    Machine/CameraEffects/cameraeffect.h \
    Machine/CameraEffects/effecttest.h \
    Events/Datas/eventclearcameraeffects.h \
    Particules/Types/particule.h \
    Particules/particulelist.h \
    Events/Datas/eventparticulecreated.h \
    Particules/particulefactory.h \
    Particules/Types/particulelifebar.h \
    Machine/listholder.h \
    Projectiles/ProjectileLauncher/projectilelauncher.h \
    Projectiles/Types/playerbullet.h \
    Projectiles/ProjectileLauncher/parallelebulletlauncher.h \
    Particules/Types/mobspawn.h \
    Entities/populator.h \
    Map/roomtype.h \
    Particules/Types/mobdeath.h \
    GUI/interactor.h \
    Events/Datas/eventinteraction.h \
    GUI/LifeBar/bosslifebar.h \
    GUI/LifeBar/lifebar.h \
    Utilities/delayedtask.h \
    Entities/Types/boss1part.h \
    Projectiles/Types/bigcircle.h \
    Controles/commandtype.h \
    GUI/aligntype.h \
    Map/blockinteractiontype.h \
    Machine/CameraEffects/cameraeffecttype.h \
    Map/boxtype.h \
    Map/rotation.h \
    Entities/team.h \
    Machine/CameraEffects/shakeeffect.h \
    Machine/States/gamestate.h \
    Machine/gameholder.h \
    Libs/json.hpp \
    Particules/Types/spawnboss1.h \
    Events/Datas/eventsetbosslifebar.h \
    Events/Datas/eventremoveentity.h \
    GUI/LifeBar/boss1partslifebar.h \
    Entities/Types/boss1end.h \
    Particules/Types/boss1transition.h \
    Utilities/animation.h \
    GUI/LifeBar/boss1finallifebar.h \
    GUI/showfps.h \
    Machine/States/pausestate.h \
    GUI/simplecontroler.h \
    Machine/States/optionsstate.h \
    Controles/keytexture.h \
    GUI/Widgets/keyconfigwidget.h \
    GUI/Widgets/label.h \
    Utilities/configs.h \
    Machine/States/mapstate.h \
    GUI/globalmaprender.h \
    Events/Datas/eventplaysound.h \
    Events/Datas/eventplaymusic.h \
    Sounds/soundplayer.h \
    Sounds/sounddata.h \
    Events/Datas/eventstopmusic.h \
    Items/itemslist.h \
    Items/itemtype.h \
    Events/Datas/eventpickitem.h \
    Events/Datas/eventdropitem.h \
    Entities/playerinfos.h \
    Events/Datas/eventexperiencechanged.h \
    Events/Datas/eventbosskilled.h \
    File/serializable.h \
    File/serializabletype.h \
    File/serializer.h \
    Events/Datas/eventitemloaded.h \
    Sounds/Effects/soundeffect.h \
    Sounds/Effects/fadeineffect.h \
    Sounds/Effects/fadeouteffect.h \
    Lights/lightrender.h \
    Lights/Types/light.h \
    Events/Datas/eventchangelightcolor.h \
    Events/Datas/eventaddlight.h \
    Events/Datas/eventdellight.h \
    Lights/Types/circlecoloredlight.h \
    Map/Render/renderdata.h \
    Map/Render/blockanimation.h \
    Utilities/tr.h \
    Machine/States/loadstate.h \
    Events/Datas/eventloadfinished.h

CONFIG += c++14
#QMAKE_CXXFLAGS += -Wsign-conversion

# SFML
DEFINES += SFML_STATIC
#   --- HOME
LIBS += -LC:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/lib
INCLUDEPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/include
DEPENDPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/include

#   --- RUBIKA
#LIBS += -LC:/Users/n.laurent/Desktop/perso/SFML/SFML-2.4.0-SW2/lib
#INCLUDEPATH += C:/Users/n.laurent/Desktop/perso/SFML/SFML-2.4.0-SW2/include
#DEPENDPATH += C:/Users/n.laurent/Desktop/perso/SFML/SFML-2.4.0-SW2/include

CONFIG(release, debug|release): LIBS += -lsfml-graphics-s \
                                        -lsfml-window-s \
                                        -lsfml-audio-s \
                                        -lsfml-system-s \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm \
                                        -lopenal32 \
                                        -lflac \
                                        -lvorbisenc \
                                        -lvorbisfile \
                                        -lvorbis \
                                        -logg

CONFIG(debug, debug|release): LIBS +=   -lsfml-graphics-s-d \
                                        -lsfml-window-s-d \
                                        -lsfml-audio-s-d \
                                        -lsfml-system-s-d \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm \
                                        -lopenal32 \
                                        -lflac \
                                        -lvorbisenc \
                                        -lvorbisfile \
                                        -lvorbis \
                                        -logg

#Boost
#   --- HOME
LIBS += -LC:/Users/Nicolas/Programation/c++/boost/boost_lib/out
INCLUDEPATH += C:/Users/Nicolas/Programation/c++/boost/boost_1_61_0
CONFIG(release, debug|release): LIBS += -lboost_filesystem-mgw53-mt-1_61 \
                                        -lboost_system-mgw53-mt-1_61
CONFIG(debug, debug|release): LIBS +=   -lboost_filesystem-mgw53-mt-d-1_61 \
                                        -lboost_system-mgw53-mt-d-1_61

#   --- RUBIKA
#LIBS += -LC:/Users/n.laurent/Desktop/perso/boost_1_62_0/boost_release/out
#INCLUDEPATH += C:/Users/n.laurent/Desktop/perso/boost_1_62_0
#CONFIG(release, debug|release): LIBS += -lboost_filesystem-mgw53-mt-1_62 \
#                                        -lboost_system-mgw53-mt-1_62
#CONFIG(debug, debug|release): LIBS +=   -lboost_filesystem-mgw53-mt-d-1_62 \
#                                        -lboost_system-mgw53-mt-d-1_62
