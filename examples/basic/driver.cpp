#include <iostream>

#include "arrow/api.h"

#include "include/arrow_code_gen.hxx"


std::vector<v2::C> generate_row_based(int n) {
    std::vector<v2::C> result;
    for (int i=0; i<n; ++i)
        result.push_back(v2::C::make_random());

    return result;
}

std::shared_ptr<arrow::Array> to_arrow_array(std::vector<v2::C> const& rows) {
    std::shared_ptr<arrow::Array> result;
    
    // get the type tree and schema
    auto type_tree = to_arrow_type<v2::C>::get();
    auto schema = std::make_shared<arrow::Schema>(type_tree->children());
    arrow::PrettyPrint(*schema, {2}, &(std::cout));

    // make a builder
    std::unique_ptr<arrow::ArrayBuilder> builder{ nullptr };
    auto statuc = arrow::MakeBuilder(arrow::default_memory_pool(), type_tree, &builder);

    for (auto const& row : rows) {
        fill_builder(static_cast<arrow::StructBuilder*>(builder.get()), row);
    }

    // finis the build of an array
    builder->Finish(&result);
    return result;
}

int main() {
    auto rows = generate_row_based(10);
    auto arr = to_arrow_array(rows);

    arrow::PrettyPrint(*arr, {2}, &(std::cout));

    return 0;
}
