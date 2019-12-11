#include <experimental/filesystem>
#include "node_ref/node_ref_table.hpp"
#include "tags/parse_tag_csv.hpp"

#include <osmium/io/writer.hpp>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Missing input file" << std::endl;
        return 1;
    }

    const std::string infile{argv[1]};

    std::experimental::filesystem::path csv;
    if (argc == 3) {
        std::cout << "Reading file..." << std::endl;
        csv = std::string(argv[2]);
    }
    auto filter = Tags::Parse(csv);
    std::cout << "Filter contains " << filter.count() << " rules." << std::endl;

    NodeRef::NodeRefTable node_ref_table;

    node_ref_table.build(infile, filter);

    node_ref_table.pruneSingleOccurrences();

    std::cout << "hash table size " << node_ref_table.size() << std::endl;
    /*

    Normalize::Normalizer normalizer(infile, node_ref_table);

    normalizer.run();

    osmium::io::Writer writer{"./normalized_ways.osm.pbf"};
    normalizer.write(writer);
    writer.close();
    */
}
