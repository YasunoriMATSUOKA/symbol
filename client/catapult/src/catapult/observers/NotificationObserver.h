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
#include "ObserverContext.h"
#include "catapult/model/Notifications.h"

namespace catapult { namespace observers {

	/// A strongly typed notification observer.
	template<typename TNotification>
	class NotificationObserverT {
	public:
		/// Notification type.
		using NotificationType = TNotification;

	public:
		virtual ~NotificationObserverT() {}

	public:
		/// Gets the observer name.
		virtual const std::string& name() const = 0;

		/// Notifies the observer with a \a notification to process and an observer \a context.
		virtual void notify(const TNotification& notification, const ObserverContext& context) const = 0;
	};
}}
