#include <cstdlib>
#include <cstdint>
#include <type_traits>

#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0

class GameObject;
class LinkingContext;

// template<bool T>
// class MemoryStream
// {
// 	
// };

class OutputMemoryStream
{
public:
	OutputMemoryStream() :
		mLinkingContext(nullptr)
	{
		mBuffer = nullptr;
		ReallocBuffer(32);
		// MallocBuffer(32);
	}
	
	~OutputMemoryStream()	{ std::free( mBuffer ); }
	
	//get a pointer to the data in the stream
	const 	char*		GetBufferPtr()	const	{ return mBuffer; }
			uint32_t	GetLength()		const	{ return mHead; }
	
			void		Write( const void* inData,
								size_t inByteCount );
	
	template< typename T > void Write( T inData )
	{
		static_assert( std::is_arithmetic< T >::value ||
					  std::is_enum< T >::value,
					  "Generic Write only supports primitive data types" );
		
		if( STREAM_ENDIANNESS == PLATFORM_ENDIANNESS )
		{
			Write( &inData, sizeof( inData ) );
		}
		else
		{
			T swappedData = ByteSwap( inData );
			Write( &swappedData, sizeof( swappedData ) );
		}
		
	}
	
	void Write( const std::vector< int >& inIntVector )
	{
		size_t elementCount = inIntVector.size();
		Write( elementCount );
		Write( inIntVector.data(), elementCount * sizeof( int ) );
	}
	
	template< typename T >
	void Write( const std::vector< T >& inVector )
	{
		uint32_t elementCount = inVector.size();
		Write( elementCount );
		for( const T& element : inVector )
		{
			Write( element );
		}
	}
	
	void Write( const std::string& inString )
	{
		size_t elementCount = inString.size() ;
		Write( elementCount );
		Write( inString.data(), elementCount * sizeof( char ) );
	}
	
	void Write( const GameObject* inGameObject )
	{
		uint32_t networkId = mLinkingContext->GetNetworkId( const_cast< GameObject* >( inGameObject ), false );
		Write( networkId );
	}

	// template<int, int>
	// void Write(const std::unordered_map<int, int>& inMap);

	template<typename Key, typename Value>
	void Write(const std::unordered_map<Key, Value>& inMap)
	{
		const size_t elementCount = inMap.size();
		Write( elementCount );
		for(auto& KV : inMap)
		{
			Write<Key>(KV.first);
			Write<Value>(KV.second);
		}
	}

	
	
private:
	void MallocBuffer( uint32_t inNewLength );
			void		ReallocBuffer( uint32_t inNewLength );
	
	char*		mBuffer = nullptr;
	uint32_t	mHead = 0;
	uint32_t	mCapacity = 0;
	
	LinkingContext* mLinkingContext = nullptr;
};

class InputMemoryStream
{
public:
	InputMemoryStream( char* inBuffer, uint32_t inByteCount ) :
	mBuffer( inBuffer ), mCapacity( inByteCount ), mHead( 0 ),
	mLinkingContext( nullptr ) {}

	~InputMemoryStream()	{ std::free( mBuffer ); }
		
	uint32_t		GetRemainingDataSize() const
					{ return mCapacity - mHead; }
	
	void		Read( void* outData, uint32_t inByteCount );


	template< typename T > void Read( T& outData )
	{
		static_assert( std::is_arithmetic< T >::value ||
					   std::is_enum< T >::value,
					   "Generic Read only supports primitive data types" );
		Read( &outData, sizeof( outData ) );
	}
	
	template< typename T >
	void Read( std::vector< T >& outVector )
	{
		size_t elementCount;
		Read( elementCount );
		outVector.resize( elementCount );
		for( const T& element : outVector )
		{
			Read( element );
		}
	}
	
	void Read( GameObject*& outGameObject )
	{
		uint32_t networkId;
		Read( networkId );
		outGameObject = mLinkingContext->GetGameObject( networkId );
	}

	// template<int, int>
	// void Read(std::unordered_map<int, int>& inMap);

	template<typename Key, typename Value>
	void Read(std::unordered_map<Key, Value>& inMap)
	{
		size_t elementCount;
		Read( elementCount );
		Key key;
		Value value;
		for(int i = 0; i < elementCount; ++i)
		{
			Read<Key>( key );
			Read<Value>( value );
			inMap[key] = value;
		}
	}

	
private:
	char*		mBuffer = nullptr;
	uint32_t	mHead = 0;
	uint32_t	mCapacity = 0;

	LinkingContext* mLinkingContext = nullptr;
};

