constraint_value(
  name = "asylo",
  constraint_setting = "@platforms//os",
  visibility = ["//visibility:public"],
)

platform (
  name = "asylo_x86_64",
  constraint_values = [
    ":asylo",
    "@platforms//cpu:x86_64",
  ],
  visibility = ["//visibility:private"],
)

