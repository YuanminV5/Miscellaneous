#include "vcslib/crypt/Rsa.h"
#include "external/botan_all.h"
#include <fstream>

namespace vcslib
{
	namespace crypt
	{
		// template class used as basic class for rsa public key and private key
		template<typename keyType>
		class RsaKey
		{
		public:
			bool loadFromFile(const std::string& filename, const std::string& password = "");
			bool loadFromMemory(const uint8* key, uint32 len, const std::string& password = "");
			virtual bool saveToFile(const std::string& filename, const std::string& password = "") = 0;

			Botan::RandomNumberGenerator* rndNumberGenerator();
			std::shared_ptr<const keyType> rsaKey();
			void setRsaKey(std::shared_ptr<keyType> key);

		protected:
			virtual void* loadKey(const std::string& filename, const std::string& password = "") = 0;
			virtual void* loadKey(Botan::DataSource& source, const std::string& password = "") = 0;
			bool checkKey(bool);

		private:
			Botan::AutoSeeded_RNG m_randomGenerator;
			std::shared_ptr<keyType> m_rsaKey;
		};

		// load a key from file specified by path
		template<typename keyType>
		bool RsaKey<keyType>::loadFromFile(const std::string& filename, const std::string& password)
		{
			if (!m_rsaKey)
			{
				m_rsaKey.reset();
			}

			bool valid = false;
			try
			{
				void* key = loadKey(filename, password);
				m_rsaKey.reset(static_cast<keyType*>(key));

				if (m_rsaKey)
				{
					valid = checkKey(false);
				}
			}
			catch (std::exception&)
			{
			}

			return valid;
		}

		// load a key from memory
		template<typename keyType>
		bool RsaKey<keyType>::loadFromMemory(const uint8* key, uint32 len, const std::string& password)
		{
			if (m_rsaKey)
			{
				m_rsaKey.reset();
			}

			bool valid = false;

			try
			{
				Botan::DataSource_Memory source(key, len);
				void* key = loadKey(source, password);
				m_rsaKey.reset(static_cast<keyType*>(key));

				if (m_rsaKey)
				{
					valid = checkKey(false);
				}
			}
			catch (std::exception&)
			{
			}

			return valid;
		}

		// checks if this key is valid
		template<typename keyType>
		bool RsaKey<keyType>::checkKey(bool strong)
		{
			if (rsaKey())
			{
				return rsaKey()->check_key(*rndNumberGenerator(), strong);
			}

			return false;
		}

		// return the random number generator
		template<typename keyType>
		Botan::RandomNumberGenerator* RsaKey<keyType>::rndNumberGenerator()
		{
			return &m_randomGenerator;
		}

		// return rsa key
		template<typename keyType>
		std::shared_ptr<const keyType> RsaKey<keyType>::rsaKey()
		{
			return m_rsaKey;
		}

		// set rsa key
		template<typename keyType>
		void RsaKey<keyType>::setRsaKey(std::shared_ptr<keyType> key)
		{
			m_rsaKey = key;
		}

		// PIMPL implementation for RsaPrivateKey
		class RsaPrivateKey::RsaPrivateKeyImpl : public RsaKey<Botan::RSA_PrivateKey>
		{
		public:
			virtual bool saveToFile(const std::string& filename, const std::string& password = "") override;

			bool generate(uint32 keyLengthBits);
			std::string sign(const uint8* data, uint32 dataSize);
			uint32 getBitLength();

		protected:
			virtual void* loadKey(const std::string& filename, const std::string& password = "") override;
			virtual void* loadKey(Botan::DataSource& source, const std::string& password = "") override;
		};

		// PIMPL method implementation: load key from file
		void* RsaPrivateKey::RsaPrivateKeyImpl::loadKey(const std::string& filename, const std::string& password)
		{
			try
			{
				return Botan::PKCS8::load_key(filename, *rndNumberGenerator(), password);
			}
			catch (std::exception&)
			{
			}

			return nullptr;
		}

		// PIMPL method implementation: load key from memory
		void* RsaPrivateKey::RsaPrivateKeyImpl::loadKey(Botan::DataSource& source, const std::string& password)
		{
			try
			{
				return Botan::PKCS8::load_key(source, *rndNumberGenerator(), password);
			}
			catch (std::exception&)
			{
			}

			return nullptr;
		}

