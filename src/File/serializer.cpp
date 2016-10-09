#include "serializer.h"
#include "Libs/json.hpp"
#include "serializable.h"
#include "Events/event.h"
#include "Events/Datas/eventitemloaded.h"
#include "Map/room.h"
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

void createAndEmitObject(SerializableType type, const json &j)
{
    std::shared_ptr<Serializable> item;

    switch(type)
    {
    case SERIALIZE_ROOM:
        item = std::make_shared<Room>(j);
    break;
    default:
        assert(false);
    break;
    }

    if(item)
        Event<EventItemLoaded>::send(EventItemLoaded(type, item));
}
