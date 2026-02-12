//
// Created by konfe on 12.02.2026.
//
#include <fstream>
#include <gtest/gtest.h>
#include "../include/reports.hpp"
#include"../include/simulation.hpp"

TEST (structure_report_Test, structure_correctness_1) {
    //Tests if the structure_report works as intended
    std::ifstream file("../test/test files/load_factory_structure_1");
    ASSERT_TRUE(file.is_open());
    Factory factory{};
    factory = load_factory_structure(file);
    file.close();
    std::ofstream file2;
    file2.open("../test/test files/structure_report_1_sol");
    ASSERT_TRUE(file2.is_open());
    structure_report(factory,file2);
    file2.close();
    bool t = compareFiles("../test/test files/structure_report_1","../test/test files/structure_report_1_sol");
    EXPECT_TRUE(t);

}

TEST(turn_report_Test, report_correctness_1) {
    //Tests if the turn_report works as intended
    std::ifstream file("../test/test files/load_factory_structure_1");
    ASSERT_TRUE(file.is_open());
    Factory factory{};
    factory = load_factory_structure(file);
    file.close();
    std::ofstream file2;
    file2.open("../test/test files/turn_report_1_sol");
    ASSERT_TRUE(file2.is_open());
    SpecificTurnsReportNotifier spec_notifier(std::set<Time>{2});
    simulate(factory, 3, [&spec_notifier, &file2](Factory& f, TimeOffset t_offset) {
        if (spec_notifier.should_generate_report(t_offset)) {
            turn_report(f, t_offset, file2);
        }
    });
    file2.close();
    bool t = compareFiles("../test/test files/turn_report_1","../test/test files/turn_report_1_sol");
    bool t2 = compareFiles("../test/test files/turn_report_1_opt_2","../test/test files/turn_report_1_sol");
    ASSERT_TRUE(t or t2);
}