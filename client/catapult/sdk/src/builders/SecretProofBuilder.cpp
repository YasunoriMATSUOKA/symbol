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

#include "SecretProofBuilder.h"

namespace catapult { namespace builders {

	SecretProofBuilder::SecretProofBuilder(model::NetworkIdentifier networkIdentifier, const Key& signer)
			: TransactionBuilder(networkIdentifier, signer)
			, m_hashAlgorithm(model::LockHashAlgorithm::Op_Sha3)
			, m_secret()
	{}

	void SecretProofBuilder::setHashAlgorithm(model::LockHashAlgorithm hashAlgorithm) {
		m_hashAlgorithm = hashAlgorithm;
	}

	void SecretProofBuilder::setSecret(const Hash512& secret) {
		m_secret = secret;
	}

	void SecretProofBuilder::setProof(const RawBuffer& proof) {
		m_proof.resize(proof.Size);
		std::memcpy(m_proof.data(), proof.pData, proof.Size);
	}

	template<typename TransactionType>
	std::unique_ptr<TransactionType> SecretProofBuilder::buildImpl() const {
		// 1. allocate, zero (header), set model::Transaction fields
		auto pTransaction = createTransaction<TransactionType>(sizeof(TransactionType) + m_proof.size());

		// 2. set transaction fields
		pTransaction->HashAlgorithm = m_hashAlgorithm;
		pTransaction->Secret = m_secret;
		pTransaction->ProofSize = utils::checked_cast<size_t, uint16_t>(m_proof.size());

		// 3. set proof
		std::memcpy(pTransaction->ProofPtr(), m_proof.data(), m_proof.size());

		return pTransaction;
	}

	std::unique_ptr<SecretProofBuilder::Transaction> SecretProofBuilder::build() const {
		return buildImpl<Transaction>();
	}

	std::unique_ptr<SecretProofBuilder::EmbeddedTransaction> SecretProofBuilder::buildEmbedded() const {
		return buildImpl<EmbeddedTransaction>();
	}
}}
