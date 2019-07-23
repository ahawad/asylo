/*
 *
 * Copyright 2019 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "asylo/crypto/fake_signing_key.h"

#include <cstdint>
#include <memory>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "asylo/crypto/signing_key.h"
#include "asylo/crypto/util/byte_container_view.h"
#include "asylo/test/util/status_matchers.h"
#include "asylo/util/cleansing_types.h"
#include "asylo/util/status.h"

namespace asylo {
namespace {

constexpr char kTestKeyDer[] = "Coffee";

constexpr char kTestMessage[] = "Fun";

constexpr char kTestMessageSignature[] = "CoffeeFun";

constexpr char kOtherMessageSignature[] = "Coffee#1";

constexpr char kOtherKeySignature[] = "Not coffeeFun";

// Verify that a FakeVerifyingKey produces an equivalent DER-encoding through
// SerializeToDer().
TEST(FakeSigningKeyTest, VerifyingKeySerializeToDer) {
  FakeVerifyingKey verifying_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);
  EXPECT_THAT(verifying_key.SerializeToDer(), IsOkAndHolds(kTestKeyDer));
}

// Verify that a FakeVerifyingKey with a status passed at construction for the
// DER-encoded key value returns that status for SerializeToDer().
TEST(FakeSigningKeyTest, VerifyingKeyFailsSerializeToDer) {
  FakeVerifyingKey verifying_key(
      UNKNOWN_SIGNATURE_SCHEME,
      Status(error::GoogleError::FAILED_PRECONDITION, kTestMessage));

  EXPECT_THAT(verifying_key.SerializeToDer().status(),
              StatusIs(error::GoogleError::FAILED_PRECONDITION, kTestMessage));
}

// Verify that a FakeVerifyingKey with a status passed at construction for the
// DER-encoded key value returns that status for Verify().
TEST(FakeSigningKeyTest, VerifyingKeyConstructedWithStatusVerify) {
  FakeVerifyingKey verifying_key(
      UNKNOWN_SIGNATURE_SCHEME,
      Status(error::GoogleError::FAILED_PRECONDITION, kTestMessage));

  EXPECT_THAT(verifying_key.Verify(kTestMessage, kTestMessageSignature),
              StatusIs(error::GoogleError::FAILED_PRECONDITION, kTestMessage));
}

// Verify that a FakeVerifyingKey verifies a valid signature.
TEST(FakeSigningKeyTest, VerifySuccess) {
  FakeVerifyingKey verifying_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);

  ASYLO_EXPECT_OK(verifying_key.Verify(kTestMessage, kTestMessageSignature));
}

// Verify that a FakeVerifyingKey does not verify a signature signed by a
// different key.
TEST(FakeSigningKeyTest, VerifyWithOtherKeySignatureFails) {
  FakeVerifyingKey verifying_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);

  EXPECT_THAT(verifying_key.Verify(kTestMessage, kOtherKeySignature),
              StatusIs(error::GoogleError::UNAUTHENTICATED));
}

// Verify that a FakeVerifyingKey does not verify a signature for a different
// message.
TEST(FakeSigningKeyTest, VerifyOtherMessageSignatureFails) {
  FakeVerifyingKey verifying_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);

  EXPECT_THAT(verifying_key.Verify(kTestMessage, kOtherMessageSignature),
              StatusIs(error::GoogleError::UNAUTHENTICATED));
}

// Verify that GetSignatureScheme() indicates the signature scheme passed at
// construction time.
TEST(FakeSigningKeyTest, SignatureScheme) {
  FakeVerifyingKey verifying_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);
  EXPECT_EQ(verifying_key.GetSignatureScheme(),
            SignatureScheme::UNKNOWN_SIGNATURE_SCHEME);

  FakeSigningKey signing_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);
  EXPECT_EQ(signing_key.GetSignatureScheme(),
            SignatureScheme::UNKNOWN_SIGNATURE_SCHEME);
}

// Verify that Sign() creates the correct signature.
TEST(FakeSigningKeyTest, CorrectSignature) {
  FakeSigningKey signing_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);

  std::vector<uint8_t> signature;
  ASYLO_ASSERT_OK(signing_key.Sign(kTestMessage, &signature));

  EXPECT_EQ(ByteContainerView(signature),
            ByteContainerView(kTestMessageSignature));
}

// Verify that a FakeSigningKey produces the correct DER-encoding by
// SerializeToDer().
TEST(FakeSigningKeyTest, SigningKeySerializeToDer) {
  FakeSigningKey signing_key(UNKNOWN_SIGNATURE_SCHEME, kTestKeyDer);

  CleansingVector<uint8_t> serialized_key;
  ASYLO_ASSERT_OK(signing_key.SerializeToDer(&serialized_key));

  EXPECT_EQ(ByteContainerView(serialized_key), ByteContainerView(kTestKeyDer));
}

// Verify that a FakeSigningKey returns the non-OK Status passed at construction
// as the error for SerializeToDer().
TEST(FakeSigningKeyTest, SigningKeySerializeToDerFailure) {
  FakeSigningKey signing_key(
      UNKNOWN_SIGNATURE_SCHEME,
      Status(error::GoogleError::FAILED_PRECONDITION, kTestMessage));

  CleansingVector<uint8_t> serialized_key;
  EXPECT_THAT(signing_key.SerializeToDer(&serialized_key),
              StatusIs(error::GoogleError::FAILED_PRECONDITION, kTestMessage));
}

// Verify that a FakeSigningKey returns the status passed at construction as the
// error for Sign().
TEST(FakeSigningKeyTest, SigningKeySignFailure) {
  FakeSigningKey signing_key(
      UNKNOWN_SIGNATURE_SCHEME,
      Status(error::GoogleError::FAILED_PRECONDITION, kTestMessage));

  std::vector<uint8_t> signature;
  EXPECT_THAT(signing_key.Sign(kTestMessage, &signature),
              StatusIs(error::GoogleError::FAILED_PRECONDITION, kTestMessage));
}

}  // namespace
}  // namespace asylo