		// PIMPL method implementation: save the key into a file
		bool RsaPrivateKey::RsaPrivateKeyImpl::saveToFile(const std::string& filename, const std::string& password)
		{
			bool written = false;

			if (rsaKey())
			{
				std::string encoded;
				if (password.empty())
				{
					encoded = Botan::PKCS8::PEM_encode(*rsaKey());
				}
				else
				{
					encoded = Botan::PKCS8::PEM_encode(*rsaKey(), *rndNumberGenerator(), password);
				}

				FILE* file = fopen(filename.c_str(), "wt");
				if (file)
				{
					size_t charWritten = fwrite(encoded.c_str(), 1, encoded.size(), file);
					written = (charWritten == encoded.size());
					fclose(file);
				}
			}

			return written;
		}

		// PIMPL method implementation: generate a key with given length
		bool RsaPrivateKey::RsaPrivateKeyImpl::generate(uint32 keyLengthBits)
		{
			std::shared_ptr<Botan::RSA_PrivateKey> privKey;

			try
			{
				privKey = std::make_shared<Botan::RSA_PrivateKey>(*rndNumberGenerator(), keyLengthBits);
			}
			catch (std::exception&)
			{
				return false;
			}

			setRsaKey(privKey);

			return true;
		}

		// PIMPL method implementation: encrypt data with this private key
		std::string RsaPrivateKey::RsaPrivateKeyImpl::sign(const uint8* data, uint32 dataSize)
		{
			if (!rsaKey())
			{
				return std::string();
			}

			Botan::PK_Signer signer(*rsaKey(), *rndNumberGenerator(), "EMSA4(SHA-256)");
			std::vector<uint8_t> signature = signer.sign_message(data, dataSize, *rndNumberGenerator());
			std::string signatureString((const char*)signature.data(), signature.size());
			return signatureString;
		}

		// PIMPL method implementation: return bit length
		uint32 RsaPrivateKey::RsaPrivateKeyImpl::getBitLength()
		{
			if (rsaKey())
			{
				return 0;
			}

			return (uint32)rsaKey()->get_n().bits();
		}

		// PIMPL implementation for RsaPublicKey
		class RsaPublicKey::RsaPublicKeyImpl : public RsaKey<Botan::RSA_PublicKey>
		{
		public:
			virtual bool saveToFile(const std::string& filename, const std::string& password = "") override;
			bool verify(const uint8* data, uint32 dataSize, const uint8* signature, uint32 signatureSize);

		protected:
			virtual void* loadKey(const std::string& filename, const std::string& password = "") override;
			virtual void* loadKey(Botan::DataSource& source, const std::string& password = "") override;
		};

		// PIMPL method implementation: load key from file
		void* RsaPublicKey::RsaPublicKeyImpl::loadKey(const std::string& filename, const std::string& password)
		{
			try
			{
				return Botan::X509::load_key(filename);
			}
			catch (std::exception&)
			{
			}

			return nullptr;
		}

		// PIMPL method implementation: load key from memory
		void* RsaPublicKey::RsaPublicKeyImpl::loadKey(Botan::DataSource& source, const std::string& password)
		{
			try
			{
				return Botan::X509::load_key(source);
			}
			catch (std::exception&)
			{
			}

			return nullptr;
		}

		// PIMPL method implementation: save the key into a file
		bool RsaPublicKey::RsaPublicKeyImpl::saveToFile(const std::string& filename, const std::string& password)
		{
			bool written = false;

			if (rsaKey())
			{
				std::string encoded = Botan::X509::PEM_encode(*rsaKey());
				FILE* file = fopen(filename.c_str(), "wt");
				if (file)
				{
					size_t charWritten = fwrite(encoded.c_str(), 1, encoded.size(), file);
					written = (charWritten == encoded.size());
					fclose(file);
				}
			}

			return written;
		}

		// PIMPL method implementation: verify signature with this public key
		bool RsaPublicKey::RsaPublicKeyImpl::verify(const uint8* data, uint32 dataSize, const uint8* signature, uint32 signatureSize)
		{
			if (!rsaKey())
			{
				return false;
			}

			Botan::PK_Verifier verifier(*rsaKey(), "EMSA4(SHA-256)");
			return verifier.verify_message(data, dataSize, signature, signatureSize);
		}

