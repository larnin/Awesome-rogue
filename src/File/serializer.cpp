#include "serializer.h"
#include "Libs/json.hpp"
#include "serializable.h"
#include "Events/event.h"
#include "Events/Datas/File/eventitemloaded.h"
#include "Events/Datas/File/eventloadfinished.h"
#include "Map/room.h"
#include "Items/itemslist.h"
#include "Entities/playerinfos.h"
#include "Entities/Types/entity.h"
#include "Entities/Types/circlemob.h"
#include "Entities/Types/crosslasermob.h"
#include "Entities/Types/player.h"
#include "Entities/Types/punchballmob.h"
#include "Entities/Types/squaremob.h"
#include "Entities/Types/trackermob.h"
#include <boost/filesystem.hpp>
#include <cassert>

using json = nlohmann::json;

void createAndEmitObject(SerializableType type, const json &j);

void load(const std::string & filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
        return;
    json j(json::parse(file));
    file.close();

    for(const auto & jItem : j)
    {
        SerializableType type(fromJson(jItem));
        createAndEmitObject(type, jItem);
    }

    Event<EventLoadFinished>::send(EventLoadFinished());
}

void save(const std::string & filename)
{
    using json = nlohmann::json;

    json j(Serializable::serializeAll());

    std::ofstream file(filename);
    if(!file.is_open())
        return;
    file << j.dump();
    file.close();
}

void remove(const std::string & filename)
{
    namespace fs = boost::filesystem;
    fs::remove(filename);
}

std::vector<std::string> filesIn(const std::string & dir, const std::string & extension)
{
    namespace fs = boost::filesystem;
    std::vector<std::string> fileList;

    if(dir.empty() || !fs::exists(dir))
        return fileList;

    fs::path apkPath(dir);
    fs::recursive_directory_iterator end;

    for (fs::recursive_directory_iterator i(apkPath); i != end; ++i)
    {
        const fs::path cp = (*i);
        std::string str(cp.string());
        auto pos(str.find_last_of('.'));
        if(pos == std::string::npos)
            continue;
        if(str.substr(pos+1) == extension)
            fileList.push_back(str);
    }
    return fileList;
}

std::vector<std::string> saveList()
{
    return filesIn(saveDir, "json");
}

void createAndEmitObject(SerializableType type, const json &j)
{
    std::shared_ptr<Serializable> item;

    auto it(j.find("data"));
    if(it == j.end())
        throw std::runtime_error("Can't find the data on json !");

    switch(type)
    {
    case SERIALIZE_ROOM:
        Event<EventItemLoaded<Room>>::send(EventItemLoaded<Room>(std::make_shared<Room>(*it)));
        break;
    case SERIALIZE_ITEM_LIST:
        Event<EventItemLoaded<ItemsList>>::send(EventItemLoaded<ItemsList>(std::make_shared<ItemsList>(*it)));
        break;
    case SERIALIZE_PLAYERINFOS:
        Event<EventItemLoaded<PlayerInfos>>::send(EventItemLoaded<PlayerInfos>(std::make_shared<PlayerInfos>(*it)));
        break;
    case SERIALIZE_CIRCLEMOB:
        Event<EventItemLoaded<Entity>>::send(EventItemLoaded<Entity>(std::make_shared<CircleMob>(*it)));
        break;
    case SERIALIZE_CROSSLASERMOB:
        Event<EventItemLoaded<Entity>>::send(EventItemLoaded<Entity>(std::make_shared<CrossLaserMob>(*it)));
        break;
    case SERIALIZE_PLAYER:
        {
            auto player = std::make_shared<Player>(*it);
            Event<EventItemLoaded<Entity>>::send(EventItemLoaded<Entity>(player));
            Event<EventItemLoaded<Player>>::send(EventItemLoaded<Player>(player));
        }
        break;
    case SERIALIZE_PUNCHBALLMOB:
        Event<EventItemLoaded<Entity>>::send(EventItemLoaded<Entity>(std::make_shared<PunchBallMob>(*it)));
        break;
    case SERIALIZE_SQUAREMOB:
        Event<EventItemLoaded<Entity>>::send(EventItemLoaded<Entity>(std::make_shared<SquareMob>(*it)));
        break;
    case SERIALIZE_TRACKERMOB:
        Event<EventItemLoaded<Entity>>::send(EventItemLoaded<Entity>(std::make_shared<TrackerMob>(*it)));
        break;
    default:
        assert(false);
    break;
    }
}

std::string filenameFromDir(std::string dir)
{
    auto pos(dir.find_last_of("/\\"));
    if(pos != std::string::npos)
        dir = dir.substr(pos+1);
    pos = dir.find_first_of('.');
    if(pos != std::string::npos)
        return dir.substr(0, pos);
    return dir;
}

bool isValidFilename(const std::string & filename)
{
    std::string invalidChar("/\\:*\"<>|");
    return filename.find_first_of(invalidChar) == std::string::npos;
}
