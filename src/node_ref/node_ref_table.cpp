#include <iostream>

#include "node_ref/node_ref_table.hpp"

namespace NodeRef {

void NodeRefTable::build(const std::string &osm_file) {
    auto tags = osmium::TagsFilter();

    return build(osm_file, tags);
}

void NodeRefTable::build(const std::string &osm_file, const osmium::TagsFilter &tags_filter) {
    osmium::io::File file{osm_file};
    osmium::io::Reader way_reader{file, osmium::osm_entity_bits::way};

    NodeRefCounter node_ref_counter{tags_filter};

    std::cout << "Parsing file of size : " << way_reader.file_size() << std::endl;
    while (const auto buf = way_reader.read()) {
        osmium::apply(buf, node_ref_counter);
    }

    way_reader.close();

    map = std::move(node_ref_counter.count_table);
}

int NodeRefTable::pruneSingleOccurrences() {
    std::size_t original_size = map.size();

    for (auto it = map.begin(); it != map.end();) {
        if (it->second == 1) {
            it = map.erase(it);
        } else {
            ++it;
        }
    }

    return original_size - map.size();
}

void NodeRefTable::incrementEntry(const osmium::unsigned_object_id_type &node_id) { map[node_id] += 1; }

std::size_t NodeRefTable::size() {
  for (const auto& entry : map) {
    std::cout << entry.first << "," << entry.second << std::endl;
  }
  return map.size();
}

void NodeRefCounter::way(const osmium::Way &way) {
    if (tag_filter(*(way.tags().begin()))) {
      return;
    }
    for (const auto &node : way.nodes()) {
        count_table[node.positive_ref()] += 1;
    }
}

} // namespace NodeRef
