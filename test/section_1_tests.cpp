#include <gtest/gtest.h>

#include "storage_types.hpp"
#include "Package.hpp"

TEST(PackageQueueTest, queue_correctness){
    //Test that checks if different queue type are implemented correctly
PackageQueue Q1 = PackageQueue(QueueType::Lifo);
PackageQueue Q2 = PackageQueue(QueueType::Fifo);
Q1.push(Package(7));
Q1.push(Package(9));
Q2.push(Package(8));
Q2.push(Package(10));
Package Px,Py;
Px = Q1.pop();
Py = Q2.pop();
ASSERT_EQ(Px.get_id(),9);
ASSERT_EQ(Py.get_id(),8);
}
TEST(PackageTest, assign_correct_ids) {
    //Tests if Package ids are assigned correctly
    //First Package should hava an id=1
    Package P1{};
    //defined like this so the destructor is automatically called
    {
        Package P2(7);
    }
    //When a Package is deleted
    //as a priority its id should be assigned first
    //so id=7
    Package P3{};
    //By default, the Package should get an id
    //1 higher than the highest current id, so id=8
    Package P4{};
    ASSERT_EQ(P1.get_id(),1);
    ASSERT_EQ(P3.get_id(),7);
    ASSERT_EQ(P4.get_id(),8);


}
TEST(PackageTest, incorrect_id_error) {
    Package P1(7);
    ASSERT_THROW(Package P2(7),std::invalid_argument);
}

