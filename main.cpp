#include "JsonReader.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <exception>
#include <filesystem>
#include <chrono>

int main(int argc, const char* const argv[])
{
    try
    {
        if (argc != 2 || !std::filesystem::exists(argv[1]))
        {
            throw std::invalid_argument("input file path");
        }

        JsonReader reader;
        std::unordered_map<std::string, uint32_t> property_count_map;
        property_count_map.reserve(9000); // Wikidata has ~8800 properties

        reader.onObjectBegin("claims", [&]()
        {
            reader.onPair("property", [&](const char* property_name)
            {
                ++property_count_map[property_name];
            });
        });

        std::function<void(const char*)> no_callback;
        reader.onObjectEnd("claims", [&]()
        {
            reader.onPair("property", no_callback);
        });

        using clock = std::chrono::high_resolution_clock;
        auto start_time = clock::now();
        reader.readFile(argv[1]);
        std::cout << "duration: " << std::chrono::duration_cast<std::chrono::seconds>(clock::now() - start_time).count() << " s\n\n";

        struct property_count_data: public std::pair<const std::string*, uint32_t>
        {
            using parent = std::pair<const std::string*, uint32_t>;
            using parent::parent;

            bool operator < (const property_count_data& other) const noexcept
            {
                return this->second < other.second;
            }
        };

        std::vector<property_count_data> data;
        data.reserve(property_count_map.size());
        for(const auto& item : property_count_map)
        {
            data.emplace_back(property_count_data{&item.first, item.second});
        }

        std::sort(data.rbegin(), data.rend());
        for (const auto& item : data)
        {
            std::cout << *item.first << ';' << item.second << std::endl;
        }
    }
    catch(const std::exception& ex)
    {
        std::cout << "error: " << ex.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cout << "error: weird exception\n";
        return -2;
    }

    return 0;
}
