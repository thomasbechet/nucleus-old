#ifndef NUVK_LOGGER_H
#define NUVK_LOGGER_H

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

#define NUVK_LOGGER_NAME "[VULKAN]["__FILE__":"S__LINE__"] "

#endif