		RsaPrivateKey::RsaPrivateKey()
			: pImpl(new RsaPrivateKeyImpl()/*, ImplDeleter<RsaPrivateKeyImpl>()*/)
		{
		}

		RsaPrivateKey::~RsaPrivateKey()
		{
		}

		RsaPrivateKey::operator bool() const
		{
			return pImpl && pImpl->rsaKey();
		}

		bool RsaPrivateKey::loadFromFile(const std::string& filename, const std::string& password)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->loadFromFile(filename, password);
		}

		bool RsaPrivateKey::loadFromMemory(const uint8* key, uint32 len, const std::string& password)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->loadFromMemory(key, len, password);
		}

		bool RsaPrivateKey::saveToFile(const std::string& filename, const std::string& password)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->saveToFile(filename, password);
		}

		bool RsaPrivateKey::generate(uint32 keyLengthBits)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->generate(keyLengthBits);
		}

		std::string RsaPrivateKey::sign(const uint8* data, uint32 dataSize)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->sign(data, dataSize);
		}

		uint32 RsaPrivateKey::getBitLength() const
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->getBitLength();
		}

		RsaPublicKey::RsaPublicKey()
			: pImpl(new RsaPublicKeyImpl()/*, ImplDeleter<RsaPublicKeyImpl>()*/)
		{
		}

		RsaPublicKey::~RsaPublicKey()
		{
		}

		RsaPublicKey::operator bool() const
		{
			return pImpl && pImpl->rsaKey();
		}

		bool RsaPublicKey::loadFromFile(const std::string& filename)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->loadFromFile(filename);
		}

		bool RsaPublicKey::loadFromMemory(const uint8* key, uint32 len)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->loadFromMemory(key, len);
		}

		bool RsaPublicKey::saveToFile(const std::string& filename)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->saveToFile(filename);
		}

		bool RsaPublicKey::verify(const uint8* data, uint32 dataSize, const uint8* signature, uint32 signatureSize)
		{
			if (!pImpl)
			{
				return false;
			}

			return pImpl->verify(data, dataSize, signature, signatureSize);
		}

		// PIMPL implementation for Certificate
		class Certificate::CertificateImpl
		{
		public:
			std::shared_ptr<Botan::X509_Certificate> m_cert;
		};

		Certificate::Certificate()
			: pImpl(new CertificateImpl()/*, ImplDeleter<CertificateImpl>()*/)
		{
		}

		Certificate::~Certificate()
		{
		}

		Certificate::Certificate(const Certificate& other)
		{
			pImpl.reset(new CertificateImpl(*other.pImpl));
		}

		Certificate& Certificate::operator=(const Certificate& other)
		{
			pImpl.reset(new CertificateImpl(*other.pImpl));
			return *this;
		}

		Certificate::operator bool()
		{
			return pImpl && pImpl->m_cert;
		}

		bool Certificate::loadFromMemory(const uint8* cert, uint32 len)
		{
			if (!pImpl)
			{
				pImpl.reset(new CertificateImpl());
			}

			bool valid = false;

			try
			{
				Botan::DataSource_Memory source((const Botan::byte*) cert, len);
				pImpl->m_cert = std::make_shared<Botan::X509_Certificate>(source);
				valid = true;
			}
			catch (std::exception&)
			{
			}

			return valid;
		}

		std::vector<Certificate> Certificate::loadCertificatesFromMemory(const uint8* certs, uint32 totalLen)
		{
			std::vector<Certificate> certContainer;
			try
			{
				Botan::DataSource_Memory source((const Botan::byte*) certs, totalLen);
				while (source.end_of_data() == false)
				{
					certContainer.push_back(Certificate());
					certContainer.back().pImpl->m_cert = std::make_shared<Botan::X509_Certificate>(source);
				}
			}
			catch (std::exception&)
			{
			}
			return certContainer;
		}

		bool Certificate::generateCertificate(RsaPrivateKey& privateKey, const Parameters& params)
		{
			if (!privateKey)
			{
				return false;
			}

			Botan::X509_Cert_Options options;
			options.common_name = params.m_commonName;
			options.country = params.m_country;
			options.organization = params.m_organization;
			options.email = params.m_email;

			if (params.m_validStartTime.valid() && params.m_validEndTime.valid() && params.m_validStartTime > params.m_validEndTime)
			{
				return false;
			}

			if (params.m_validStartTime.valid())
			{
				options.not_before(params.m_validStartTime.toString(vcslib::Time::AC_Days));
			}

			if (params.m_validEndTime.valid())
			{
				options.not_after(params.m_validEndTime.toString(vcslib::Time::AC_Days));
			}

			pImpl->m_cert = std::make_shared<Botan::X509_Certificate>(
				Botan::X509::create_self_signed_cert(options, *(privateKey.pImpl->rsaKey()), "SHA-256", *(privateKey.pImpl->rndNumberGenerator())));

			return true;
		}

		bool Certificate::saveToFile(const std::string& filename)
		{
			bool written = false;
			if (pImpl->m_cert)
			{
				std::string encoded = pImpl->m_cert->PEM_encode();
				FILE* file = fopen(filename.c_str(), "wt");
				if (file)
				{
					size_t charWritten = fwrite(encoded.c_str(), 1, encoded.size(), file);
					written = (charWritten == encoded.size());
					fclose(file);
				}
			}
			return written;
		}

		std::string Certificate::getCommonName() const
		{
			std::string cn;
			if (pImpl->m_cert)
			{
				std::vector<std::string> info = pImpl->m_cert->subject_info("CommonName");
				if (info.empty() == false) {
					cn = info[0];
				}
			}
			return cn;
		}

		RsaPublicKey Certificate::getPublicKey() const
		{
			RsaPublicKey pubKey;
			if (pImpl->m_cert)
			{
				std::shared_ptr<Botan::Public_Key> key(pImpl->m_cert->subject_public_key());
				pubKey.pImpl->setRsaKey(std::dynamic_pointer_cast<Botan::RSA_PublicKey>(key));
			}

			return pubKey;
		}

		std::string Certificate::getEncodedCertificate(CertEncoding encoding) const
		{
			std::string certEncode;
			if (pImpl->m_cert)
			{
				if (encoding == ceBER)
				{
					std::vector<uint8_t> encode = pImpl->m_cert->BER_encode();
					certEncode.resize(encode.size());
					memcpy(&certEncode[0], encode.data(), encode.size());
				}
				else if (encoding == cePEM)
				{
					certEncode = pImpl->m_cert->PEM_encode();
				}
				else
				{
					certEncode = std::string();
				}
			}

			return certEncode;
		}

		// PIMPL implementation for CertificateStore
		class CertificateStore::CertificateStoreImpl
		{
		public:
			Botan::Certificate_Store_In_Memory m_store;
		};

		CertificateStore::CertificateStore()
			: pImpl(new CertificateStoreImpl()/*, ImplDeleter<CertificateStoreImpl>()*/)
		{
		}

		CertificateStore::~CertificateStore()
		{
		}

		bool CertificateStore::addCertificate(const Certificate& cert)
		{
			if (!pImpl)
			{
				return false;
			}

			if (!cert.pImpl->m_cert)
			{
				return false;
			}

			pImpl->m_store.add_certificate(cert.pImpl->m_cert);

			return true;
		}

		bool CertificateStore::validateCertificate(const std::vector<Certificate>& end_certs)
		{
			if (!pImpl)
			{
				return false;
			}

			if (end_certs.empty())
			{
				return false;
			}

			std::vector<Botan::X509_Certificate> endCerts;

			// extract X509 certificates 
			for (const auto& cert : end_certs)
			{
				if (!cert.pImpl->m_cert)
				{
					return false;
				}
				endCerts.push_back(*cert.pImpl->m_cert);
			}

			bool valid = false;
			try
			{
				// validate certificate path
				Botan::Path_Validation_Result validationResult =
					Botan::x509_path_validate(endCerts, Botan::Path_Validation_Restrictions(), pImpl->m_store);

				valid = validationResult.result() == Botan::Certificate_Status_Code::VERIFIED;
			}
			catch (std::exception&)
			{
				// in case of any exception, validation is failed
			}

			return valid;
		}

		void* CertificateStore::X509CertificateStore()
		{
			if (!pImpl)
			{
				return nullptr;
			}

			return &pImpl->m_store;
		}
	}

}
