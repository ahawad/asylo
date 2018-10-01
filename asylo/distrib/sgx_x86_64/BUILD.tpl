#
# Copyright 2018 Asylo authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

licenses(["notice"])  # Apache v2.0

package(default_visibility = ["//visibility:public"])


cc_toolchain_suite(
    name = "crosstool",
    toolchains = {
    "sgx_x86_64": "cc-compiler",
    "sgx_x86_64|compiler": "cc-compiler",
    },
)

filegroup(
    name = "everything",
    srcs = glob(
        include = ["**"],
        exclude = [
            "BUILD",
            "CROSSTOOL",
        ],
    ) + [
        "@com_google_asylo//asylo/platform/posix:posix_headers",
        "@com_google_asylo//asylo/platform/system:system_headers",
    ],
)

cc_toolchain(
    name = "cc-compiler",
    all_files = ":everything",
    compiler_files = ":everything",
    cpu = "sgx_x86_64",
    dwp_files = ":everything",
    dynamic_runtime_libs = [":everything"],
    linker_files = ":everything",
    objcopy_files = ":everything",
    static_runtime_libs = [":everything"],
    strip_files = ":everything",
    supports_param_files = 0,
)

