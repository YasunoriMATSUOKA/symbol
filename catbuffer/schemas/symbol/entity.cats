import "types.cats"

# enumeration of entity types
enum EntityType : uint16
	# reserved entity type
	reserved = 0x0000

# enumeration of network types
enum NetworkType : uint8
	# public network
	mainnet = 0x68

	# public test network
	testnet = 0x98

# binary layout for a size-prefixed entity
struct SizePrefixedEntity
	# entity size
	size = uint32

# binary layout for a verifiable entity
struct VerifiableEntity
	# reserved padding to align Signature on 8-byte boundary
	verifiableEntityHeader_Reserved1 = uint32

	# entity signature
	signature = Signature

# binary layout for a blockchain entity (block or transaction)
struct EntityBody
	# entity signer's public key
	signerPublicKey = PublicKey

	# reserved padding to align end of EntityBody on 8-byte boundary
	entityBody_Reserved1 = uint32

	# entity version
	version = uint8

	# entity network
	network = NetworkType

	# entity type
	type = EntityType
