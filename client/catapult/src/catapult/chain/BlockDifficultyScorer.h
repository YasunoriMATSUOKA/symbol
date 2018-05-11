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
#include "catapult/cache_core/BlockDifficultyCache.h"
#include "catapult/model/BlockChainConfiguration.h"
#include "catapult/types.h"

namespace catapult { namespace chain {

	/// Calculates the block difficulty given the past difficulties and timestamps (\a difficultyInfos) for the
	/// block chain described by \a config.
	Difficulty CalculateDifficulty(const cache::DifficultyInfoRange& difficultyInfos, const model::BlockChainConfiguration& config);

	/// Calculates the block difficulty at \a height for the block chain described by \a config
	/// given the block difficulty \a cache.
	Difficulty CalculateDifficulty(const cache::BlockDifficultyCache& cache, Height height, const model::BlockChainConfiguration& config);

	/// Calculates the block difficulty at \a height into \a difficulty for the block chain described by
	/// \a config given the block difficulty \a cache.
	bool TryCalculateDifficulty(
			const cache::BlockDifficultyCache& cache,
			Height height,
			const model::BlockChainConfiguration& config,
			Difficulty& difficulty);
}}
