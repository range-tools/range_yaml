#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include <range_yaml.h>

void left_pad(const int depth) {
  for (int i{0}; i < depth; ++i) {
    std::cout << "  ";
  }
}

void print_level(const int depth, RangeYaml *yaml) {
  for (const RangeYaml::Node *n : *yaml) {
    switch (n->type) {
    case (RangeYaml::SCALAR):
      std::cout << n->type << "  ";
      left_pad(depth);
      std::cout << n->key << ": " << n->scalar << ' ' << '\n';
      break;
    case (RangeYaml::MAPPING):
      std::cout << n->type << "  ";
      left_pad(depth);
      std::cout << n->key << ":\n";
      print_level(depth + 1, n->nested);
    }
  }
}

const std::string read_file(std::string_view path) {
  // https://stackoverflow.com/questions/195323/what-is-the-most-elegant-way-to-read-a-text-file-with-c
  std::ifstream in(path.data());
  std::string contents((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());

  return contents;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " [file.yaml]\n";
    return 0;
  }
  const std::string yaml_data{read_file(argv[1])};

  RangeYaml yaml(yaml_data.c_str());
  print_level(0, &yaml);
}
