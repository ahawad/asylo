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

#include <array>
#include <memory>
#include <string>

#include <gtest/gtest.h>
#include "absl/memory/memory.h"
#include "asylo/enclave_manager.h"
#include "asylo/platform/primitives/examples/hello_enclave.h"
#include "asylo/platform/primitives/extent.h"
#include "asylo/platform/primitives/test/test_backend.h"
#include "asylo/platform/primitives/untrusted_primitives.h"
#include "asylo/platform/primitives/util/dispatch_table.h"
#include "asylo/platform/primitives/util/message.h"
#include "asylo/test/util/status_matchers.h"
#include "asylo/util/status_macros.h"
#include "asylo/util/statusor.h"

using ::testing::SizeIs;
using ::testing::StrEq;

namespace asylo {
namespace primitives {
namespace test {

class HelloTest : public ::testing::Test {
 protected:
  // Loads an instance of a test enclave, aborting on failure.
  void SetUp() override {
    EnclaveManager::Configure(EnclaveManagerOptions());
    client_ = test::TestBackend::Get()->LoadTestEnclaveOrDie(
        /*enclave_name=*/"hello_test", absl::make_unique<DispatchTable>());
    ASYLO_EXPECT_OK(client_->exit_call_provider()->RegisterExitHandler(
        kExternalHelloHandler, ExitHandler{HelloHandler}));
  }

  void TearDown() override { ASYLO_EXPECT_OK(client_->Destroy()); }

  std::shared_ptr<Client> client_;

 private:
  // When the enclave asks for it, send "Hello".
  static Status HelloHandler(std::shared_ptr<Client> client, void *context,
                             NativeParameterStack *params) {
    // Push our message on to the output message to pass to the enclave.
    MessageWriter out;
    out.Push("Hello");
    out.Serialize(params);
    return Status::OkStatus();
  }
};

TEST_F(HelloTest, Hello) {
  MessageReader out;
  ASYLO_ASSERT_OK(client_->EnclaveCall(kHelloEnclaveSelector, nullptr, &out));
  EXPECT_THAT(out, SizeIs(1));
  EXPECT_THAT(out.next().As<char>(), StrEq("Hello, World!"));
}

}  // namespace test
}  // namespace primitives
}  // namespace asylo
