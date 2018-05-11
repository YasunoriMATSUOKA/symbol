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

#include "NodeUtils.h"
#include "catapult/extensions/LocalNodeBootstrapper.h"
#include "catapult/ionet/NodeContainer.h"

namespace catapult { namespace local {

	namespace {
		void CheckString(const std::string& str, const char* name) {
			if (str.size() <= std::numeric_limits<uint8_t>::max())
				return;

			std::ostringstream out;
			out << name << " is too long (" << str << ")";
			CATAPULT_THROW_INVALID_ARGUMENT(out.str().c_str());
		}

		void ValidateAndAddNode(ionet::NodeContainerModifier& modifier, const ionet::Node& node, ionet::NodeSource source) {
			CheckString(node.endpoint().Host, "host");
			CheckString(node.metadata().Name, "name");
			modifier.add(node, source);
		}
	}

	void SeedNodeContainer(ionet::NodeContainer& nodes, const extensions::LocalNodeBootstrapper& bootstrapper) {
		auto modifier = nodes.modifier();
		for (const auto& node : bootstrapper.staticNodes())
			ValidateAndAddNode(modifier, node, ionet::NodeSource::Static);

		ValidateAndAddNode(modifier, config::ToLocalNode(bootstrapper.config()), ionet::NodeSource::Local);
	}
}}
