#ifndef VCSLIB_CRYPT_RSA_H
#define VCSLIB_CRYPT_RSA_H

#include <vcslib/Time.h>
#include <string>
#include <vector>
#include <memory>

namespace vcslib
{
	namespace crypt
	{
		// deleter for PIMPL implementation
		template <class T>
		struct ImplDeleter
		{
			void operator()(T* p)
			{
				delete p;
			}
		};

		template<class T>
		using ImplUniquePtr = std::unique_ptr<T/*, ImplDeleter<T>*/>;

		class Certificate;
		class CertificateStore;

		// RsaPrivateKey, movable/uncopyable
		class RsaPrivateKey
		{
		public:
			RsaPrivateKey();
			~RsaPrivateKey();

			// copy constructor and copy assignment operator
			RsaPrivateKey(const RsaPrivateKey&) = delete;
			RsaPrivateKey& operator=(const RsaPrivateKey&) = delete;

			// move constructor and move assignment operator
			RsaPrivateKey(RsaPrivateKey&&) = default;
			RsaPrivateKey& operator=(RsaPrivateKey&&) = default;

			explicit operator bool() const;

			/**
			 * load a private key from file
			 * @param filename name of the private key file
			 * @param password the password to decrypt the key
			 * @returns true if certificate is successfully loaded; false otherwise.
			 */
			bool loadFromFile(const std::string& filename, const std::string& password = "");

			/**
			 * load a private key from memory buffer
			 * @param key memory buffer
			 * @param len size of memory buffer
			 * @param the password to decrypt the key
			 * @returns true if certificate is successfully loaded; false otherwise.
			 */
			bool loadFromMemory(const uint8* key, uint32 len, const std::string& password = "");

			/**
			 * save the private key into file
			 * @param name of the saved private key file
			 * @param the password to encrypt the key
			 * @return true public key is saved successfully; false otherwise
			 */
			bool saveToFile(const std::string& filename, const std::string& password = "");

			/**
			 * generate a private key with specified length
			 * @param keyLengthBits key length
			 * @return true public key is generated successfully; false otherwise
			 */
			bool generate(uint32 keyLengthBits);

			/**
			 * sign a message with the private key
			 * @param data message data to be signed
			 * @param dataSize size of the message data
			 * @return signed message
			 */
			std::string sign(const uint8* data, uint32 dataSize);

			/**
			 * get bit length of the private key
			 * @return bit length of the private key
			 */
			uint32 getBitLength() const;

		private:
			friend class Certificate;
			class RsaPrivateKeyImpl;
			ImplUniquePtr<RsaPrivateKeyImpl> pImpl;
		};

		// RsaPublicKey, movable/uncopyable
		class RsaPublicKey
		{
		public:
			RsaPublicKey();
			~RsaPublicKey();

			// copy constructor and copy assignment operator
			RsaPublicKey(const RsaPublicKey&) = delete;
			RsaPublicKey& operator=(const RsaPublicKey&) = delete;

			// move constructor and move assignment operator
			RsaPublicKey(RsaPublicKey&&) = default;
			RsaPublicKey& operator=(RsaPublicKey&&) = default;

			explicit operator bool() const;

			/**
			 * load a public key from file
			 * @param filename name of public key file
			 * @returns true if public key is successfully loaded; false otherwise.
			 */
			bool loadFromFile(const std::string& filename);
			
			/**
			 * load a public key from memory buffer
			 * @param key memory buffer
			 * @len size of memory buffer
			 * @returns true if public key is successfully loaded; false otherwise.
			 */
			bool loadFromMemory(const uint8* key, uint32 len);

			/**
			 * save the public key into file
			 * @param name of the saved public key file
			 * @return true public key is saved successfully; false otherwise
			 */
			bool saveToFile(const std::string& filename);

			/**
			 * verify the signature of a message
			 * @param data message data to verify
			 * @param dataSize size of the message data
			 * @param signature signature of the message data
			 * @param signatureSize size of the signature
			 * @return true the signature is valid; false otherwise
			 */
			bool verify(const uint8* data, uint32 dataSize, const uint8* signature, uint32 signatureSize);

		private:
			friend class Certificate;
			class RsaPublicKeyImpl;
			ImplUniquePtr<RsaPublicKeyImpl> pImpl;
		};

		// Certificate, movable/copyable
		class Certificate
		{
		public:
			enum CertEncoding
			{
				ceBER,
				cePEM
			};

			struct Parameters
			{
				vcslib::Time m_validStartTime;
				vcslib::Time m_validEndTime;
				std::string m_commonName;
				std::string m_country;
				std::string m_organization;
				std::string m_email;
			};

			Certificate();
			virtual ~Certificate();

			// copy constructor and copy assignment operator
			Certificate(const Certificate&);
			Certificate& operator=(const Certificate&);

			// move constructor and move assignment operator
			Certificate(Certificate&&) = default;
			Certificate& operator=(Certificate&&) = default;

			explicit operator bool();

			/**
			 * load a certificate from the given buffer.
			 * @returns true if certificate is successfully loaded; false otherwise.
			 */
			bool loadFromMemory(const uint8* cert, uint32 len);

			/**
			 * load a bunch of certificates from the given buffer.
			 * The certificates need to be directly concatenated(e.g. as returned by rcpplus sdk certificate check callback).
			 * @returns the parsed certificates in a std::vector.
			 */
			static std::vector<Certificate> loadCertificatesFromMemory(const uint8* certs, uint32 totalLen);

			/**
			 * get the public key from certificate
			 * @return public key on the certificate
			 */
			RsaPublicKey getPublicKey() const;

			/**
			 * generate a certificate and sign it with private key
			 * @param privateKey private key used to sign the certificate
			 * @param params necessary certificate information
			 * @return true if certificate is generated successfully; false otherwise
			 */
			bool generateCertificate(RsaPrivateKey& privateKey, const Parameters& params);

			/**
			 * encode certificate into string
			 * @param encoding encoding 
			 * @return encoded certificate
			 */
			std::string getEncodedCertificate(CertEncoding encoding) const;

			/**
			 * save a certificate into file
			 * @param name of the saved certificate file
			 * @return true certificate is saved successfully; false otherwise
			 */
			bool saveToFile(const std::string& filename);

			/**
			 * get the common name from the certificate
			 * @return common name on the certificate
			 */
			std::string getCommonName() const;

		private:
			friend class CertificateStore;
			class CertificateImpl;
			ImplUniquePtr<CertificateImpl> pImpl;
		};

		// CertificateStore, movable/uncopyable
		class CertificateStore
		{
		public:
			CertificateStore();
			~CertificateStore();

			// copy constructor and copy assignment operator
			CertificateStore(const CertificateStore&) = delete;
			CertificateStore& operator=(const CertificateStore&) = delete;

			// move constructor and move assignment operator
			CertificateStore(CertificateStore&&) = delete;
			CertificateStore& operator=(CertificateStore&&) = delete;

			/**
			 * add a trusted certificate into store
			 * @param cert trusted CA certificate
			 */
			bool addCertificate(const Certificate& cert);

			/** 
			 * PKIX Path Validation
			 * @param end_certs certificate chain to validate
			 */
			bool validateCertificate(const std::vector<Certificate>& end_certs);

			/**
			 * get pointer to X509 certificate store
			 * @return pointer to X509 certificate store
			 */
			void* X509CertificateStore();

		private:
			class CertificateStoreImpl;
			ImplUniquePtr<CertificateStoreImpl> pImpl;
		};
	}
}

#endif