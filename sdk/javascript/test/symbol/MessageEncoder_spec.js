const { PrivateKey, PublicKey } = require('../../src/CryptoTypes');
const { concatArrays } = require('../../src/impl/CipherHelpers');
const { KeyPair } = require('../../src/symbol/KeyPair');
const { MessageEncoder } = require('../../src/symbol/MessageEncoder');
const { runBasicMessageEncoderTests } = require('../test/messageEncoderTests');
const { expect } = require('chai');

describe('MessageEncoder (Symbol)', () => {
	runBasicMessageEncoderTests({ KeyPair, MessageEncoder, encodeAccessor: encoder => encoder.encode.bind(encoder) });

	// note: there's no sender decode test for persistent harvesting delegation, cause sender does not have ephemeral key pair
	it('recipient can decode encoded persistent harvesting delegation', () => {
		// Arrange:
		const keyPair = new KeyPair(PrivateKey.random());
		const nodeKeyPair = new KeyPair(PrivateKey.random());
		const remoteKeyPair = new KeyPair(new PrivateKey('11223344556677889900AABBCCDDEEFF11223344556677889900AABBCCDDEEFF'));
		const vrfKeyPair = new KeyPair(new PrivateKey('11223344556677889900AABBCCDDEEFF11223344556677889900AABBCCDDEEFF'));
		const encoder = new MessageEncoder(keyPair);
		const encoded = encoder.encodePersistentHarvestingDelegation(nodeKeyPair.publicKey, remoteKeyPair, vrfKeyPair);

		// Act:
		const decoder = new MessageEncoder(nodeKeyPair);
		const [result, decoded] = decoder.tryDecode(keyPair.publicKey, encoded);

		// Assert:
		/* eslint-disable no-unused-expressions */
		expect(result).to.be.true;
		expect(decoded).to.deep.equal(concatArrays(remoteKeyPair.privateKey.bytes, vrfKeyPair.privateKey.bytes));
	});

	it('decode falls back to input when message has unknown type', () => {
		// Arrange:
		const encoder = new MessageEncoder(new KeyPair(PrivateKey.random()));
		const invalidEncoded = Uint8Array.from(Buffer.from('024A4A4A', 'hex'));

		// Act:
		const [result, decoded] = encoder.tryDecode(new PublicKey(new Uint8Array(32)), invalidEncoded);

		// Assert:
		expect(result).to.be.false;
		expect(decoded).to.deep.equal(invalidEncoded);
	});
});
