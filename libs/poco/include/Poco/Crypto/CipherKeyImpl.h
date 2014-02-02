//
// CipherKeyImpl.h
//
// $Id: //poco/1.4/Crypto/include/Poco/Crypto/CipherKeyImpl.h#3 $
//
// Library: Crypto
// Package: Cipher
// Module:  CipherKeyImpl
//
// Definition of the CipherKeyImpl class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Crypto_CipherKeyImpl_INCLUDED
#define Crypto_CipherKeyImpl_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <vector>


struct evp_cipher_st;
typedef struct evp_cipher_st EVP_CIPHER;


namespace Poco {
namespace Crypto {


class CipherKeyImpl: public RefCountedObject
	/// An implementation of the CipherKey class for OpenSSL's crypto library.
{
public:
	typedef std::vector<unsigned char> ByteVec;
	typedef Poco::AutoPtr<CipherKeyImpl> Ptr;

	enum Mode
		/// Cipher mode of operation. This mode determines how multiple blocks
		/// are connected; this is essential to improve security.
	{
		MODE_STREAM_CIPHER,	/// Stream cipher
		MODE_ECB,			/// Electronic codebook (plain concatenation)
		MODE_CBC,			/// Cipher block chaining (default)
		MODE_CFB,			/// Cipher feedback
		MODE_OFB			/// Output feedback
	};

	CipherKeyImpl(const std::string& name, 
		const std::string& passphrase, 
		const std::string& salt,
		int iterationCount);
		/// Creates a new CipherKeyImpl object, using
		/// the given cipher name, passphrase, salt value
		/// and iteration count.

	CipherKeyImpl(const std::string& name, 
		const ByteVec& key, 
		const ByteVec& iv);
		/// Creates a new CipherKeyImpl object, using the 
		/// given cipher name, key and initialization vector.

	CipherKeyImpl(const std::string& name);
		/// Creates a new CipherKeyImpl object. Autoinitializes key
		/// and initialization vector.

	virtual ~CipherKeyImpl();
		/// Destroys the CipherKeyImpl.

	const std::string& name() const;
		/// Returns the name of the Cipher.

	int keySize() const;
		/// Returns the key size of the Cipher.

	int blockSize() const;
		/// Returns the block size of the Cipher.

	int ivSize() const;
		/// Returns the IV size of the Cipher.

	Mode mode() const;
		/// Returns the Cipher's mode of operation.
	
	const ByteVec& getKey() const;
		/// Returns the key for the Cipher.

	void setKey(const ByteVec& key);
		/// Sets the key for the Cipher.

	const ByteVec& getIV() const;
		/// Returns the initialization vector (IV) for the Cipher.

	void setIV(const ByteVec& iv);
		/// Sets the initialization vector (IV) for the Cipher.

	const EVP_CIPHER* cipher();
		/// Returns the cipher object
	
private:
	void generateKey(const std::string& passphrase,
		const std::string& salt,
		int iterationCount);
	 	/// Generates key and IV from a password and optional salt string.

	void generateKey();
		/// Generates key and IV from random data.

	void getRandomBytes(ByteVec& vec, std::size_t count);
		/// Stores random bytes in vec.

private:
	const EVP_CIPHER*  _pCipher;
	std::string	       _name;
	ByteVec		       _key;
	ByteVec		       _iv;
	OpenSSLInitializer _openSSLInitializer;
};


//
// Inlines
//
inline const std::string& CipherKeyImpl::name() const
{
	return _name;
}


inline const CipherKeyImpl::ByteVec& CipherKeyImpl::getKey() const
{
	return _key;
}


inline void CipherKeyImpl::setKey(const ByteVec& key)
{
	poco_assert(key.size() == static_cast<ByteVec::size_type>(keySize()));
	_key = key;
}


inline const CipherKeyImpl::ByteVec& CipherKeyImpl::getIV() const
{
	return _iv;
}


inline void CipherKeyImpl::setIV(const ByteVec& iv)
{
	poco_assert(iv.size() == static_cast<ByteVec::size_type>(ivSize()));
	_iv = iv;
}


inline const EVP_CIPHER* CipherKeyImpl::cipher()
{
	return _pCipher;
}


} } // namespace Poco::Crypto


#endif // Crypto_CipherKeyImpl_INCLUDED
