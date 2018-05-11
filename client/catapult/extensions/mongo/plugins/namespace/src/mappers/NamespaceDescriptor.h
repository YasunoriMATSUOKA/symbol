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
#include "plugins/txes/namespace/src/state/RootNamespace.h"

namespace catapult { namespace mongo { namespace plugins {

	/// A namespace descriptor.
	struct NamespaceDescriptor {
	public:
		/// Creates a namespace descriptor around \a path, \a pRootNamespace, \a ownerAddress, \a index and \a isActive.
		explicit NamespaceDescriptor(
				const state::Namespace::Path& path,
				const std::shared_ptr<const state::RootNamespace>& pRootNamespace,
				const Address& ownerAddress,
				uint32_t index,
				bool isActive)
				: Path(path)
				, pRoot(pRootNamespace)
				, OwnerAddress(ownerAddress)
				, Index(index)
				, IsActive(isActive)
		{}

	public:
		/// Returns \c true if the described namespace is a root namespace.
		bool IsRoot() const {
			return 1 == Path.size();
		}

	public:
		/// Namespace path.
		const state::Namespace::Path Path;

		/// Associated root namespace.
		const std::shared_ptr<const state::RootNamespace> pRoot;

		/// Owner address.
		/// \note Address is stored to minimize number of conversions.
		Address OwnerAddress;

		/// Index in the root namespace history.
		uint32_t Index;

		/// Flag indicating whether or not the namespace is active.
		bool IsActive;
	};
}}}
