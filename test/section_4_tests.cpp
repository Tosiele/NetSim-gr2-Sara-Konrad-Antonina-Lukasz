//
// Created by konfe on 24.01.2026.
//
#include <fstream>
#include <gtest/gtest.h>
#include "../include/factory.hpp"


TEST(parse_line_Test, correct_parsing_ramp) {
    //Tests if a ramp structure is analyzed correctly
    std::string s = "LOADING_RAMP id=1 delivery-interval=3";
    ParsedLineData ps = parse_line(s);
    ParsedLineData pe;
    pe.element_type = ElementType::RAMP;
    pe.parameters = {{"id","1"},{"delivery-interval","3"}};
    EXPECT_EQ(ps, pe);
}
TEST (parse_line_Test, correct_parsing_worker) {
    //Tests if a worker structure is analyzed correctly
    std::string s = "WORKER id=1 processing-time=2 queue-type=FIFO";
    ParsedLineData ps = parse_line(s);
    ParsedLineData pe;
    pe.element_type = ElementType::WORKER;
    pe.parameters = {{"id","1"},{"processing-time","2"},{"queue-type","FIFO"}};
    EXPECT_EQ(ps, pe);
}
TEST (parse_line_Test, incorrect_parsing) {
    //Tests if an incorrect structure throws an error
    std::string s = "ERROR other=something";
    ASSERT_THROW(parse_line(s),std::invalid_argument);
}

#include <filesystem>

TEST (load_and_save_factory_structure_Test, loading_and_save_correctness_1) {

    std::cout << "Current path: "
              << std::filesystem::current_path()
              << std::endl;
    //Tests if the load_factory_structure and save_factory_structure work as intended
    //Needs to be checked after full Factory implementation
    std::ifstream file("IO test files/load_factory_structure_1");
    ASSERT_TRUE(file.is_open());
    Factory factory{};
    factory = load_factory_structure(file);
    file.close();
    std::ofstream file2;
    file2.open("IO test files/save_factory_structure_2");
    ASSERT_TRUE(file2.is_open());
    //save_factory_structure(factory,file2);
    file2.close();
    bool t = compareFiles("IO test files/save_factory_structure_1","IO test files/save_factory_structure_2");
    EXPECT_TRUE(t);

}