#pragma once

#include <cmath>
#include <cstdint>

#include <algorithm>
#include <array>
#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <jni.h>

#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/native_window_jni.h>

#define dbglog(...) __android_log_print(ANDROID_LOG_DEBUG, "NDK", __VA_ARGS__)

#include "clock.h"
#include "gl.h"

using namespace std;
