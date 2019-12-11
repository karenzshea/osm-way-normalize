#include <iostream>

#include "node_ref_table.hpp"

#include <osmium/handler.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/io/reader.hpp>
#include <osmium/osm/types.hpp>
#include <osmium/visitor.hpp>

class Normalizer : public osmium::handler::Handler {
  public:
    static const constexpr auto kBufferCapacity = 1u << 12u;

    Normalizer(const NodeRef::Table &_table) : intersections_table(_table) {}

    // rewrite a way with evenly spaced segments
    osmium::Way normalizeWay(const osmium::Way &way, std::vector<int> anchor_nodes);

    // identify which nodes cannot be rewritten
    void way(const osmium::Way &way) {
        const auto num_nodes = way.nodes().size();
        std::vector<int> anchor_nodes(num_nodes, 0);
        // first and last nodes should not move
        anchor_nodes[0] = 1;
        anchor_nodes[num_nodes - 1] = 1;
        for (std::size_t i = 1; i < num_nodes - 1; i++) {
            const auto &node = way.nodes()[i];
            const auto found = intersections_table.find(node.ref());
            if (found != intersections_table.end())
                anchor_nodes[i] = 1;
        }
        // osmium::Way normalized = normalizeWay(way, anchor_nodes);
        // outbuf.add_item(normalized);
    }
    // getting normalized ways for writing
    osmium::memory::Buffer buffer() {
        osmium::memory::Buffer tmp{kBufferCapacity};

        using std::swap;
        swap(tmp, outbuf);

        return tmp;
    }

  private:
    osmium::memory::Buffer outbuf{kBufferCapacity};
    const NodeRef::Table &intersections_table;
};

osmium::memory::Buffer normalizeWays(const osmium::io::File &file, const NodeRef::Table &table) {
    osmium::io::Reader way_reader{file};

    // set up handler to get locations for nodes
    NodeRef::IndexType sparsemem_index;
    NodeRef::LocationHandlerType location_handler{sparsemem_index};
    Normalizer normalizer(table);

    std::cout << "Parsing file of size : " << way_reader.file_size() << std::endl;
    osmium::apply(way_reader, location_handler, normalizer);
    way_reader.close();
    return normalizer.buffer();
}
