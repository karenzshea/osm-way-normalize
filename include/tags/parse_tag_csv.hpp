#include <experimental/filesystem>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <osmium/tags/tags_filter.hpp>

namespace Tags {
osmium::TagsFilter Parse(const std::experimental::filesystem::path &path_to_exclude_csv);
}
