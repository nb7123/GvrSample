//
// Created by michael on 17-4-17.
//

#ifndef GVRSAMPLES_LOG_H
#define GVRSAMPLES_LOG_H

#include <android/log.h>
#define LOGI(TAG, ...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, (TAG), __VA_ARGS__))
#define LOGD(TAG, ...) \
  ((void)__android_log_print(ANDROID_LOG_DEBUG, (TAG), __VA_ARGS__))
#define LOGW(TAG, ...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, (TAG), __VA_ARGS__))
#define LOGE(TAG, ...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, (TAG), __VA_ARGS__))

#endif //GVRSAMPLES_LOG_H
