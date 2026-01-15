#include <gtest/gtest.h>

#include "storage_types.hpp"
#include "Package.hpp"

TEST(PackageQueueTest, queue_correctness){
PackageQueue Q1 = PackageQueue(QueueType::Lifo);
PackageQueue Q2 = PackageQueue(QueueType::Fifo);
Q1.push(Package(7));
Q1.push(Package(9));
Q2.push(Package(7));
Q2.push(Package(9));
Package Px,Py;
Px = Q1.pop();
Py = Q2.pop();
ASSERT_EQ(Px,Package(7));
ASSERT_EQ(Py,Package(9));
}
