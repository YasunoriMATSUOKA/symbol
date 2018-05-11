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

#include "ReclaimMemoryInspector.h"
#include "catapult/validators/ValidationResult.h"

namespace catapult { namespace consumers {

	namespace {
		class ReclaimMemoryInspector {
		public:
			void operator()(disruptor::ConsumerInput& input, const disruptor::ConsumerCompletionResult& completionResult) const {
				if (disruptor::CompletionStatus::Aborted == completionResult.CompletionStatus) {
					auto validationResult = static_cast<validators::ValidationResult>(completionResult.CompletionCode);
					CATAPULT_LOG_LEVEL(MapToLogLevel(validationResult))
							<< "consumer aborted at position " << completionResult.FinalConsumerPosition
							<< " while processing " << input
							<< " due to " << validationResult;
				}

				input = disruptor::ConsumerInput();
			}
		};
	}

	disruptor::DisruptorInspector CreateReclaimMemoryInspector() {
		return ReclaimMemoryInspector();
	}
}}
