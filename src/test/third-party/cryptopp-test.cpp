// TootDesk - Desktop Mastodon Client
// Copyright (C) 2018 Igor Siemienowicz
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

/**
 * \file cryptopp-test.cpp
 * Simple test to verify the integration of the Crypto++ library
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <iostream>

#include <catch/catch.hpp>

#include <crypto++/modes.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>

// Basic verification of Crypto++ availability
TEST_CASE("cryptopp", "[third-party][current]")
{

    // Set up the key and IV
    //
    // TODO generate these randomly
    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    std::string
        plaintext = "This is the plaintext"
        , ciphertext
        , decrypted_text;

    // Set up the crypto transformation
    CryptoPP::AES::Encryption
        aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption
        cbcEncryption(aesEncryption, iv);

    CryptoPP::StreamTransformationFilter
        encryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));

    // Perform the encryption
    encryptor.Put(
        reinterpret_cast<const unsigned char*>(plaintext.c_str()),
        plaintext.length());
    encryptor.MessageEnd();

    // Ciphertext has been generated, and is different from the plaintext
    REQUIRE(!ciphertext.empty());
    REQUIRE(ciphertext != plaintext);

    // Decryption
    CryptoPP::AES::Decryption
        aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption
        cbcDecryption(aesDecryption, iv);
    CryptoPP::StreamTransformationFilter
        decryptor(cbcDecryption, new CryptoPP::StringSink(decrypted_text));
    decryptor.Put(
        reinterpret_cast<const unsigned char*>(ciphertext.c_str()),
        ciphertext.size());
    decryptor.MessageEnd();

    // Decrypted text has been generated, and is the same as the original
    // plaintext
    REQUIRE(!decrypted_text.empty());
    REQUIRE(decrypted_text == plaintext);

    // std::cout
    //     << std::endl << "[DEBUG] plain   : " << plaintext
    //     << std::endl << "[DEBUG] cipher  : ";
    // for (auto c : ciphertext)
    //     std::cout << std::hex << (0xff & static_cast<byte>(c));    
    // std::cout
    //     << std::endl << "[DEBUG] decryped: " << decrypted_text
    //     << std::endl;

}   // end cryptopp test
