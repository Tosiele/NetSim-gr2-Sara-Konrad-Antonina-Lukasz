#include <gtest/gtest.h>

#include "storage_types.hpp"
#include "Package.hpp"
#include "nodes.hpp"
#include "types.hpp"

TEST(ReceiverPreferencesTest, probability_correctness){
    //Tests if the probability of each receiver being chosen is equal and correct
  using preferences_t = std::map<IPackageReceiver*, double>;
  Receiver R1{};
  Receiver R2{};
  Receiver R3{};
  ReceiverPreferences RP(probability_generator);
  RP.add_receiver(&R1);
  preferences_t& prefs = RP.get_preferences();
  EXPECT_EQ(prefs.begin()->second,1);
  RP.add_receiver(&R2);
  prefs = RP.get_preferences();
  EXPECT_EQ(prefs.begin()->second,1/2.0);
  EXPECT_EQ(prefs.rbegin()->second,1/2.0);
  RP.add_receiver(&R3);
  RP.remove_receiver(&R1);
  prefs = RP.get_preferences();
  EXPECT_EQ(prefs.begin()->second,1/2.0);
  EXPECT_EQ(prefs.rbegin()->second,1/2.0);
  RP.remove_receiver(&R2);
  prefs = RP.get_preferences();
  EXPECT_EQ(prefs.begin()->second,1);

 }
double simple_func(){
    return 0.4;
}


TEST(ReceiverPreferencesTest, choosing_correctness) {
    //Tests if the receivers are chosen correctly using a mocked random generator
    using preferences_t = std::map<IPackageReceiver*, double>;
    Receiver R1{};
    Receiver R2{};
    Receiver R3{};
    Receiver R4{};
    ReceiverPreferences RP(simple_func);
    RP.add_receiver(&R1);
    IPackageReceiver* Ro = RP.choose_receiver();
    RP.add_receiver(&R2);
    IPackageReceiver* Ro2 = RP.choose_receiver();
    RP.add_receiver(&R3);
    IPackageReceiver* Ro3 = RP.choose_receiver();
    RP.remove_receiver(&R1);
    IPackageReceiver* Ro4 = RP.choose_receiver();
    EXPECT_EQ(Ro,&R1);
    EXPECT_EQ(Ro2,&R2);
    EXPECT_EQ(Ro3,&R2);
    EXPECT_EQ(Ro4,&R3);
}

TEST(PackageSenderTest, buffer_emptying) {
    //Tests if the PackageSender buffer is emptied correctly after sending the package
    auto Q1 = std::make_unique<PackageQueue>(QueueType::Fifo);
    ExampleSender sender;
    Storehouse S = Storehouse(1,std::move(Q1));
    sender.receiver_preferences.add_receiver(&S);
    sender.pusher(); //when actual class implemented, replace this with an actual method that invokes push_package
    auto b = std::move(sender.get_sending_buffer());
    ASSERT_TRUE(b.has_value());
    sender.send_package();
    auto b2 = std::move(sender.get_sending_buffer());
    ASSERT_FALSE(b2.has_value());
}

TEST(StorehoustTest, correct_storage) {
    //Tests if the Storehouse stores packages correctly
    auto Q1 = std::make_unique<PackageQueue>(QueueType::Fifo);
    Storehouse S1 = Storehouse(1,std::move(Q1));
    auto Q2 = std::make_unique<PackageQueue>(QueueType::Lifo);
    Storehouse S2 = Storehouse(1,std::move(Q2));
    S1.receive_package(Package(7));
    S1.receive_package(Package(8));
    S2.receive_package(Package(7));
    S2.receive_package(Package(8));
    EXPECT_EQ(*S1.begin(), Package(8));
    EXPECT_EQ(*S2.begin(), Package(8)); //both queue types are the same, because the difference is in pop method, not push
}


