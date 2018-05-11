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

#pragma once
#include "catapult/utils/BlockSpan.h"
#include "catapult/types.h"
#include <stdint.h>

namespace catapult { namespace utils { class ConfigurationBag; } }

namespace catapult { namespace config {

	/// Lock plugin configuration settings.
	struct LockConfiguration {
	public:
		/// Amount that has to be locked per aggregate in partial cache.
		Amount LockedFundsPerAggregate;

		/// Maximum number of blocks for which a hash lock can exist.
		utils::BlockSpan MaxHashLockDuration;

		/// Maximum number of blocks for which a secret lock can exist.
		utils::BlockSpan MaxSecretLockDuration;

		/// Minimum size of a proof in bytes.
		uint16_t MinProofSize;

		/// Maximum size of a proof in bytes.
		uint16_t MaxProofSize;

	private:
		LockConfiguration() = default;

	public:
		/// Creates an uninitialized lock configuration.
		static LockConfiguration Uninitialized();

		/// Loads lock configuration from \a bag.
		static LockConfiguration LoadFromBag(const utils::ConfigurationBag& bag);
	};
}}
