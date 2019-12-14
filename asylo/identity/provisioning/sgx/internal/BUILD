#
# Copyright 2019 Asylo authors
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
load("//asylo/bazel:asylo.bzl", "cc_test_and_cc_enclave_test")
load("//asylo/bazel:copts.bzl", "ASYLO_DEFAULT_COPTS")

# This package defines Asylo-internal targets for SGX provisioning.

licenses(["notice"])

proto_library(
    name = "platform_provisioning_proto",
    srcs = ["platform_provisioning.proto"],
    visibility = ["//asylo:implementation"],
)

cc_proto_library(
    name = "platform_provisioning_cc_proto",
    visibility = ["//asylo:implementation"],
    deps = [":platform_provisioning_proto"],
)

proto_library(
    name = "sgx_pcs_client_proto",
    srcs = ["sgx_pcs_client.proto"],
    visibility = ["//asylo:implementation"],
)

cc_proto_library(
    name = "sgx_pcs_client_cc_proto",
    visibility = ["//asylo:implementation"],
    deps = [":sgx_pcs_client_proto"],
)

proto_library(
    name = "tcb_proto",
    srcs = ["tcb.proto"],
    visibility = ["//asylo:implementation"],
    deps = [
        ":platform_provisioning_proto",
        "//asylo/identity/sgx:machine_configuration_proto",
        "@com_google_protobuf//:timestamp_proto",
    ],
)

cc_proto_library(
    name = "tcb_cc_proto",
    visibility = ["//asylo:implementation"],
    deps = [":tcb_proto"],
)

