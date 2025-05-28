/*
 *
 * Copyright 2018 Asylo authors
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

#include "asylo/util/logging.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace asylo {

#ifdef __ASYLO__
constexpr bool kInsideEnclave = true;
#else  // __ASYLO__
constexpr bool kInsideEnclave = false;
#endif // __ASYLO__

} // namespace asylo
