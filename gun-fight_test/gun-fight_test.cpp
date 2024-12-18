#include "pch.h"
#include "CppUnitTest.h"
#include "../gun-fight/weapons.h"

#include <memory>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace gunfighttest{
	TEST_CLASS(revolvertests){
	public:
		TEST_METHOD(WepInit){
			auto rev = std::make_unique<wep::revolver>(wep::revolver());
			Assert::AreEqual(rev->get_ammo(), wep::REVOLVER_AMMO);
			Assert::AreEqual(rev->get_damage(), wep::REVOLVER_DMG);
			Assert::AreEqual(rev->is_loaded(), true);
		}
		TEST_METHOD(WepFireReloadLoaded) {
			auto rev = std::make_unique<wep::revolver>(wep::revolver());

			// fire the weapon then reload
			Assert::AreEqual(rev->fire(), true);
			Assert::AreEqual(rev->is_loaded(), false);
			Assert::AreEqual(rev->fire(), false);
			Assert::AreEqual(rev->reload(), true);
			Assert::AreEqual(rev->get_ammo(), 5);
			Assert::AreEqual(rev->is_loaded(), true);
		}
		TEST_METHOD(WepReloadLoaded) {
			auto rev = std::make_unique<wep::revolver>(wep::revolver());

			Assert::AreEqual(rev->is_loaded(), true);
			Assert::AreEqual(rev->reload(), true);
			Assert::AreEqual(rev->get_ammo(), 5);
			Assert::AreEqual(rev->is_loaded(), true);
		}
		TEST_METHOD(WepFireUnloaded) {
			auto rev = std::make_unique<wep::revolver>(wep::revolver());
			Assert::AreEqual(rev->is_loaded(), true);
			Assert::AreEqual(rev->fire(), true);
			Assert::AreEqual(rev->fire(), false);
		}
		TEST_METHOD(WepNoAmmo) {
			auto rev = std::make_unique<wep::revolver>(wep::revolver());
			Assert::AreEqual(rev->is_loaded(), true);
			while (rev->get_ammo() > 0) {
				rev->fire();
				rev->reload();
			}
			Assert::AreEqual(rev->get_ammo(), 0);
			Assert::AreEqual(rev->is_loaded(), true);
			// cannot reload, no spare bullets
			Assert::AreEqual(rev->reload(), false);

			// fire the last bullet and try to reload
			Assert::AreEqual(rev->fire(), true);
			Assert::AreEqual(rev->reload(), false);
		}
	};
	TEST_CLASS(gunmantests) {

	};

}
