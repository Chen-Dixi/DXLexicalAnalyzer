//
//  Log.h
//  置换缺失率模拟
//
//  Created by Dixi-Chen on 16/12/11.
//  Copyright © 2016年 Dixi-Chen. All rights reserved.
//

#ifndef Log_h
#define Log_h

#undef XDBUG
#define XDBUG(fmt, ...) printf("DEBUG:" fmt"\n", ##__VA_ARGS__);

#undef XERROR
#define XERROR(fmt, ...) printf("ERROR:" fmt"\n", ##__VA_ARGS__);

#undef XINFO
#define XINFO(fmt, ...) printf("INFO:" fmt"\n", ##__VA_ARGS__);

#endif /* Log_h */
