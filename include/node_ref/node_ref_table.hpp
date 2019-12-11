#ifndef NODE_REF_TABLE_HPP
#define NODE_REF_TABLE_HPP

#include <functional>
#include <string>
#include <unordered_map>

#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/io/pbf_output.hpp>
#include <osmium/osm/tag.hpp>
#include <osmium/osm/types.hpp>
#include <osmium/tags/tags_filter.hpp>

namespace NodeRef {

using IndexType = osmium::index::map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location>;
using LocationHandlerType = osmium::handler::NodeLocationsForWays<IndexType>;

typedef short int Occurrence;

class NodeRefTable {
  public:
    NodeRefTable() = default;
    NodeRefTable(const NodeRefTable &) = delete;
    NodeRefTable(NodeRefTable &&table) = delete;

    void build(const std::string &osm_file);
    void build(const std::string &osm_file, const osmium::TagsFilter &tags_filter);
    int pruneSingleOccurrences();
    void incrementEntry(const osmium::unsigned_object_id_type &node_id);
    std::size_t size();

  private:
    std::unordered_map<osmium::object_id_type, Occurrence> map;
};

class NodeRefCounter : public osmium::handler::Handler {
  public:
    NodeRefCounter(const osmium::TagsFilter &_tag_filter) : tag_filter(_tag_filter){};

    void way(const osmium::Way &way);

    std::unordered_map<osmium::object_id_type, Occurrence> count_table;

  private:
    const osmium::TagsFilter &tag_filter;
};

} // namespace NodeRef
#endif
