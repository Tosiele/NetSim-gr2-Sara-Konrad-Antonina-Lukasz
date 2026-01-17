#include <gtest/gtest.h>

#include "storage_types.hpp"
#include "Package.hpp"
#include "nodes.hpp"
#include "types.hpp"

TEST(ReceiverPreferencesTest, probability_correctness){
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