cc_library(
    name = "mock_sgx_pcs_client",
    testonly = 1,
    hdrs = ["mock_sgx_pcs_client.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":platform_provisioning_cc_proto",
        ":sgx_pcs_client",
        "//asylo/identity/sgx:machine_configuration_cc_proto",
        "//asylo/util:status",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "platform_provisioning",
    srcs = ["platform_provisioning.cc"],
    hdrs = ["platform_provisioning.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":platform_provisioning_cc_proto",
        "//asylo/crypto/util:trivial_object_util",
        "//asylo/identity/sgx:hardware_types",
        "//asylo/identity/sgx:machine_configuration_cc_proto",
        "//asylo/util:status",
        "@com_google_absl//absl/strings",
    ],
)

cc_test_and_cc_enclave_test(
    name = "platform_provisioning_test",
    srcs = ["platform_provisioning_test.cc"],
    copts = ASYLO_DEFAULT_COPTS,
    deps = [
        ":platform_provisioning",
        ":platform_provisioning_cc_proto",
        "//asylo/crypto/util:trivial_object_util",
        "//asylo/identity/sgx:hardware_types",
        "//asylo/test/util:status_matchers",
        "//asylo/test/util:test_main",
        "//asylo/util:status",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "provisioning_consistency_checker",
    srcs = ["provisioning_consistency_checker.cc"],
    hdrs = ["provisioning_consistency_checker.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":tcb_cc_proto",
        ":tcb_container_util",
        "//asylo/identity/sgx:pck_certificates_cc_proto",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/strings",
    ],
)

cc_test_and_cc_enclave_test(
    name = "provisioning_consistency_checker_test",
    srcs = ["provisioning_consistency_checker_test.cc"],
    copts = ASYLO_DEFAULT_COPTS,
    deps = [
        ":provisioning_consistency_checker",
        "//asylo/test/util:test_main",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "sgx_pcs_client",
    hdrs = ["sgx_pcs_client.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":platform_provisioning_cc_proto",
        ":sgx_pcs_client_cc_proto",
        ":tcb_cc_proto",
        "//asylo/crypto:certificate_cc_proto",
        "//asylo/identity/sgx:machine_configuration_cc_proto",
        "//asylo/identity/sgx:pck_certificates_cc_proto",
        "//asylo/util:status",
    ],
)

cc_library(
    name = "sgx_pcs_client_impl",
    srcs = ["sgx_pcs_client_impl.cc"],
    hdrs = ["sgx_pcs_client_impl.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":platform_provisioning",
        ":platform_provisioning_cc_proto",
        ":sgx_pcs_client",
        ":sgx_pcs_client_cc_proto",
        ":signed_tcb_info_from_json",
        ":tcb",
        "//asylo/crypto:algorithms_cc_proto",
        "//asylo/crypto:asymmetric_encryption_key",
        "//asylo/crypto:certificate_util",
        "//asylo/identity/sgx:machine_configuration_cc_proto",
        "//asylo/identity/sgx:pck_certs_from_json",
        "//asylo/util:function_deleter",
        "//asylo/util:hex_util",
        "//asylo/util:http_fetcher",
        "//asylo/util:logging",
        "//asylo/util:proto_enum_util",
        "//asylo/util:status",
        "//asylo/util:url_util",
        "@com_github_curl_curl//:curl",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_test(
    name = "sgx_pcs_client_impl_test",
    srcs = ["sgx_pcs_client_impl_test.cc"],
    copts = ASYLO_DEFAULT_COPTS,
    deps = [
        ":platform_provisioning_cc_proto",
        ":sgx_pcs_client",
        ":sgx_pcs_client_cc_proto",
        ":sgx_pcs_client_impl",
        "//asylo/crypto:algorithms_cc_proto",
        "//asylo/crypto/util:byte_container_view",
        "//asylo/identity/sgx:machine_configuration_cc_proto",
        "//asylo/test/util:proto_matchers",
        "//asylo/test/util:status_matchers",
        "//asylo/test/util:test_main",
        "//asylo/util:error_codes",
        "//asylo/util:proto_enum_util",
        "//asylo/util:status",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "signed_tcb_info_from_json",
    srcs = ["signed_tcb_info_from_json.cc"],
    hdrs = ["signed_tcb_info_from_json.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":sgx_pcs_client_cc_proto",
        "//asylo/util:error_codes",
        "//asylo/util:hex_util",
        "//asylo/util:proto_struct_util",
        "//asylo/util:status",
        "@com_google_absl//absl/strings",
        "@com_google_protobuf//:cc_wkt_protos",
        "@com_google_protobuf//:protobuf",
        "@com_googlesource_code_re2//:re2",
    ],
)

cc_test_and_cc_enclave_test(
    name = "signed_tcb_info_from_json_test",
    srcs = ["signed_tcb_info_from_json_test.cc"],
    copts = ASYLO_DEFAULT_COPTS,
    deps = [
        ":sgx_pcs_client_cc_proto",
        ":signed_tcb_info_from_json",
        "//asylo/test/util:proto_matchers",
        "//asylo/test/util:status_matchers",
        "//asylo/test/util:test_main",
        "//asylo/util:error_codes",
        "//asylo/util:proto_enum_util",
        "//asylo/util:status",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:cc_wkt_protos",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "tcb",
    srcs = ["tcb.cc"],
    hdrs = ["tcb.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":platform_provisioning",
        ":tcb_cc_proto",
        ":tcb_container_util",
        "//asylo/crypto/util:byte_container_view",
        "//asylo/identity/sgx:hardware_types",
        "//asylo/util:hex_util",
        "//asylo/util:status",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_protobuf//:cc_wkt_protos",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_test_and_cc_enclave_test(
    name = "tcb_test",
    srcs = ["tcb_test.cc"],
    copts = ASYLO_DEFAULT_COPTS,
    deps = [
        ":platform_provisioning_cc_proto",
        ":tcb",
        ":tcb_cc_proto",
        "//asylo/identity/sgx:machine_configuration_cc_proto",
        "//asylo/test/util:proto_matchers",
        "//asylo/test/util:status_matchers",
        "//asylo/test/util:test_main",
        "//asylo/util:logging",
        "//asylo/util:status",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:span",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:cc_wkt_protos",
    ],
)

cc_library(
    name = "tcb_container_util",
    srcs = ["tcb_container_util.cc"],
    hdrs = ["tcb_container_util.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo/identity/sgx:__subpackages__"],
    deps = [
        ":platform_provisioning_cc_proto",
        ":tcb_cc_proto",
        "//asylo/crypto/util:byte_container_util",
        "//asylo/crypto/util:byte_container_view",
        "//asylo/identity/sgx:machine_configuration_cc_proto",
        "//asylo/util:status",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_test_and_cc_enclave_test(
    name = "tcb_container_util_test",
    srcs = ["tcb_container_util_test.cc"],
    copts = ASYLO_DEFAULT_COPTS,
    deps = [
        ":platform_provisioning",
        ":platform_provisioning_cc_proto",
        ":tcb",
        ":tcb_cc_proto",
        ":tcb_container_util",
        "//asylo/identity/sgx:hardware_types",
        "//asylo/test/util:test_main",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/random",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "tcb_info_from_json",
    srcs = ["tcb_info_from_json.cc"],
    hdrs = ["tcb_info_from_json.h"],
    copts = ASYLO_DEFAULT_COPTS,
    visibility = ["//asylo:implementation"],
    deps = [
        ":platform_provisioning",
        ":platform_provisioning_cc_proto",
        ":tcb",
        ":tcb_cc_proto",
        ":tcb_container_util",
        "//asylo/util:hex_util",
        "//asylo/util:logging",
        "//asylo/util:proto_struct_util",
        "//asylo/util:status",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_absl//absl/time",
        "@com_google_protobuf//:cc_wkt_protos",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_test_and_cc_enclave_test(
    name = "tcb_info_from_json_test",
    srcs = ["tcb_info_from_json_test.cc"],
    copts = ASYLO_DEFAULT_COPTS,
    deps = [
        ":platform_provisioning_cc_proto",
        ":tcb",
        ":tcb_cc_proto",
        ":tcb_info_from_json",
        "//asylo/test/util:output_collector",
        "//asylo/test/util:proto_matchers",
        "//asylo/test/util:status_matchers",
        "//asylo/test/util:test_main",
        "//asylo/util:logging",
        "//asylo/util:status",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:span",
        "@com_google_googletest//:gtest",
        "@com_google_protobuf//:cc_wkt_protos",
        "@com_google_protobuf//:protobuf",
    ],
)