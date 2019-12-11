#include <experimental/filesystem>

#include "tags/parse_tag_csv.hpp"

osmium::TagsFilter Tags::Parse(const std::experimental::filesystem::path &path_to_exclude_csv) {
    std::vector<std::string> tag_list;

    std::fstream exclude_csv(path_to_exclude_csv, std::ios_base::in);

    if (!exclude_csv.is_open()) {
        std::cerr << "I cannot open the file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (std::string temp; std::getline(exclude_csv, temp);) {
        tag_list.push_back(std::move(temp));
    }

    osmium::TagsFilter filter{false};
    const std::regex tag_regex("(.+)=(.+)");
    std::smatch pieces_match;
    for (const std::string &tag : tag_list) {
        if (std::regex_match(tag, pieces_match, tag_regex)) {
            auto key = pieces_match[1].str();
            auto value = pieces_match[2].str();
            if (value == "*") {
                filter.add_rule(true, key);
            } else {
                filter.add_rule(true, key, value);
            }
        }
    }
    return filter;
}
