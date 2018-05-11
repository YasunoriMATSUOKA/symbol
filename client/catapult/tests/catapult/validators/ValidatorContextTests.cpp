/**
*** Copyright (c) 2016-present,
*** Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp. All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#include "catapult/validators/ValidatorContext.h"
#include "tests/test/cache/CacheTestUtils.h"
#include "tests/TestHarness.h"

namespace catapult { namespace validators {

#define TEST_CLASS ValidatorContextTests

	TEST(TEST_CLASS, CanCreateValidatorContextAroundHeightAndNetworkAndCache) {
		// Act:
		auto networkInfo = model::NetworkInfo(static_cast<model::NetworkIdentifier>(0xAD), {}, {});
		auto cache = test::CreateEmptyCatapultCache();
		auto cacheView = cache.createView();
		auto readOnlyCache = cacheView.toReadOnly();
		auto context = ValidatorContext(Height(1234), Timestamp(987), networkInfo, readOnlyCache);

		// Assert:
		EXPECT_EQ(Height(1234), context.Height);
		EXPECT_EQ(Timestamp(987), context.BlockTime);
		EXPECT_EQ(static_cast<model::NetworkIdentifier>(0xAD), context.Network.Identifier);
		EXPECT_EQ(&readOnlyCache, &context.Cache);
	}
}}
